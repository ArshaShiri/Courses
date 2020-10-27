#include "CompilationEngine.h"
#include "Token.h"


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

void CompilationEngine::compileClass_()
{
  // The current 3 tokens are class followed by { and an identifier
  outputFile_ << *tokenizer_.getCurrentToken();
  outputFile_ << *advanceAndGetNextToken();
  outputFile_ << *advanceAndGetNextToken();
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