#include "CompilationEngine.h"
#include "Helper.h"
#include "Token.h"

// General rule, each compile method first prints the current token and at the end
// advances the token. (some exceptions... explain well)

CompilationEngine::CompilationEngine(
  const std::string &otputFilepath, JackTokenizer &tokenizer) : tokenizer_{tokenizer}
{
  outputFile_.open(otputFilepath);

  if (tokenizer.hasMoreTokens())
  {
    tokenizer.advance();
    compileClass_();
  }
}

const Token CompilationEngine::advanceAndGetNextToken()
{
  if (tokenizer_.hasMoreTokens())
  {
    tokenizer_.advance();
    return tokenizer_.getCurrentToken();
  }
  
  throw std::runtime_error("No more tokens remains");
}

void CompilationEngine::compileClass_()
{
  outputFile_ << "<class>" << '\n';

  // The current 3 tokens are "class" followed by '{' and an identifier
  outputFile_ << tokenizer_.getCurrentToken();
  outputFile_ << advanceAndGetNextToken();
  outputFile_ << advanceAndGetNextToken();

  // We might have  0 to many variable declaration of this class
  auto nextToken = advanceAndGetNextToken();
  while (tokenizer_.hasMoreTokens())
  {
    if (nextToken.getTokenType() == JackTokenType::KEYWORD)
    {
      switch (nextToken.getKeyWordType())
      {
        case KeywordType::STATIC:
        case KeywordType::FIELD:
        {
          compileClassVarDec_();
          break;
        }
        case KeywordType::CONSTRUCTOR:
        case KeywordType::FUNCTION:
        case KeywordType::METHOD:
        {
          compileClassSubroutine_();
          break;
        }

        default:
          throw std::runtime_error("This keyword is not meant to be in the class: ");
        break;
      }

      nextToken = tokenizer_.getCurrentToken();
    }
    else
      break;
  }

  // Next token is '{'
  outputFile_ << nextToken;

  outputFile_ << "</class>" << '\n';
}

void CompilationEngine::compileClassVarDec_()
{
  outputFile_ << "<classVarDec>" << '\n';

  // Either "static" or "field".
  outputFile_ << tokenizer_.getCurrentToken();

  // Type
  outputFile_ << advanceAndGetNextToken();

  // Var name
  outputFile_ << advanceAndGetNextToken();

  auto nextToken = advanceAndGetNextToken();

  // check if there are other variables by checking if the next token is a ','
  while (nextToken.getSymbol() == ',')
  {
    outputFile_ << nextToken;

    // Another var name
    outputFile_ << advanceAndGetNextToken();

    nextToken = advanceAndGetNextToken();
  }

  // ;
  outputFile_ << nextToken;
  tokenizer_.advance();
  outputFile_ << "</classVarDec>" << '\n';
}

void CompilationEngine::compileClassSubroutine_()
{
  outputFile_ << "<subroutineDec>" << '\n';

  // constructor, function or method.
  outputFile_ << tokenizer_.getCurrentToken();

  // Type
  outputFile_ << advanceAndGetNextToken();

  // Function's name.
  outputFile_ << advanceAndGetNextToken();

  // (
  outputFile_ << advanceAndGetNextToken();

  tokenizer_.advance();
  compileParameterList_();

  // )
  outputFile_ << tokenizer_.getCurrentToken();

  outputFile_ << "<subroutineBody>" << '\n';

  // {
  outputFile_ << advanceAndGetNextToken();

  tokenizer_.advance();
  compileVarDec_();
  compileStatements_();

  // }
  outputFile_ << tokenizer_.getCurrentToken();

  outputFile_ << "</subroutineBody>" << '\n';
  outputFile_ << "</subroutineDec>" << '\n';
  tokenizer_.advance();
}

void CompilationEngine::compileParameterList_()
{
  outputFile_ << "<parameterList>" << '\n';

  auto currentToken = tokenizer_.getCurrentToken();
  if (currentToken.getSymbol() == ')')
  {
    outputFile_ << "</parameterList>" << '\n';
    return;
  }

  // type
  outputFile_ << currentToken;

  // name
  outputFile_ << advanceAndGetNextToken();

  // Check for comma and repeat the process.
  currentToken = advanceAndGetNextToken();
  while (currentToken.getSymbol() == ',')
  {
    // ,
    outputFile_ << currentToken;

    // type
    outputFile_ << advanceAndGetNextToken();

    // name
    outputFile_ << advanceAndGetNextToken();
    currentToken = advanceAndGetNextToken();
  }

  outputFile_ << "</parameterList>" << '\n';
}

void CompilationEngine::compileVarDec_()
{
  auto currentToken = tokenizer_.getCurrentToken();

  while (currentToken.getTokenType() == JackTokenType::KEYWORD)
  {
    if (currentToken.getKeyWordType() != KeywordType::VAR)
      break;

    outputFile_ << "<varDec>" << '\n';

    // Var
    outputFile_ << currentToken;

    // Type
    outputFile_ << advanceAndGetNextToken();

    // Var name
    outputFile_ << advanceAndGetNextToken();

    currentToken = advanceAndGetNextToken();

    // Check if there are other variables by checking if the next token is a ','
    while (currentToken.getSymbol() == ',')
    {
      outputFile_ << currentToken;

      // Another var name
      outputFile_ << advanceAndGetNextToken();

      currentToken = advanceAndGetNextToken();
    }

    // ;
    outputFile_ << currentToken;
    outputFile_ << "</varDec>" << '\n';
    currentToken = advanceAndGetNextToken();
  }
}

void CompilationEngine::compileReturn_()
{
  outputFile_ << "<returnStatement>" << '\n';

  // Return
  outputFile_ << tokenizer_.getCurrentToken();

  if (advanceAndGetNextToken().getTokenType() != JackTokenType::SYMBOL)
  {
    // Expression
    compileExpression_();
  }

  // ;
  outputFile_ << tokenizer_.getCurrentToken();
  outputFile_ << "</returnStatement>" << '\n';
  tokenizer_.advance();
}

void CompilationEngine::handleKeywordInStatements_(const Token &currentToken)
{
  switch (currentToken.getKeyWordType())
  {
  case KeywordType::LET:
  {
    compileLet_();
    break;
  }
  case KeywordType::IF:
  {
    compileIf_();
    break;
  }
  case KeywordType::WHILE:
  {
    compileWhile_();
    break;
  }
  case KeywordType::DO:
  {
    compileDo_();
    break;
  }
  case KeywordType::RETURN:
  {
    compileReturn_();
    break;
  }
  default:
    throw std::runtime_error("Wrong expression in compileStatements_!");
    break;
  }
}

void CompilationEngine::compileStatements_()
{
  outputFile_ << "<statements>" << '\n';
  auto currentToken = tokenizer_.getCurrentToken();

  while (currentToken.getTokenType() == JackTokenType::KEYWORD)
  {
    handleKeywordInStatements_(currentToken);
    currentToken = tokenizer_.getCurrentToken();
  }

  outputFile_ << "</statements>" << '\n';
}

void CompilationEngine::compileSubroutineCall_()
{
  // subroutine name or class name or variable name.
  outputFile_ << tokenizer_.getCurrentToken();

  // We want to check if we are calling a method on class or variable. We have to check for the
  // '.' symbol.

  auto currentToken = advanceAndGetNextToken();
  while (currentToken.getSymbol() == '.')
  {
    // The previous token was a class or variable name.
    // Subroutine name is the current token.

    // '.'
    outputFile_ << currentToken;

    // subroutine name
    outputFile_ << advanceAndGetNextToken();

    // Set the next token which is '(' to pCurrentToken
    currentToken = advanceAndGetNextToken();
  }

  // (
  outputFile_ << currentToken;

  tokenizer_.advance();
  compileExpressionList_();

  // )
  outputFile_ << tokenizer_.getCurrentToken();

  tokenizer_.advance();
}

void CompilationEngine::compileDo_()
{
  outputFile_ << "<doStatement>" << '\n';

  // do
  outputFile_ << tokenizer_.getCurrentToken();
  tokenizer_.advance();
  compileSubroutineCall_();

  // ;
  outputFile_ << tokenizer_.getCurrentToken();
  outputFile_ << "</doStatement>" << '\n';

  tokenizer_.advance();
}

void CompilationEngine::compileLet_()
{
  outputFile_ << "<letStatement>" << '\n';

  // Let
  outputFile_ << tokenizer_.getCurrentToken();

  // Var name
  outputFile_ << advanceAndGetNextToken();

  // We want to check if we are addressing and array here.
  auto nextToken = advanceAndGetNextToken();

  if ((nextToken.getTokenType() == JackTokenType::SYMBOL) &&
       nextToken.getSymbol() == '[')
  {
    // [
    outputFile_ << nextToken;
    tokenizer_.advance();

    compileExpression_();

    // ]
    outputFile_ << tokenizer_.getCurrentToken();
    nextToken = advanceAndGetNextToken();
  }

  // =
  outputFile_ << nextToken;
  tokenizer_.advance();

  compileExpression_();

  // ;
  outputFile_ << tokenizer_.getCurrentToken();

  tokenizer_.advance();
  outputFile_ << "</letStatement>" << '\n';
}

void CompilationEngine::compileWhile_()
{
  // The code is very similar to if statement. Take out some common functionality, in general 
  // compiling inside a parentheses and inside a body {} is common.
  outputFile_ << "<whileStatement>" << '\n';

  // while
  outputFile_ << tokenizer_.getCurrentToken();

  // '('
  outputFile_ << advanceAndGetNextToken();

  tokenizer_.advance();
  compileExpression_();

  // ')'
  outputFile_ << tokenizer_.getCurrentToken();

  // '{'
  outputFile_ << advanceAndGetNextToken();
  tokenizer_.advance();

  compileStatements_();

  // '}'
  outputFile_ << tokenizer_.getCurrentToken();
  tokenizer_.advance();

  outputFile_ << "</whileStatement>" << '\n';
}

void CompilationEngine::compileIf_()
{
  outputFile_ << "<ifStatement>" << '\n';

  auto compileIfBody = [this]()
  {
    // '{'
    outputFile_ << advanceAndGetNextToken();
    tokenizer_.advance();

    compileStatements_();

    // '}'
    outputFile_ << tokenizer_.getCurrentToken();
    tokenizer_.advance();
  };

  // If
  outputFile_ << tokenizer_.getCurrentToken();

  // '('
  outputFile_ << advanceAndGetNextToken();

  tokenizer_.advance();
  compileExpression_();

  // ')'
  outputFile_ << tokenizer_.getCurrentToken();

  compileIfBody();

  // Check if we have an else statement.
  if (tokenizer_.getCurrentToken().getKeyWordType() == KeywordType::ELSE)
  {
    // else
    outputFile_ << tokenizer_.getCurrentToken();
    compileIfBody();
  }

  outputFile_ << "</ifStatement>" << '\n';
}

void CompilationEngine::compileExpression_()
{
  outputFile_ << "<expression>" << '\n';

  // Compile the current term.
  compileTerm_();

  // Check if we have an operator next, if yes we need to compile more terms.
  auto currentToken = tokenizer_.getCurrentToken();
  while ((currentToken.getTokenType() == JackTokenType::SYMBOL) &&
         (isOp(currentToken.getSymbol())))
  {
    // Print the operator.
    outputFile_ << currentToken;
    tokenizer_.advance();
    compileTerm_();
    currentToken = tokenizer_.getCurrentToken();
  }

  outputFile_ << "</expression>" << '\n';
}

void CompilationEngine::compileTerm_()
{
  outputFile_ << "<term>" << '\n';

  auto currentToken = tokenizer_.getCurrentToken();
  outputFile_ << currentToken;

  // It can be '(' or a unary operator.
  if (currentToken.getTokenType() == JackTokenType::SYMBOL)
  {
    const auto symbol = currentToken.getSymbol();
    tokenizer_.advance();

    if (symbol == '(')
    {
      compileExpression_();

      // ')'
      outputFile_ << tokenizer_.getCurrentToken();
      tokenizer_.advance();
    }
    else if (isUnaryOp(symbol))
      compileTerm_();
    else
      throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
  }
  else
  {
    currentToken = advanceAndGetNextToken();

    // We first want to check if there is a '[' or '(' or '.'
    if (currentToken.getTokenType() == JackTokenType::SYMBOL)
    {
      const auto symbol = currentToken.getSymbol();

      if ((symbol == '(') || (symbol == '[') || (symbol == '.'))
      {
        // Print the symbol.
        outputFile_ << currentToken;
        tokenizer_.advance();

        // Array or call.
        if ((symbol == '[') || (symbol == '('))
        {
          compileExpression_();

          // ']' or ')'
          outputFile_ << tokenizer_.getCurrentToken();
          tokenizer_.advance();
        }

        // Subroutine call.
        else if (symbol == '.')
          compileSubroutineCall_();
        else
          throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" after term");
      }
    }
    else
    {
      outputFile_ << currentToken;
      tokenizer_.advance();
    }
  }

  outputFile_ << "</term>" << '\n';
}

void CompilationEngine::compileExpressionList_()
{
  outputFile_ << "<expressionList>" << '\n';

  // In case there are no expressions
  if (!(tokenizer_.getCurrentToken().getSymbol() == ')'))
  {
    compileExpression_();

    auto currentToken = tokenizer_.getCurrentToken();
    while (currentToken.getSymbol() == ',')
    {
      // ','
      outputFile_ << currentToken;
      tokenizer_.advance();
      compileExpression_();
      currentToken = tokenizer_.getCurrentToken();
    }
  }

  outputFile_ << "</expressionList>" << '\n';
}