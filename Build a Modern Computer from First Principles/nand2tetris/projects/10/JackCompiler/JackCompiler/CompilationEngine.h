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
  void compileClassVarDec_();
  void compileClassSubroutine_();

  void compileParameterList_();
  void compileVarDec_();
  void compileStatements_();

  void compileDo_();
  void compileLet_();
  void compileWhile_();

  void compileIf_();
  void compileExpression_();
  void compileTerm_();

  void compileExpressionList_();

  const Token* advanceAndGetNextToken();

  JackTokenizer &tokenizer_;
  std::ofstream outputFile_;
};