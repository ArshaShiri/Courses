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
  // Private methods:

  // General rule: each compile method starts from the current token and before returning, advances 
  // to the next token.

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

  /*Compiles a sequence of statements, not including the enclosing {}*/
  void compileStatements_();

  /*Compiles do statement.*/
  void compileDo_();

  /*Compiles let statement.*/
  void compileLet_();

  /*Compiles while statement.*/
  void compileWhile_();

  void compileIf_();
  void compileExpression_();
  void compileTerm_();

  /*Compiles a (possibly empty) comma-separated list of expressions, and returns the number of 
    expressions.*/
  size_t compileExpressionList_();


  void compileSubroutineCall_();  
  void compileReturn_();
  void handleKeywordInStatements_(const Token &currentToken);

  /*Translates the token which represents the operator and calls the vmwriter arithmetic writer.*/
  void handleOperator(const char character);

  const Token advanceAndGetNextToken();
  void addToSymbolTable_(const std::string &identifierName,
                         const std::string &identifierType,
                         KeywordType keywordType);

  // Private attributes:
  JackTokenizer &tokenizer_;
  std::string className_;
  VMWriter VMWriter_;
  SymbolTable symbolTable_;
};

