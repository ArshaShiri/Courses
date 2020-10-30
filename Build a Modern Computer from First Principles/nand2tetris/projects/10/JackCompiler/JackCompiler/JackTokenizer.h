#pragma once

#include<memory>
#include <string>
#include <vector>

#include "Token.h"

class JackTokenizer
{
public:
  JackTokenizer(const std::string &inputFileOrDirPath);
  bool hasMoreTokens() const;
  void advance();
  const Token* getCurrentToken() const;

private:
  void storeTokens_(std::ifstream &inputFile);
  void extractTokensInString_(const std::string &txt);
  //void setStringTokensInNextLine_();

  std::vector<std::string> tokens_;
  //std::vector<std::string> stringTokensInCurrentLine_;

  bool hasMoreTokens_;
  size_t currentTokenNumber_;
  //size_t currentTokenNumberInLine_;
  std::unique_ptr<Token> pToken_;
};

