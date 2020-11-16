#include "CompilationEngine.h"
#include "Helper.h"
#include "OSFunctionWriterHelper.h"
#include "Token.h"

CompilationEngine::CompilationEngine(const std::string &otputFilepath, JackTokenizer &tokenizer) : 
  tokenizer_{tokenizer}, VMWriter_{otputFilepath}
{
  if (tokenizer.hasMoreTokens())
  {
    tokenizer.advance();
    compileClass_();
  }

  VMWriter_.close();
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
  // The current token is "class". The following two tokens are an identifier(class name) and '{'.
  // We store the class name as it is useful to generate names for static methods as they contain
  // the name of the class in them. Also class name is useful in method symbol tables too. We can
  // skip the '{' symbol.
  className_ = advanceAndGetNextToken().getName();
  tokenizer_.advance();

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
}

void CompilationEngine::addToSymbolTable_(const std::string &identifierName,
                                          const std::string &identifierType,
                                          KeywordType keywordType)
{
  switch (keywordType)
  {
  case KeywordType::STATIC: 
    symbolTable_.define(identifierName, identifierType, IdentifierKind::STATIC); break;

  case KeywordType::FIELD:
    symbolTable_.define(identifierName, identifierType, IdentifierKind::FIELD); break;

  case KeywordType::VAR:
    symbolTable_.define(identifierName, identifierType, IdentifierKind::VAR); break;
  
  // There is no specific keyword for arguments, so we have to add that manually to the symbol table
  // when we are parsing parameter lists.
  default:
    throw std::runtime_error("Cannot add this keyword type to the symbol table! " __FUNCTION__);
  }
}

void CompilationEngine::compileClassVarDec_()
{
  // We are not outputting anything. Just update the symbol table.

  // Either "static" or "field".
  const auto keywordType = tokenizer_.getCurrentToken().getKeyWordType();
  const auto symbolType = advanceAndGetNextToken().getName();
  auto symbolName = advanceAndGetNextToken().getName();

  addToSymbolTable_(symbolName, symbolType, keywordType);

  auto nextToken = advanceAndGetNextToken();

  // check if there are other variables by checking if the next token is a ','
  while (nextToken.getSymbol() == ',')
  {
    // Another var name
    symbolName = advanceAndGetNextToken().getName();
    addToSymbolTable_(symbolName, symbolType, keywordType);
    nextToken = advanceAndGetNextToken();
  }
}

void CompilationEngine::compileClassSubroutine_()
{
  // We are in a scope of a new subroutine. Clear the symbol table for the subroutine before adding
  // new entries.
  symbolTable_.clearSubroutineSymbolTable();
  const auto currentToken = tokenizer_.getCurrentToken();
  auto subRoutineName = std::string{};
  switch (currentToken.getKeyWordType())
  {
  case KeywordType::CONSTRUCTOR:
  {
    // We first need to allocate the needed amount of memory. To do so, we have to count the number
    // of field variables and push it to the stack and call alloc with it.
    const auto filedVarCount = symbolTable_.varCount(IdentifierKind::FIELD);
    VMWriter_.writePush(MemorySegment::CONST, filedVarCount);
    VMWriter_.writeCall(OSMemoryWriterHelper::allocName(), OSMemoryWriterHelper::allocnumberOfArgs());
    break;
  }
  case KeywordType::FUNCTION:
  {
    // In the generate vm code, a function name is translated to the class name followed by a dot
    // and the function name.
    subRoutineName += className_ + ".";
  }
  default:
    break;
  }

  // skip the token that specifies constructor, function or method.
  tokenizer_.advance();

  // skip the token that specifies the return type.
  tokenizer_.advance();

  // Store the function's name.
  subRoutineName += tokenizer_.getCurrentToken().getName();

  // skip '('
  tokenizer_.advance();

  tokenizer_.advance();
  compileParameterList_();

  // After compiling the parameter list, the symbol table should have all the arguments stored and
  // and we can write the vm command for this subroutine.
  VMWriter_.writeFunction(subRoutineName, symbolTable_.varCount(IdentifierKind::VAR));

  // skip ')'
  tokenizer_.advance();

  // <subroutineBody>

  // skip '{'
  tokenizer_.advance();
  compileVarDec_();
  compileStatements_();

  // skip '}'
  tokenizer_.advance();
  // </subroutineBody>
}

void CompilationEngine::compileParameterList_()
{
  auto currentToken = tokenizer_.getCurrentToken();

  // No parameters are available.
  if (currentToken.getSymbol() == ')')
    return;
  
  // type: int, char, or any other object type.
  auto symbolType = currentToken.getName();

  // name
  auto symbolName = advanceAndGetNextToken().getName();
  symbolTable_.define(symbolName, symbolType, IdentifierKind::ARG);

  // Check for comma and repeat the process.
  currentToken = advanceAndGetNextToken();
  while (currentToken.getSymbol() == ',')
  {
    // skip over ',' as we do not need it.
    tokenizer_.advance();

    symbolType = advanceAndGetNextToken().getName();
    symbolName = advanceAndGetNextToken().getName();
    symbolTable_.define(symbolName, symbolType, IdentifierKind::ARG);
    currentToken = advanceAndGetNextToken();
  }
}

void CompilationEngine::compileVarDec_()
{
  // Only updating the symbol table.
  auto currentToken = tokenizer_.getCurrentToken();

  while (currentToken.getTokenType() == JackTokenType::KEYWORD)
  {
    if (currentToken.getKeyWordType() != KeywordType::VAR)
      break;

    // skip the keyword var
    tokenizer_.advance();

    // Type
    const auto varType = advanceAndGetNextToken().getName();

    // Var name
    auto varName = advanceAndGetNextToken().getName();

    addToSymbolTable_(varName, varType, KeywordType::VAR);

    currentToken = advanceAndGetNextToken();

    // Check if there are other variables by checking if the next token is a ','
    while (currentToken.getSymbol() == ',')
    {
      // Skip ','
      tokenizer_.advance();

      // Another var name
      varName = advanceAndGetNextToken().getName();
      addToSymbolTable_(varName, varType, KeywordType::VAR);
      currentToken = advanceAndGetNextToken();
    }

    // skip ';'
    tokenizer_.advance();
    currentToken = advanceAndGetNextToken();
  }
}

void CompilationEngine::compileReturn_()
{
  // Check if the next token is ';' if yes it means it was: return; meaning the subroutine was void
  auto isSubroutineVoid = advanceAndGetNextToken().getTokenType() == JackTokenType::SYMBOL;
  if (isSubroutineVoid)
    VMWriter_.writePush(MemorySegment::CONST, 0);

  // Return
  VMWriter_.writeReturn();

  if (!isSubroutineVoid)
  {
    // Expression
    compileExpression_();
  }

  // skip ';'
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
  auto currentToken = tokenizer_.getCurrentToken();

  while (currentToken.getTokenType() == JackTokenType::KEYWORD)
  {
    handleKeywordInStatements_(currentToken);
    currentToken = tokenizer_.getCurrentToken();
  }
}

void CompilationEngine::compileSubroutineCall_()
{
  // identifier, can be var name, class name or subroutine name.
  auto identifier = tokenizer_.getCurrentToken().getName();

  auto currentToken = advanceAndGetNextToken();
  auto subroutineName = std::string{};
  subroutineName += identifier;

  // We want to check if we are calling a method on class or variable. We have to check for the
  // '.' symbol.
  if (currentToken.getSymbol() == '.')
  {
    // The previous token was a class or variable name.
    // Subroutine name is the current token.

    // skip '.'
    tokenizer_.advance();

    
    if (!symbolTable_.doesIdentifierExist(identifier))
    {
      // When an identifier does not exist, it means that the method is a static method of the 
      // current class or another class.
      subroutineName += ".";
    }
    subroutineName += tokenizer_.getCurrentToken().getName();
    tokenizer_.advance();
  }

  // skip '('
  tokenizer_.advance();
  auto numberOfExpressions = compileExpressionList_();
  VMWriter_.writeCall(subroutineName, numberOfExpressions);

  // skip ')'
  tokenizer_.advance();
}

void CompilationEngine::compileDo_()
{
  // This means we are calling a void method. Otherwise we are not going to be in do.

  // skip "do"
  tokenizer_.advance();
  compileSubroutineCall_();

  // skip ';'
  tokenizer_.advance();
  VMWriter_.writePop(MemorySegment::TEMPT, 0);
}

void CompilationEngine::compileLet_()
{
  //outputFile_ << "<letStatement>" << '\n';

  //// Let
  //outputFile_ << tokenizer_.getCurrentToken();

  //// Var name
  //outputFile_ << advanceAndGetNextToken();

  //// We want to check if we are addressing and array here.
  //auto nextToken = advanceAndGetNextToken();

  //if ((nextToken.getTokenType() == JackTokenType::SYMBOL) &&
  //     nextToken.getSymbol() == '[')
  //{
  //  // [
  //  outputFile_ << nextToken;
  //  tokenizer_.advance();

  //  compileExpression_();

  //  // ]
  //  outputFile_ << tokenizer_.getCurrentToken();
  //  nextToken = advanceAndGetNextToken();
  //}

  //// =
  //outputFile_ << nextToken;
  //tokenizer_.advance();

  //compileExpression_();

  //// ;
  //outputFile_ << tokenizer_.getCurrentToken();

  //tokenizer_.advance();
  //outputFile_ << "</letStatement>" << '\n';
}

void CompilationEngine::compileWhile_()
{
  //// The code is very similar to if statement. Take out some common functionality, in general 
  //// compiling inside a parentheses and inside a body {} is common.
  //outputFile_ << "<whileStatement>" << '\n';

  //// while
  //outputFile_ << tokenizer_.getCurrentToken();

  //// '('
  //outputFile_ << advanceAndGetNextToken();

  //tokenizer_.advance();
  //compileExpression_();

  //// ')'
  //outputFile_ << tokenizer_.getCurrentToken();

  //// '{'
  //outputFile_ << advanceAndGetNextToken();
  //tokenizer_.advance();

  //compileStatements_();

  //// '}'
  //outputFile_ << tokenizer_.getCurrentToken();
  //tokenizer_.advance();

  //outputFile_ << "</whileStatement>" << '\n';
}

void CompilationEngine::compileIf_()
{
  //outputFile_ << "<ifStatement>" << '\n';

  //auto compileIfBody = [this]()
  //{
  //  // '{'
  //  outputFile_ << advanceAndGetNextToken();
  //  tokenizer_.advance();

  //  compileStatements_();

  //  // '}'
  //  outputFile_ << tokenizer_.getCurrentToken();
  //  tokenizer_.advance();
  //};

  //// If
  //outputFile_ << tokenizer_.getCurrentToken();

  //// '('
  //outputFile_ << advanceAndGetNextToken();

  //tokenizer_.advance();
  //compileExpression_();

  //// ')'
  //outputFile_ << tokenizer_.getCurrentToken();

  //compileIfBody();

  //// Check if we have an else statement.
  //if (tokenizer_.getCurrentToken().getKeyWordType() == KeywordType::ELSE)
  //{
  //  // else
  //  outputFile_ << tokenizer_.getCurrentToken();
  //  compileIfBody();
  //}

  //outputFile_ << "</ifStatement>" << '\n';
}

void CompilationEngine::handleOperator(const char character)
{
  if (character == '+')
  {
    VMWriter_.writeArithmetic(ArithmeticCommand::ADD);
    return;
  }

  if (character == '-')
  {
    VMWriter_.writeArithmetic(ArithmeticCommand::SUB);
    return;
  }

  if (character == '*')
  {
    // Multiplication is handled by the operations system.
    VMWriter_.writeCall(OSMathWriterHelper::multName(), OSMathWriterHelper::multnumberOfArgs());
    return;
  }

  if (character == '/')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  if (character == '&')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  if (character == '|')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  if (character == '<')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  if (character == '>')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  if (character == '=')
  {
    throw std::runtime_error("Arithmetic operator is not implemented.");
    return;
  }

  throw std::runtime_error("Operator " + std::string{character} +"is not supported." __FUNCTION__);
}

void CompilationEngine::compileExpression_()
{
  // Compile the current term.
  compileTerm_();

  // Check if we have an operator next, if yes we need to compile more terms.
  auto currentToken = tokenizer_.getCurrentToken();
  while ((currentToken.getTokenType() == JackTokenType::SYMBOL) &&
         (isOp(currentToken.getSymbol())))
  {
    tokenizer_.advance();
    compileTerm_();

    // Apply operator
    handleOperator(currentToken.getSymbol());
    currentToken = tokenizer_.getCurrentToken();
  }
}

void CompilationEngine::compileTerm_()
{
  const auto currentToken = tokenizer_.getCurrentToken();
  switch (currentToken.getTokenType())
  {
  case JackTokenType::SYMBOL:
  {
    // It can be '(' or a unary operator.
    const auto symbol = currentToken.getSymbol();
    tokenizer_.advance();

    if (symbol == '(')
    {
      compileExpression_();

      // skip ')'
      tokenizer_.advance();
    }
    else if (isUnaryOp(symbol))
      compileTerm_();
    else
      throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
    break;
  }
  case JackTokenType::INTEGERCONSTATNT:
  {
    VMWriter_.writePush(MemorySegment::CONST, std::stoi(tokenizer_.getCurrentToken().getName()));
    tokenizer_.advance();
    break;
  }
  case JackTokenType::STRINGCONSTANT:
    throw std::runtime_error("STRINGCONSTANT is not implemented yet in " __FUNCTION__);
  case JackTokenType::IDENTIFIER:
    throw std::runtime_error("IDENTIFIER is not implemented yet in " __FUNCTION__);
  default:
    throw std::runtime_error("This token should not be in " __FUNCTION__);
  }

  //auto currentToken = tokenizer_.getCurrentToken();
  //outputFile_ << currentToken;

  //if (currentToken.getTokenType() == JackTokenType::SYMBOL)
  //{
  //  const auto symbol = currentToken.getSymbol();
  //  tokenizer_.advance();

  //  if (symbol == '(')
  //  {
  //    compileExpression_();

  //    // ')'
  //    outputFile_ << tokenizer_.getCurrentToken();
  //    tokenizer_.advance();
  //  }
  //  else if (isUnaryOp(symbol))
  //    compileTerm_();
  //  else
  //    throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
  //}
  //else
  //{
  //  currentToken = advanceAndGetNextToken();

  //  // We first want to check if there is a '[' or '(' or '.'
  //  if (currentToken.getTokenType() == JackTokenType::SYMBOL)
  //  {
  //    const auto symbol = currentToken.getSymbol();

  //    if ((symbol == '(') || (symbol == '[') || (symbol == '.'))
  //    {
  //      // Print the symbol.
  //      outputFile_ << currentToken;
  //      tokenizer_.advance();

  //      // Array or call.
  //      if ((symbol == '[') || (symbol == '('))
  //      {
  //        compileExpression_();

  //        // ']' or ')'
  //        outputFile_ << tokenizer_.getCurrentToken();
  //        tokenizer_.advance();
  //      }

  //      // Subroutine call.
  //      else if (symbol == '.')
  //        compileSubroutineCall_();
  //      else
  //        throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" after term");
  //    }
  //  }
  //  else
  //  {
  //    outputFile_ << currentToken;
  //    tokenizer_.advance();
  //  }
  //}
}

size_t CompilationEngine::compileExpressionList_()
{
  auto numberOfExpressions = 0;
  // In case there are no expressions
  if (!(tokenizer_.getCurrentToken().getSymbol() == ')'))
  {
    numberOfExpressions++;
    compileExpression_();

    auto currentToken = tokenizer_.getCurrentToken();
    while (currentToken.getSymbol() == ',')
    {
      // skip ','
      tokenizer_.advance();
      
      // Go to the next token.
      tokenizer_.advance();
      compileExpression_();
      currentToken = tokenizer_.getCurrentToken();
    }
  }

  return numberOfExpressions;
}