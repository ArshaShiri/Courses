#pragma once

#include <iostream>
#include <string>

enum class TokenType
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
  VOID,
  VAR,
  STATIC,
  FIELD,
  LET,
  DO,
  IF,
  ELSE,
  WHILE,
  RETURN,
  TRUE,
  FASLE,
  NULL_,
  THIS
};

class Token
{
public:
  Token(const std::string &name, TokenType tokenType);

  TokenType getTokenType() const;

  template <TokenType TT>
  decltype(auto) getValue() const;

  friend std::ostream& operator<<(std::ostream &os, const Token &token);


private:
  KeywordType getKeyWordType_() const;
  char getSymbol_() const;
  std::string getName_() const;
  std::string getType_() const;

  std::string name_;
  TokenType tokenType_;
};

template <TokenType TT>
decltype(auto) Token::getValue() const
{
  if constexpr (TT == TokenType::KEYWORD)
    return getKeyWordType_();
  else if constexpr (TT == TokenType::SYMBOL)
    return getSymbol_();
  else
    return getName_();
}




//class Token
//{
//public:
//  Token(const std::string &name);
//  const std::string& getName() const;
//
//  friend std::ostream& operator<<(std::ostream &os, const Token &token);
//  
//private:
//  virtual std::string getType_() const = 0;
//
//  std::string name_;
//};
//
//class Symbol : public Token
//{
//public:
//  Symbol(const std::string &name);
//
//private:
//  std::string getType_() const override;
//};
//
//class IntegerConstant : public Token
//{
//public:
//  IntegerConstant(const std::string &name);
//
//private:
//  std::string getType_() const override;
//};
//
//class StringConstant : public Token
//{
//public:
//  StringConstant(const std::string &name);
//
//private:
//  std::string getType_() const override;
//};
//
//class Identifier : public Token
//{
//public:
//  Identifier(const std::string &name);
//
//private:
//  std::string getType_() const override;
//};
//

//
//class Keyword : public Token
//{
//public:
//  Keyword(const std::string &name);
//  KeywordType getKeywordType() const;
//
//private:
//  std::string getType_() const override;
//
//  KeywordType keywordType_;
//};