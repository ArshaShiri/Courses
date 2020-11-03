#pragma once

#include <iostream>
#include <string>

enum class JackTokenType
{
  KEYWORD,
  SYMBOL,
  INTEGERCONSTATNT,
  STRINGCONSTANT,
  IDENTIFIER
};

enum class KeywordType
{
  CLASS,
  METHOD,
  FUNCTION,
  CONSTRUCTOR,
  INT,
  BOOLEAN,
  CHAR,
  VOID_,
  VAR,
  STATIC,
  FIELD,
  LET,
  DO,
  IF,
  ELSE,
  WHILE,
  RETURN,
  TRUE_,
  FASLE,
  NULL_,
  THIS
};

class Token
{
public:
  Token(const std::string &name, JackTokenType tokenType);

  JackTokenType getTokenType() const;

  template <JackTokenType TT>
  decltype(auto) getValue() const;

  friend std::ostream& operator<<(std::ostream &os, const Token &token);

private:
  KeywordType getKeyWordType_() const;
  char getSymbol_() const;
  std::string getName_() const;
  std::string getType_() const;

  std::string name_;
  JackTokenType tokenType_;
};

template <JackTokenType TT>
decltype(auto) Token::getValue() const
{
  if constexpr (TT == JackTokenType::KEYWORD)
    return getKeyWordType_();
  else if constexpr (TT == JackTokenType::SYMBOL)
    return getSymbol_();
  else
    return getName_();
}