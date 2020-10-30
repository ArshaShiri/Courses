#include "CompilationEngine.h"
#include "Token.h"

// General rule, each compile method first prints the current token and at the end
// advances the token. (some exceptions... explain well)

CompilationEngine::CompilationEngine(
  const std::string &otputFilepath, JackTokenizer &tokenizer) : tokenizer_{tokenizer}
{
  outputFile_.open(otputFilepath + ".xml");

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

  // The current 3 tokens are class followed by { and an identifier
  outputFile_ << *tokenizer_.getCurrentToken();
  outputFile_ << *advanceAndGetNextToken();
  outputFile_ << *advanceAndGetNextToken();

  // We might have  0 to many variable declaration of this class
  while (tokenizer_.hasMoreTokens())
  {
    const auto pNextToken = advanceAndGetNextToken();
    if (pNextToken->isTerminal())
    {
      const auto &keyword = static_cast<const Keyword&>(*pNextToken);
      switch (keyword.getKeywordType())
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
    }
    else
      break;
  }

  // Next token is {
  outputFile_ << *advanceAndGetNextToken();

  outputFile_ << "</class>" << '\n';
}

void CompilationEngine::compileClassVarDec_()
{
  outputFile_ << "<classVarDec>" << '\n';

  // Either static or field.
  outputFile_ << *tokenizer_.getCurrentToken();

  // Type
  outputFile_ << *advanceAndGetNextToken();

  // Var name
  outputFile_ << *advanceAndGetNextToken();

  auto pNextToken = advanceAndGetNextToken();

  while (pNextToken->getName() == ",")
  {
    outputFile_ << *pNextToken;

    // Another var name
    outputFile_ << *advanceAndGetNextToken();

    pNextToken = advanceAndGetNextToken();
  }

  // ;
  outputFile_ << *pNextToken;

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
  outputFile_ << *advanceAndGetNextToken();

  outputFile_ << "</subroutineBody>" << '\n';
  outputFile_ << "</subroutineDec>" << '\n';
}

void CompilationEngine::compileParameterList_()
{
  outputFile_ << "<parameterList>" << '\n';
  outputFile_ << "</parameterList>" << '\n';
}

void CompilationEngine::compileVarDec_()
{
  outputFile_ << "<varDec>" << '\n';

  auto pCurrentToken = tokenizer_.getCurrentToken();

  while (pCurrentToken->isTerminal())
  {
    const auto pKeyword = static_cast<const Keyword*>(pCurrentToken);

    if (pKeyword->getKeywordType() != KeywordType::VAR)
      break;

    // Var
    outputFile_ << *pKeyword;

    // Type
    outputFile_ << *advanceAndGetNextToken();

    // Var name
    outputFile_ << *advanceAndGetNextToken();

    pCurrentToken = advanceAndGetNextToken();

    while (pCurrentToken->getName() == ",")
    {
      outputFile_ << *pCurrentToken;

      // Another var name
      outputFile_ << *advanceAndGetNextToken();

      pCurrentToken = advanceAndGetNextToken();
    }

    // ;
    outputFile_ << *pCurrentToken;

    pCurrentToken = advanceAndGetNextToken();
  }

  outputFile_ << "</varDec>" << '\n';
}

void CompilationEngine::compileStatements_()
{
  outputFile_ << "<statements>" << '\n';
  auto pCurrentToken = tokenizer_.getCurrentToken();

  while (pCurrentToken->isTerminal())
  {
    const auto pKeyword = static_cast<const Keyword*>(pCurrentToken);

    switch (pKeyword->getKeywordType())
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
      // Return
      outputFile_ << *tokenizer_.getCurrentToken();

      auto pNexttoken = advanceAndGetNextToken();
      if (pNexttoken->getName() != ";")
      {
        // Expression
        outputFile_ << *pNexttoken;
      }

      // ;
      outputFile_ << *advanceAndGetNextToken();
      break;
    }
    default:
      throw std::runtime_error("Wrong expression in compileStatements_!");
      break;
    }

    pCurrentToken = advanceAndGetNextToken();
  }

  outputFile_ << "</statements>" << '\n';
}

void CompilationEngine::compileDo_()
{}

void CompilationEngine::compileLet_()
{
  outputFile_ << "<letStatement>" << '\n';

  // Let
  outputFile_ << *tokenizer_.getCurrentToken();

  // Var name
  outputFile_ << *advanceAndGetNextToken();

  // We want to check if we are addressing and array here.
  auto pNextToken = advanceAndGetNextToken();

  if (pNextToken->getName() == "[")
  {
    // [
    outputFile_ << *pNextToken;

    compileExpression_();

    // ]
    outputFile_ << *tokenizer_.getCurrentToken();
    pNextToken = advanceAndGetNextToken();
  }

  // =
  outputFile_ << *pNextToken;

  compileExpression_();

  // ;
  outputFile_ << *advanceAndGetNextToken();

  tokenizer_.advance();
  outputFile_ << "</letStatement>" << '\n';
}

void CompilationEngine::compileWhile_()
{}

void CompilationEngine::compileIf_()
{}

void CompilationEngine::compileExpression_()
{
  outputFile_ << "<expression>" << '\n';
  

  outputFile_ << "</expression>" << '\n';
}

void CompilationEngine::compileTerm_()
{}

void CompilationEngine::compileExpressionList_()
{}