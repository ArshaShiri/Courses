/*Tokenizes the string that found in a jack file. It goes through the file character by *character
  and stores the found tokens.*/

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
  const Token& getCurrentToken() const;

private:
  void tokenize_(std::ifstream &inputFile);
  std::vector<std::string> extractTokensInString_(const std::string &txt) const;
  void buildTokens_(std::vector<std::string> &&tokens);
  void buildToken_(std::string &&token);

  size_t currentTokenNumber_;
  std::vector<Token> tokens_;
  bool waitingForFirstCall_;
};

