#include <functional>

#include "CompilationEngine.h"
#include "Helper.h"
#include "OSFunctionWriterHelper.h"
#include "Token.h"

CompilationEngine::CompilationEngine(const std::string &otputFilepath, JackTokenizer &tokenizer) : 
  tokenizer_{tokenizer}, VMWriter_{otputFilepath}, whileCounter_{0}, ifCounter_{0}
{
  if (tokenizer.hasMoreTokens())
  {
    tokenizer.advance();
    compileClass_();
  }

  VMWriter_.close();
}

const Token CompilationEngine::advanceAndGetNextToken_()
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
  className_ = advanceAndGetNextToken_().getName();
  tokenizer_.advance();

  // We might have  0 to many variable declaration of this class
  auto nextToken = advanceAndGetNextToken_();
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
  const auto symbolType = advanceAndGetNextToken_().getName();
  auto symbolName = advanceAndGetNextToken_().getName();

  addToSymbolTable_(symbolName, symbolType, keywordType);

  auto nextToken = advanceAndGetNextToken_();

  // check if there are other variables by checking if the next token is a ','
  while (nextToken.getSymbol() == ',')
  {
    // Another var name
    symbolName = advanceAndGetNextToken_().getName();
    addToSymbolTable_(symbolName, symbolType, keywordType);
    nextToken = advanceAndGetNextToken_();
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

  // Go to the next token.
  tokenizer_.advance();

  // skip '('
  tokenizer_.advance();
  compileParameterList_();

  // skip ')'
  tokenizer_.advance();

  // <subroutineBody>

  // skip '{'
  tokenizer_.advance();
  compileVarDec_();

  // After compiling the variable declaration, the symbol table should have all the arguments stored and
  // and we can write the vm command for this subroutine.
  VMWriter_.writeFunction(subRoutineName, symbolTable_.varCount(IdentifierKind::VAR));

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
  auto symbolName = advanceAndGetNextToken_().getName();
  symbolTable_.define(symbolName, symbolType, IdentifierKind::ARG);

  // Check for comma and repeat the process.
  currentToken = advanceAndGetNextToken_();
  while (currentToken.getSymbol() == ',')
  {
    // skip over ',' and update the type. And define the next symbol.
    symbolType = advanceAndGetNextToken_().getName();
    symbolName = advanceAndGetNextToken_().getName();
    symbolTable_.define(symbolName, symbolType, IdentifierKind::ARG);
    currentToken = advanceAndGetNextToken_();
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

    // skip the keyword var and get the type
    const auto varType = advanceAndGetNextToken_().getName();

    // Var name
    auto varName = advanceAndGetNextToken_().getName();

    addToSymbolTable_(varName, varType, KeywordType::VAR);

    currentToken = advanceAndGetNextToken_();

    // Check if there are other variables by checking if the next token is a ','
    while (currentToken.getSymbol() == ',')
    {
      // Skip ',' and get another var name
      varName = advanceAndGetNextToken_().getName();
      addToSymbolTable_(varName, varType, KeywordType::VAR);
      currentToken = advanceAndGetNextToken_();
    }

    // skip ';' and get the next token
    currentToken = advanceAndGetNextToken_();
  }
}

void CompilationEngine::compileReturn_()
{
  // Check if the next token is ';' if yes it means it was: return; meaning the subroutine was void
  auto isSubroutineVoid = advanceAndGetNextToken_().getTokenType() == JackTokenType::SYMBOL;
  if (isSubroutineVoid)
    VMWriter_.writePush(MemorySegment::CONST, 0);
  else
    compileExpression_();

  // Return
  VMWriter_.writeReturn();

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

void CompilationEngine::compileCallOnObject_(std::string classOrVarName)
{
  const auto currentToken = tokenizer_.getCurrentToken();

  // Remove the symbol '.' from the name to get the identifier name.
  const auto identifier = classOrVarName.substr(0, classOrVarName.size() - 1);
  classOrVarName += tokenizer_.getCurrentToken().getName();
  tokenizer_.advance();

  // skip '('
  tokenizer_.advance();
  auto numberOfExpressions = compileExpressionList_();
  VMWriter_.writeCall(classOrVarName, numberOfExpressions);

  // skip ')'
  tokenizer_.advance();
}

void CompilationEngine::compileSubroutineCall_()
{
  // identifier, can be var name, class name or subroutine name.
  auto identifier = tokenizer_.getCurrentToken().getName();

  const auto currentToken = advanceAndGetNextToken_();
  auto classOrVarName = std::string{};
  classOrVarName += identifier;

  if (currentToken.getSymbol() != '.')
    throw std::runtime_error(
      "Var or class should be followed by a '.' to call a subroutine. " __FUNCTION__);

  classOrVarName += currentToken.getSymbol();

  // skip '.'
  tokenizer_.advance();

  compileCallOnObject_(classOrVarName);
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
  // skip Let and get the var name. The var name is used later to pop the rhs result into it.
  const auto varName = advanceAndGetNextToken_().getName();

  // We want to check if we are addressing and array here.
  auto nextToken = advanceAndGetNextToken_();

  if ((nextToken.getTokenType() == JackTokenType::SYMBOL) &&
       nextToken.getSymbol() == '[')
  {
    // skip '['
    tokenizer_.advance();

    compileExpression_();

    // skip ']' and get the next token
    nextToken = advanceAndGetNextToken_();
  }

  // skip '='
  tokenizer_.advance();

  compileExpression_();

  // After compiling the expression, the result of the rhs is on the stack. We have to pop it to the
  // variable.
  writePushPop_(varName, symbolTable_.getIndexOf(varName), false);
  //VMWriter_.writePop(MemorySegment::LOCAL, symbolTable_.getIndexOf(varName));f

  // skip ';'
  tokenizer_.advance();
}

void CompilationEngine::compileWhile_()
{
  const auto whileExpressionStartLable = "WHILE_EXP" + std::to_string(whileCounter_);
  const auto whileExpressionEndLable = "WHILE_END" + std::to_string(whileCounter_);
  ++whileCounter_;

  // skip while
  tokenizer_.advance();

  // skip '('
  tokenizer_.advance();

  VMWriter_.writeLabel(whileExpressionStartLable);
  compileExpression_();
  VMWriter_.writeArithmetic(ArithmeticCommand::NOT);
  VMWriter_.writeIf(whileExpressionEndLable);

  // skip ')'
  tokenizer_.advance();

  // skip '{'
  tokenizer_.advance();

  compileStatements_();

  // skip '}'
  tokenizer_.advance();
  VMWriter_.writeGoTo(whileExpressionStartLable);
  VMWriter_.writeLabel(whileExpressionEndLable);
}

void CompilationEngine::compileIf_()
{
  auto compileIfBody = [this]()
  {
    // skip '{'
    tokenizer_.advance();

    compileStatements_();

    // skip '}'
    tokenizer_.advance();
  };

  // Label names for if when it is true and when it is false.
  const auto ifTrue = "IF_TRUE" + std::to_string(ifCounter_);
  const auto ifFalse = "IF_FALSE" + std::to_string(ifCounter_);
  const auto ifEnd = "IF_END" + std::to_string(ifCounter_);
  ++ifCounter_;

  // skip If
  tokenizer_.advance();

  // skip '('
  tokenizer_.advance();

  // Write appropriate goto commands to control the flow of the program based on the condition of 
  // the if statement.
  compileExpression_();
  VMWriter_.writeIf(ifTrue);
  VMWriter_.writeGoTo(ifFalse);
  VMWriter_.writeLabel(ifTrue);

  // skip ')'
  tokenizer_.advance();

  compileIfBody();

  // When the body is finished, goto the end of the if.
  VMWriter_.writeGoTo(ifEnd);

  // Check if we have an else statement.
  if (tokenizer_.getCurrentToken().getKeyWordType() == KeywordType::ELSE)
  {
    // skip else
    tokenizer_.advance();
    VMWriter_.writeLabel(ifFalse);
    compileIfBody();
  }

  VMWriter_.writeLabel(ifEnd);
}

void CompilationEngine::handleOperator_(const char character)
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
    VMWriter_.writeArithmetic(ArithmeticCommand::AND);
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
    VMWriter_.writeArithmetic(ArithmeticCommand::GT);
    return;
  }

  if (character == '=')
  {
    VMWriter_.writeArithmetic(ArithmeticCommand::EQ);
    return;
  }

  throw std::runtime_error("Operator " + std::string{character} +"is not supported." __FUNCTION__);
}

void CompilationEngine::handleUnaryOperator_(const char character)
{
  if (character == '-')
  {
    VMWriter_.writeArithmetic(ArithmeticCommand::NEG);
    return;
  }

  if (character == '~')
  {
    VMWriter_.writeArithmetic(ArithmeticCommand::NOT);
    return;
  }

  throw std::runtime_error("Unary operator " + std::string{character} +"is not supported." __FUNCTION__);
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
    handleOperator_(currentToken.getSymbol());
    currentToken = tokenizer_.getCurrentToken();
  }
}

void CompilationEngine::compileSymbolTerm_()
{
  // It can be '(' or a unary operator.
  const auto symbol = tokenizer_.getCurrentToken().getSymbol();
  tokenizer_.advance();

  if (symbol == '(')
  {
    compileExpression_();

    // skip ')'
    tokenizer_.advance();
  }
  else if (isUnaryOp(symbol))
  {
    // The unary operator has to be printed after the term itself because of the postfix summation.
    compileTerm_();
    handleUnaryOperator_(symbol);
  }
  else
    throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" before term");
}

void CompilationEngine::compileIntegerTerm_()
{
  VMWriter_.writePush(MemorySegment::CONST, std::stoi(tokenizer_.getCurrentToken().getName()));
  tokenizer_.advance();
}


void CompilationEngine::writePushPop_(const std::string &identifierName, int varIndex, bool isPush)
{
  MemorySegment memorySegment;

  switch (symbolTable_.getKindOf(identifierName))
  {
  case IdentifierKind::ARG: memorySegment = MemorySegment::ARG; break; //VMWriter_.writePush(MemorySegment::ARG, varIndex); break;
  case IdentifierKind::VAR: memorySegment = MemorySegment::LOCAL; break; //VMWriter_.writePush(MemorySegment::LOCAL, varIndex); break;
  default: throw std::runtime_error("Not supported symbol type " __FUNCTION__);
  }

  if (isPush)
    VMWriter_.writePush(memorySegment, varIndex);
  else
    VMWriter_.writePop(memorySegment, varIndex);
}

void CompilationEngine::compileIdentifierTerm_()
{
  auto isSymbolSpecial = [](const Token &token)
  {
    // A special symbol is a '[' or '(' or '.'. This means there are more terms involved, because
    // we are calling a subroutine or there is an array involved etc.
    auto isSymbol = token.getTokenType() == JackTokenType::SYMBOL;

    if (!isSymbol)
      return false;
    else
    {
      const auto symbol = token.getSymbol();
      return (symbol == '(') || (symbol == '[') || (symbol == '.');
    }
  };

  const auto identifierName = tokenizer_.getCurrentToken().getName();
  const auto currentToken = advanceAndGetNextToken_();

  // We first want to check if there is a '[' or '(' or '.'
  if (isSymbolSpecial(currentToken))
  {
    const auto symbol = currentToken.getSymbol();

    // Skip the symbol.
    tokenizer_.advance();

    // Array or call.
    if ((symbol == '[') || (symbol == '('))
    {
      compileExpression_();

      // skip ']' or ')'
      tokenizer_.advance();
    }

    // Subroutine call.
    else if (symbol == '.')
    {
      compileCallOnObject_(identifierName + std::string{symbol});
    }
    else
      throw std::runtime_error("Unsupported symbol " + std::string{symbol} +" after term");
  }
  else
    writePushPop_(identifierName, symbolTable_.getIndexOf(identifierName), true);
}

void CompilationEngine::compileKeywordTerm_()
{
  switch (tokenizer_.getCurrentToken().getKeyWordType())
  {
  case KeywordType::TRUE_:
  {
    // push zero and apply not to it. !0
    VMWriter_.writePush(MemorySegment::CONST, 0);
    VMWriter_.writeArithmetic(ArithmeticCommand::NOT);
    break;
  }
  case KeywordType::FASLE: VMWriter_.writePush(MemorySegment::CONST, 0); break;
  default:
    throw std::runtime_error("Keyword is not supported in " __FUNCTION__);
  }

  tokenizer_.advance();
}

void CompilationEngine::compileTerm_()
{
  switch (tokenizer_.getCurrentToken().getTokenType())
  {
  case JackTokenType::SYMBOL: compileSymbolTerm_(); break;
  case JackTokenType::INTEGERCONSTATNT: compileIntegerTerm_(); break;
  case JackTokenType::STRINGCONSTANT:
    throw std::runtime_error("STRINGCONSTANT is not implemented yet in " __FUNCTION__);
  case JackTokenType::IDENTIFIER: compileIdentifierTerm_(); break;
  case JackTokenType::KEYWORD: compileKeywordTerm_(); break;
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
      numberOfExpressions++;
      
      // Go to the next token.
      compileExpression_();
      currentToken = tokenizer_.getCurrentToken();
    }
  }

  return numberOfExpressions;
}