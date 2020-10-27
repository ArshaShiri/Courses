#pragma once

#include <fstream>
#include <string>

#include "JackTokenizer.h"

class Token;

class CompilationEngine
{
public:
  CompilationEngine(const std::string &otputFilepath, JackTokenizer &tokenizer);

private:
  void compileClass_();

  const Token* advanceAndGetNextToken();

  JackTokenizer &tokenizer_;
  std::ofstream outputFile_;
};