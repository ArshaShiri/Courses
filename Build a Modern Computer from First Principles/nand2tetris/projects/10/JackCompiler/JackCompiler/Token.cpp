#include "Token.h"


std::ostream& operator<<(std::ostream &os, const Token &token)
{
  os << "<" + token.getType_() + '>' + " " <<
    token.name_ + " "  <<
    "</" + token.getType_() + '>' << '\n';

  return os;
}

Token::Token(const std::string &name) : name_{name}
{}

const std::string& Token::getName() const
{
  return name_;
}

std::string Symbol::getType_() const
{
  return "symbol";
}

Symbol::Symbol(const std::string &name) : Token{name}
{}


std::string IntegerConstant::getType_() const
{
  return "integerConstant";
}

IntegerConstant::IntegerConstant(const std::string &name) : Token{name}
{}


std::string StringConstant::getType_() const
{
  return "stringConstant";
}

StringConstant::StringConstant(const std::string &name) : Token{name}
{}


std::string Identifier::getType_() const
{
  return "identifier";
}

Identifier::Identifier(const std::string &name) : Token{name}
{}

Keyword::Keyword(const std::string &name) : Token{name}
{
  if (name == "class")            { keywordType_ = KeywordType::CLASS; }
  else if (name == "method")      { keywordType_ = KeywordType::METHOD; }
  else if (name == "function")    { keywordType_ = KeywordType::FUNCTION; }
  else if (name == "constructor") { keywordType_ = KeywordType::CONSTRUCTOR; }
  else if (name == "int")         { keywordType_ = KeywordType::INT; }
  else if (name == "boolean")     { keywordType_ = KeywordType::BOOLEAN; }
  else if (name == "char")        { keywordType_ = KeywordType::CHAR; }
  else if (name == "void")        { keywordType_ = KeywordType::VOID; }
  else if (name == "var")         { keywordType_ = KeywordType::VAR; }
  else if (name == "static")      { keywordType_ = KeywordType::STATIC; }
  else if (name == "field")       { keywordType_ = KeywordType::FIELD; }
  else if (name == "let")         { keywordType_ = KeywordType::LET; }
  else if (name == "do")          { keywordType_ = KeywordType::DO; }
  else if (name == "if")          { keywordType_ = KeywordType::IF; }
  else if (name == "else")        { keywordType_ = KeywordType::ELSE; }
  else if (name == "while")       { keywordType_ = KeywordType::WHILE; }
  else if (name == "return")      { keywordType_ = KeywordType::RETURN; }
  else if (name == "true")        { keywordType_ = KeywordType::TRUE; }
  else if (name == "false")       { keywordType_ = KeywordType::FASLE; }
  else if (name == "null")        { keywordType_ = KeywordType::NULL_; }
  else if (name == "this")        { keywordType_ = KeywordType::THIS; }
  else { throw std::runtime_error("Unsupported keyword"); }
}

std::string Keyword::getType_() const
{
  return "keyword";
}

KeywordType Keyword::getKeywordType() const
{
  return keywordType_;
}