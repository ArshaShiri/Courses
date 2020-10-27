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

class Token
{
public:
  Token(const std::string &name);
  const std::string& getName() const;

  virtual bool isTerminal() { return true; };
  friend std::ostream& operator<<(std::ostream &os, const Token &token);
  
private:
  virtual std::string getType_() const = 0;

  std::string name_;
};

class Symbol : public Token
{
public:
  Symbol(const std::string &name);

private:
  std::string getType_() const override;
};

class IntegerConstant : public Token
{
public:
  IntegerConstant(const std::string &name);

private:
  std::string getType_() const override;
};

class StringConstant : public Token
{
public:
  StringConstant(const std::string &name);

private:
  std::string getType_() const override;
};

class Identifier : public Token
{
public:
  Identifier(const std::string &name);

private:
  std::string getType_() const override;
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

class Keyword : public Token
{
public:
  Keyword(const std::string &name);
  bool isTerminal() override { return true; };
  KeywordType getKeywordType() const;

private:
  std::string getType_() const override;

  KeywordType keywordType_;
};