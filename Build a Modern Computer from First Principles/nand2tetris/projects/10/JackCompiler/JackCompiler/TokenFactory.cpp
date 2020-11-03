#include <algorithm>
#include <cctype>

#include "TokenFactory.h"


TokenFactory::TokenFactory(const std::string &token) : tokenName_{token}
{}

std::unique_ptr<Token> TokenFactory::tokenize()
{
  if (isKeyword()) return std::make_unique<Token>(tokenName_, JackTokenType::KEYWORD);
  if (isSymbol()) return std::make_unique<Token>(tokenName_, JackTokenType::SYMBOL);
  if (isIntegerConst()) return std::make_unique<Token>(tokenName_, JackTokenType::INTEGERCONSTATNT);
  if (isStringConst()) return std::make_unique<Token>(tokenName_, JackTokenType::STRINGCONSTANT);
  if (isIdentifier()) return std::make_unique<Token>(tokenName_, JackTokenType::IDENTIFIER);

  throw std::runtime_error("Cannot tokenize the current token: " + tokenName_);
}


bool TokenFactory::isKeyword()
{
  if (tokenName_ == "class")        return true;
  if (tokenName_ == "method")       return true;
  if (tokenName_ == "function")     return true;

  if (tokenName_ == "constructor")  return true;
  if (tokenName_ == "int")          return true;
  if (tokenName_ == "boolean")      return true;

  if (tokenName_ == "char")         return true;
  if (tokenName_ == "void")         return true;
  if (tokenName_ == "var")          return true;

  if (tokenName_ == "static")       return true;
  if (tokenName_ == "field")        return true;
  if (tokenName_ == "let")          return true;

  if (tokenName_ == "do")           return true;
  if (tokenName_ == "if")           return true;
  if (tokenName_ == "else")         return true;

  if (tokenName_ == "while")        return true;
  if (tokenName_ == "return")       return true;
  if (tokenName_ == "true")         return true;

  if (tokenName_ == "false")        return true;
  if (tokenName_ == "null")         return true;
  if (tokenName_ == "this")         return true;

  return false;
}

bool TokenFactory::isSymbol()
{
  if (tokenName_ == "{")  return true;
  if (tokenName_ == "}")  return true;
  if (tokenName_ == "(")  return true;

  if (tokenName_ == ")")  return true;
  if (tokenName_ == "[")  return true;
  if (tokenName_ == "]")  return true;

  if (tokenName_ == ".")  return true;
  if (tokenName_ == ",")  return true;
  if (tokenName_ == ";")  return true;

  if (tokenName_ == "+")  return true;
  if (tokenName_ == "-")  return true;
  if (tokenName_ == "*")  return true;

  if (tokenName_ == "/")  return true;
  if (tokenName_ == "*")  return true;
  if (tokenName_ == "|")  return true;

  if (tokenName_ == "<")  return true;
  if (tokenName_ == ">")  return true;
  if (tokenName_ == "=")  return true;

  // if (tokenName_ == "")        return true; Not sure what is this in the specs??

  return false;
}

bool TokenFactory::isIntegerConst()
{
  return !tokenName_.empty() && std::all_of(tokenName_.begin(), tokenName_.end(), ::isdigit);
}

bool TokenFactory::isStringConst()
{
  const auto startsWithQuotation = tokenName_.find_first_of("") == 0;
  const auto EndsWithQuotation = tokenName_.find_last_of("") == tokenName_.size() - 1;

  return startsWithQuotation && EndsWithQuotation;
}

bool TokenFactory::isIdentifier()
{
  return !tokenName_.empty();
}