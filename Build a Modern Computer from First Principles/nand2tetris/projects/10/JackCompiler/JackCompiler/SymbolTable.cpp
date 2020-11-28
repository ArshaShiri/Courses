#include <limits>

#include "SymbolTable.h"

SymbolTable::SymbolTable() : staticCounter_{0}, fieldCounter_{0}, argCounter_{0}, varCounter_{0}
{}

void SymbolTable::define(const std::string &identifierName,
                         const std::string &identifierType,
                         IdentifierKind kind)
{
  switch (kind)
  {
  case IdentifierKind::STATIC:
  {
    classSymbolTable_.emplace(identifierName,
                              IdentifierInfo{identifierType, kind, staticCounter_++});
    break;
  }
  case IdentifierKind::FIELD:
  {
    classSymbolTable_.emplace(identifierName,
                              IdentifierInfo{identifierType, kind, fieldCounter_++});
    break;
  }
  case IdentifierKind::ARG:
  {
    subroutineSymbolTable_.emplace(identifierName,
                                   IdentifierInfo{identifierType, kind, argCounter_++});
    break;
  }
  case IdentifierKind::VAR:
  {
    subroutineSymbolTable_.emplace(identifierName,
                                   IdentifierInfo{identifierType, kind, varCounter_++});
    break;
  }

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in SymbolTable::define");
  }
}

int SymbolTable::varCount(IdentifierKind kind) const
{
  switch (kind)
  {
  case IdentifierKind::STATIC:
    return staticCounter_;

  case IdentifierKind::FIELD:
    return fieldCounter_;

  case IdentifierKind::ARG:
    return argCounter_;

  case IdentifierKind::VAR:
    return varCounter_;

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in SymbolTable::varCount");
  }
}

bool SymbolTable::doesIdentifierExist(const std::string &identifierName) const
{
  return (classSymbolTable_.find(identifierName) != classSymbolTable_.end()) &&
         (subroutineSymbolTable_.find(identifierName) != subroutineSymbolTable_.end());
}

void SymbolTable::clearSubroutineSymbolTable()
{
  subroutineSymbolTable_.clear();
  argCounter_ = 0;
  varCounter_ = 0;
}

IdentifierKind SymbolTable::getKindOf(const std::string &identifierName) const
{
  return getIdentifierInfo_(identifierName).identifierKind;
}

const std::string SymbolTable::getTypeOf(const std::string &identifierName) const
{
  return getIdentifierInfo_(identifierName).identifierType;
}

int SymbolTable::getIndexOf(const std::string &identifierName) const
{
  return getIdentifierInfo_(identifierName).identifierIndex;
}

const IdentifierInfo SymbolTable::getIdentifierInfo_(const std::string &identifierName) const
{
  auto subroutineSymbolIt = subroutineSymbolTable_.find(identifierName);
  if (subroutineSymbolIt != subroutineSymbolTable_.end())
    return subroutineSymbolIt->second;

  auto classSymbolIt = classSymbolTable_.find(identifierName);
  if (classSymbolIt != classSymbolTable_.end())
    return classSymbolIt->second;
  
  return {"", IdentifierKind::UNDEFINED, std::numeric_limits<size_t>::max()};
}