/*Define all the token types based on Jack language documentations.*/

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

  KeywordType getKeyWordType() const;
  char getSymbol() const;
  const std::string &getName() const;
  std::string getType() const;

  // This uses C++17 which is not supported by the grader of this course on coursera.
  //template <JackTokenType TT>
  //decltype(auto) getValue() const;

private:

  std::string name_;
  JackTokenType tokenType_;
};


//template <JackTokenType TT>
//decltype(auto) Token::getValue() const
//{
//  if constexpr (TT == JackTokenType::KEYWORD)
//    return getKeyWordType_();
//  else if constexpr (TT == JackTokenType::SYMBOL)
//    return getSymbol_();
//  else
//    return getName_();
//}
