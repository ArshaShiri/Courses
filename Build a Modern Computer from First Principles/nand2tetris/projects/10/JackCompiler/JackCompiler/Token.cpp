#include "Token.h"

Token::Token(const std::string &name, JackTokenType tokenType) : name_{name}, tokenType_{tokenType}
{}

KeywordType Token::getKeyWordType() const
{
    if (name_ == "class")            { return KeywordType::CLASS; }
    else if (name_ == "method")      { return KeywordType::METHOD; }
    else if (name_ == "function")    { return KeywordType::FUNCTION; }
    else if (name_ == "constructor") { return KeywordType::CONSTRUCTOR; }
    else if (name_ == "int")         { return KeywordType::INT; }
    else if (name_ == "boolean")     { return KeywordType::BOOLEAN; }
    else if (name_ == "char")        { return KeywordType::CHAR; }
    else if (name_ == "void")        { return KeywordType::VOID_; }
    else if (name_ == "var")         { return KeywordType::VAR; }
    else if (name_ == "static")      { return KeywordType::STATIC; }
    else if (name_ == "field")       { return KeywordType::FIELD; }
    else if (name_ == "let")         { return KeywordType::LET; }
    else if (name_ == "do")          { return KeywordType::DO; }
    else if (name_ == "if")          { return KeywordType::IF; }
    else if (name_ == "else")        { return KeywordType::ELSE; }
    else if (name_ == "while")       { return KeywordType::WHILE; }
    else if (name_ == "return")      { return KeywordType::RETURN; }
    else if (name_ == "true")        { return KeywordType::TRUE_; }
    else if (name_ == "false")       { return KeywordType::FASLE; }
    else if (name_ == "null")        { return KeywordType::NULL_; }
    else if (name_ == "this")        { return KeywordType::THIS; }
    else { throw std::runtime_error("Unsupported keyword: " + name_ + " " + __FUNCTION__); }
}

char Token::getSymbol() const
{
  return name_.at(0);
}

const std::string &Token::getName() const
{
  return name_;
}

JackTokenType Token::getTokenType() const
{
  return tokenType_;
}

std::string Token::getType() const
{
  switch (tokenType_)
  {
    case JackTokenType::KEYWORD: return "keyword";
    case JackTokenType::SYMBOL: return "symbol";
    case JackTokenType::INTEGERCONSTATNT: return "integerConstant";
    case JackTokenType::STRINGCONSTANT: return "stringConstant";
    case JackTokenType::IDENTIFIER: return "identifier";
    default: throw std::runtime_error("Token type is not supported");
  }
}