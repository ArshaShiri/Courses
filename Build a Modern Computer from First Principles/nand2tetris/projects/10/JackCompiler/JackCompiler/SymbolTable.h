/*Keeps track of two main symbol table. One is related to the class which has field or static vars
  and the other one is related to the current subroutine that is being compiled. Since Jack syntax
  requires the variables to be declared at the beginning of each function/subroutine/class, we do
  not more symbol tables. Also scoped variables are not supported as well.*/

#pragma once

#include <string>
#include <unordered_map>

enum class IdentifierKind
{
  STATIC,
  FIELD,
  ARG,
  VAR,
  UNDEFINED
};

struct IdentifierInfo
{
  IdentifierInfo(const std::string &type,
                 IdentifierKind kind,
                 size_t index) : identifierType{type},
                                 identifierKind{kind},
                                 identifierIndex{index}
  {}

  std::string identifierType;
  IdentifierKind identifierKind;
  size_t identifierIndex;
};

using SymbolTableType = std::unordered_map<std::string, IdentifierInfo>;

class SymbolTable
{
public:
  SymbolTable();

  void define(const std::string &identifierName,
              const std::string &identifierType,
              IdentifierKind kind);

  const IdentifierKind &getKindOf(const std::string &identifierName) const;
  const std::string &getTypeOf(const std::string &identifierName) const;

  int varCount(IdentifierKind kind) const;
  int getIndexOf(const std::string &identifierName) const;
  bool doesIdentifierExist(const std::string &identifierName) const;

  void clearSubroutineSymbolTable();

private:
  // Private methods
  const IdentifierInfo &getIdentifierInfo_(const std::string &identifierName) const;

  // Private attributes 
  SymbolTableType classSymbolTable_;
  SymbolTableType subroutineSymbolTable_;

  size_t staticCounter_;
  size_t fieldCounter_;
  size_t argCounter_;
  size_t varCounter_;
};
