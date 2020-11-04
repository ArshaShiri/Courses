#include "CompilationEngine.h"
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

const Token* CompilationEngine::advanceAndGetNextToken()
{
  if (tokenizer_.hasMoreTokens())
  {
    tokenizer_.advance();
    return tokenizer_.getCurrentToken();
  }

  return nullptr;
}

void CompilationEngine::compileClass_()
{
  outputFile_ << "<class>" << '\n';

  // The current 3 tokens are "class" followed by '{' and an identifier
  outputFile_ << *tokenizer_.getCurrentToken();
  outputFile_ << *advanceAndGetNextToken();
  outputFile_ << *advanceAndGetNextToken();

  // We might have  0 to many variable declaration of this class
  auto pNextToken = advanceAndGetNextToken();
  while (tokenizer_.hasMoreTokens())
  {
    if (pNextToken->getTokenType() == JackTokenType::KEYWORD)
    {
      switch (pNextToken->getValue<JackTokenType::KEYWORD>())
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
          throw std::runtime_error("This keyword is not meant to be in the class");
        break;
      }

      pNextToken = tokenizer_.getCurrentToken();
    }
    else
      break;
  }

  // Next token is '{'
  outputFile_ << *pNextToken;

  outputFile_ << "</class>" << '\n';
}

void CompilationEngine::compileClassVarDec_()
{
  outputFile_ << "<classVarDec>" << '\n';

  // Either "static" or "field".
  outputFile_ << *tokenizer_.getCurrentToken();

  // Type
  outputFile_ << *advanceAndGetNextToken();

  // Var name
  outputFile_ << *advanceAndGetNextToken();

  auto pNextToken = advanceAndGetNextToken();

  // check if there are other variables by checking if the next token is a ','
  while (pNextToken->getValue<JackTokenType::SYMBOL>() == ',')
  {
    outputFile_ << *pNextToken;

    // Another var name
    outputFile_ << *advanceAndGetNextToken();

    pNextToken = advanceAndGetNextToken();
  }

  // ;
  outputFile_ << *pNextToken;
  tokenizer_.advance();
  outputFile_ << "</classVarDec>" << '\n';
}

void CompilationEngine::compileClassSubroutine_()
{
  outputFile_ << "<subroutineDec>" << '\n';

  // constructor, function or method.
  outputFile_ << *tokenizer_.getCurrentToken();

  // Type
  outputFile_ << *advanceAndGetNextToken();

  // Function's name.
  outputFile_ << *advanceAndGetNextToken();

  // (
  outputFile_ << *advanceAndGetNextToken();

  tokenizer_.advance();
  compileParameterList_();

  // )
  outputFile_ << *tokenizer_.getCurrentToken();

  outputFile_ << "<subroutineBody>" << '\n';

  // {
  outputFile_ << *advanceAndGetNextToken();

  tokenizer_.advance();
  compileVarDec_();
  compileStatements_();

  // }
  outputFile_ << *tokenizer_.getCurrentToken();

  outputFile_ << "</subroutineBody>" << '\n';
  outputFile_ << "</subroutineDec>" << '\n';
  tokenizer_.advance();
}

void CompilationEngine::compileParameterList_()
{
  outputFile_ << "<parameterList>" << '\n';

  auto pCurrentToken = tokenizer_.getCurrentToken();
  if (pCurrentToken->getValue<JackTokenType::SYMBOL>() == ')')
  {
    outputFile_ << "</parameterList>" << '\n';
    return;
  }

  // type
  outputFile_ << *pCurrentToken;

  // name
  outputFile_ << *advanceAndGetNextToken();

  // Check for comma and repeat the process.
  pCurrentToken = advanceAndGetNextToken();
  while (pCurrentToken->getValue<JackTokenType::SYMBOL>() == ',')
  {
    // ,
    outputFile_ << *pCurrentToken;

    // type
    outputFile_ << *advanceAndGetNextToken();

    // name
    outputFile_ << *advanceAndGetNextToken();
    pCurrentToken = advanceAndGetNextToken();
  }

  outputFile_ << "</parameterList>" << '\n';
}

void CompilationEngine::compileVarDec_()
{
  auto pCurrentToken = tokenizer_.getCurrentToken();

  while (pCurrentToken->getTokenType() == JackTokenType::KEYWORD)
  {
    if (pCurrentToken->getValue<JackTokenType::KEYWORD>() != KeywordType::VAR)
      break;

    outputFile_ << "<varDec>" << '\n';

    // Var
    outputFile_ << *pCurrentToken;

    // Type
    outputFile_ << *advanceAndGetNextToken();

    // Var name
    outputFile_ << *advanceAndGetNextToken();

    pCurrentToken = advanceAndGetNextToken();

    // Check if there are other variables by checking if the next token is a ','
    while (pCurrentToken->getValue<JackTokenType::SYMBOL>() == ',')
    {
      outputFile_ << *pCurrentToken;

      // Another var name
      outputFile_ << *advanceAndGetNextToken();

      pCurrentToken = advanceAndGetNextToken();
    }

    // ;
    outputFile_ << *pCurrentToken;
    outputFile_ << "</varDec>" << '\n';
    pCurrentToken = advanceAndGetNextToken();
  }
}

void CompilationEngine::compileReturn_()
{
  outputFile_ << "<returnStatement>" << '\n';

  // Return
  outputFile_ << *tokenizer_.getCurrentToken();

  if (advanceAndGetNextToken()->getTokenType() != JackTokenType::SYMBOL)
  {
    // Expression
    compileExpression_();
  }

  // ;
  outputFile_ << *tokenizer_.getCurrentToken();
  outputFile_ << "</returnStatement>" << '\n';
  tokenizer_.advance();
}

void CompilationEngine::handleKeywordInStatements_(const Token *pCurrentToken)
{
  switch (pCurrentToken->getValue<JackTokenType::KEYWORD>())
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
  auto pCurrentToken = tokenizer_.getCurrentToken();

  while (pCurrentToken->getTokenType() == JackTokenType::KEYWORD)
  {
    handleKeywordInStatements_(pCurrentToken);
    pCurrentToken = tokenizer_.getCurrentToken();
  }

  outputFile_ << "</statements>" << '\n';
}

void CompilationEngine::compileSubroutineCall_()
{
  // subroutine name or class name or variable name.
  outputFile_ << *tokenizer_.getCurrentToken();

  // We want to check if we are calling a method on class or variable. We have to check for the
  // '.' symbol.

  auto pCurrentToken = advanceAndGetNextToken();
  while (pCurrentToken->getValue<JackTokenType::SYMBOL>() == '.')
  {
    // The previous token was a class or variable name.
    // Subroutine name is the current token.

    // '.'
    outputFile_ << *pCurrentToken;

    // subroutine name
    outputFile_ << *advanceAndGetNextToken();

    // Set the next token which is '(' to pCurrentToken
    pCurrentToken = advanceAndGetNextToken();
  }

  // (
  outputFile_ << *pCurrentToken;

  tokenizer_.advance();
  compileExpressionList_();

  // )
  outputFile_ << *tokenizer_.getCurrentToken();

  tokenizer_.advance();
}

void CompilationEngine::compileDo_()
{
  outputFile_ << "<doStatement>" << '\n';

  // do
  outputFile_ << *tokenizer_.getCurrentToken();
  tokenizer_.advance();
  compileSubroutineCall_();

  // ;
  outputFile_ << *tokenizer_.getCurrentToken();
  outputFile_ << "</doStatement>" << '\n';

  tokenizer_.advance();
}

void CompilationEngine::compileLet_()
{
  outputFile_ << "<letStatement>" << '\n';

  // Let
  outputFile_ << *tokenizer_.getCurrentToken();

  // Var name
  outputFile_ << *advanceAndGetNextToken();

  // We want to check if we are addressing and array here.
  auto pNextToken = advanceAndGetNextToken();

  if ((pNextToken->getTokenType() == JackTokenType::SYMBOL) && 
       pNextToken->getValue<JackTokenType::SYMBOL>() == '[')
  {
    // [
    outputFile_ << *pNextToken;
    tokenizer_.advance();

    compileExpression_();

    // ]
    outputFile_ << *tokenizer_.getCurrentToken();
    pNextToken = advanceAndGetNextToken();
  }

  // =
  outputFile_ << *pNextToken;
  tokenizer_.advance();

  compileExpression_();

  // ;
  outputFile_ << *tokenizer_.getCurrentToken();

  tokenizer_.advance();
  outputFile_ << "</letStatement>" << '\n';
}

void CompilationEngine::compileWhile_()
{
  // The code is very similar to if statement. Take out some common functionality, in general 
  // compiling inside a parentheses and inside a body {} is common.
  outputFile_ << "<whileStatement>" << '\n';

  // while
  outputFile_ << *tokenizer_.getCurrentToken();

  // '('
  outputFile_ << *advanceAndGetNextToken();

  tokenizer_.advance();
  compileExpression_();

  // ')'
  outputFile_ << *tokenizer_.getCurrentToken();

  // '{'
  outputFile_ << *advanceAndGetNextToken();
  tokenizer_.advance();

  compileStatements_();

  // '}'
  outputFile_ << *tokenizer_.getCurrentToken();
  tokenizer_.advance();

  outputFile_ << "</whileStatement>" << '\n';
}

void CompilationEngine::compileIf_()
{
  outputFile_ << "<ifStatement>" << '\n';

  auto compileIfBody = [this]()
  {
    // '{'
    outputFile_ << *advanceAndGetNextToken();
    tokenizer_.advance();

    compileStatements_();

    // '}'
    outputFile_ << *tokenizer_.getCurrentToken();
    tokenizer_.advance();
  };

  // If
  outputFile_ << *tokenizer_.getCurrentToken();

  // '('
  outputFile_ << *advanceAndGetNextToken();

  tokenizer_.advance();
  compileExpression_();

  // ')'
  outputFile_ << *tokenizer_.getCurrentToken();

  compileIfBody();

  // Check if we have an else statement.
  if (tokenizer_.getCurrentToken()->getValue<JackTokenType::KEYWORD>() == KeywordType::ELSE)
  {
    // else
    outputFile_ << *tokenizer_.getCurrentToken();
    compileIfBody();
  }

  outputFile_ << "</ifStatement>" << '\n';
}

bool CompilationEngine::isOp_(const char symbol) const
{
  if (symbol == '+') return true;
  if (symbol == '-') return true;
  if (symbol == '*') return true;

  if (symbol == '/') return true;
  if (symbol == '&') return true;
  if (symbol == '|') return true;

  if (symbol == '<') return true;
  if (symbol == '>') return true;
  if (symbol == '=') return true;

  return false;
}

bool CompilationEngine::isUnaryOp_(const char symbol) const
{
  if (symbol == '-') return true;
  if (symbol == '~') return true;

  return false;
}

void CompilationEngine::compileExpression_()
{
  outputFile_ << "<expression>" << '\n';

  // Compile the current term.
  compileTerm_();

  // Check if we have an operator next, if yes we need to compile more terms.
  auto pCurrentToken = tokenizer_.getCurrentToken();
  while ((pCurrentToken->getTokenType() == JackTokenType::SYMBOL) &&
         (isOp_(pCurrentToken->getValue<JackTokenType::SYMBOL>())))
  {
    // Print the operator.
    outputFile_ << *pCurrentToken;
    tokenizer_.advance();
    compileTerm_();
    pCurrentToken = tokenizer_.getCurrentToken();
  }

  outputFile_ << "</expression>" << '\n';
}

void CompilationEngine::compileTerm_()
{
  outputFile_ << "<term>" << '\n';

  auto pCurrentToken = tokenizer_.getCurrentToken();
  outputFile_ << *pCurrentToken;

  // It can be '(' or a unary operator.
  if (pCurrentToken->getTokenType() == JackTokenType::SYMBOL)
  {
    const auto symbol = pCurrentToken->getValue<JackTokenType::SYMBOL>();
    tokenizer_.advance();

    if (symbol == '(')
    {
      compileExpression_();

      // ')'
      outputFile_ << *tokenizer_.getCurrentToken();
      tokenizer_.advance();
    }
    else if (isUnaryOp_(symbol))
      compileTerm_();
    else
      throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
  }
  else
  {
    pCurrentToken = advanceAndGetNextToken();

    // We first want to check if there is a '[' or '(' or '.'
    if (pCurrentToken->getTokenType() == JackTokenType::SYMBOL)
    {
      const auto symbol = pCurrentToken->getValue<JackTokenType::SYMBOL>();

      if ((symbol == '(') || (symbol == '[') || (symbol == '.'))
      {
        // Print the symbol.
        outputFile_ << *pCurrentToken;
        tokenizer_.advance();

        // Array or call.
        if ((symbol == '[') || (symbol == '('))
        {
          compileExpression_();

          // ']' or ')'
          outputFile_ << *tokenizer_.getCurrentToken();
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
      outputFile_ << *pCurrentToken;
      tokenizer_.advance();
    }
  }

  //switch (pCurrentToken->getTokenType())
  //{
  //case JackTokenType::SYMBOL:
  //{
  //  //const auto symbol = pCurrentToken->getValue<JackTokenType::SYMBOL>();
  //  //if (!isOp_(symbol) || !isUnaryOp_(symbol))
  //  //  throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
  //  tokenizer_.advance();
  //  // The previous token was an operator, so we need to compile the next term as well
  //  compileTerm_();

  //  break;
  //}
  //default:
  //{
  //  // The previous token was not a symbol.
  //  pCurrentToken = advanceAndGetNextToken();

  //  // We first want to check if there is a '[' or '(' or '.'
  //  if (pCurrentToken->getTokenType() == JackTokenType::SYMBOL)
  //  {
  //    const auto symbol = pCurrentToken->getValue<JackTokenType::SYMBOL>();

  //    if ((symbol == '(') || (symbol == '[') || (symbol == '.'))
  //    {
  //      // Print the symbol.
  //      outputFile_ << *pCurrentToken;
  //      tokenizer_.advance();

  //      // Array or call.
  //      if ((symbol == '[') || (symbol == '('))
  //      {
  //        compileExpression_();

  //        // ']' or ')'
  //        outputFile_ << *tokenizer_.getCurrentToken();
  //        tokenizer_.advance();
  //      }

  //      // Subroutine call.
  //      else if (symbol == '.')
  //        compileSubroutineCall_();
  //      else
  //        throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" after term");
  //    }
  //  }
  //  break;
  //}
  //}

  outputFile_ << "</term>" << '\n';
}

void CompilationEngine::compileExpressionList_()
{
  outputFile_ << "<expressionList>" << '\n';

  // In case there are no expressions
  if (!(tokenizer_.getCurrentToken()->getValue<JackTokenType::SYMBOL>() == ')'))
  {
    compileExpression_();

    auto pCurrentToken = tokenizer_.getCurrentToken();
    while (pCurrentToken->getValue<JackTokenType::SYMBOL>() == ',')
    {
      // ','
      outputFile_ << *pCurrentToken;
      tokenizer_.advance();
      compileExpression_();
      pCurrentToken = tokenizer_.getCurrentToken();
    }
  }

  outputFile_ << "</expressionList>" << '\n';
}