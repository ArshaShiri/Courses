#pragma once

#include <memory>
#include <string>

#include "Token.h"

class TokenFactory
{
public:
  TokenFactory(const std::string &token);

  std::unique_ptr<Token> tokenize();

private:
  bool isKeyword();
  bool isSymbol();
  bool isIntegerConst();
  bool isStringConst();
  bool isIdentifier();

  const std::string &tokenName_;
};

