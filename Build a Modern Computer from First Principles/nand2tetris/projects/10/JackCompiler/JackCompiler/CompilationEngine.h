#pragma once

#include <fstream>
#include <string>

#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

class Token;

class CompilationEngine
{
public:
  CompilationEngine(const std::string &otputFilepath, JackTokenizer &tokenizer);

private:
  // Private methods

  /*Compiles a complete class.*/
  void compileClass_();

  /*Compiles static or field declarations.*/
  void compileClassVarDec_();

  /*Compiles a class subroutine.*/
  void compileClassSubroutine_();

  /*Compiles a possibly empty parameter list, not including the enclosing ().*/
  void compileParameterList_();

  /*Compiles a var declaration.*/
  void compileVarDec_();

  void compileStatements_();

  void compileDo_();
  void compileLet_();
  void compileWhile_();

  void compileIf_();
  void compileExpression_();
  void compileTerm_();

  void compileExpressionList_();
  void compileSubroutineCall_();
  
  void compileReturn_();
  void handleKeywordInStatements_(const Token &currentToken);

  const Token advanceAndGetNextToken();
  void addToSymbolTable_(const std::string &identifierName,
                        const std::string &identifierType,
                        KeywordType keywordType);

  // Private attributes.
  JackTokenizer &tokenizer_;
  VMWriter VMWriter_;
  SymbolTable symbolTable_;
};

