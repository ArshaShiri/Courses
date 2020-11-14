#include "SymbolTable.h"

SymbolType getSymbolType(IdentifierKind kind)
{
  switch (kind)
  {
  case IdentifierKind::STATIC:
  case IdentifierKind::FIELD:
    return SymbolType::CLASS;

  case IdentifierKind::ARG:
  case IdentifierKind::VAR:
    return SymbolType::SUBROUTINE;

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in " __FUNCTION__);
  }
}

SymbolTable::SymbolTable() : classSymbolIndex{0}, subroutineSymbolIndex{0}
{}

void SymbolTable::define(const std::string &identifierName,
                         const std::string &identifierType,
                         IdentifierKind kind)
{
  switch (getSymbolType(kind))
  {
  case SymbolType::CLASS:
    classSymbolTable_.emplace(identifierName, 
                              IdentifierInfo{identifierType, kind, classSymbolIndex++});
  case SymbolType::SUBROUTINE:
    subroutineSymbolTable_.emplace(identifierName,
                                   IdentifierInfo{identifierType, kind, subroutineSymbolIndex++});

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in " __FUNCTION__);
  }
}

int SymbolTable::varCount(IdentifierKind kind) const
{
  auto getVarCount = [](const SymbolTableType &symbolTableType,
                        IdentifierKind kind)
  {
    auto count = size_t{0};

    for (const auto &symbol : symbolTableType)
    {
      if (symbol.second.identifierKind == kind)
        ++count;
    }

    return count;
  };


  switch (getSymbolType(kind))
  {
  case SymbolType::CLASS:
    return getVarCount(classSymbolTable_, kind);

  case SymbolType::SUBROUTINE:
    return getVarCount(subroutineSymbolTable_, kind);

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in " __FUNCTION__);
  }
}

const IdentifierInfo &SymbolTable::getIdentifierInfo(const std::string &identifierName) const
{
  if (auto symbolIt = subroutineSymbolTable_.find(identifierName);
    symbolIt != subroutineSymbolTable_.end())
    return symbolIt->second;

  if (auto symbolIt = classSymbolTable_.find(identifierName);
    symbolIt != classSymbolTable_.end())
    return symbolIt->second;

  throw std::runtime_error("Cannot find the identifier info for " + identifierName +
    "in " __FUNCTION__);
}


IdentifierKind SymbolTable::getKindOf(const std::string &identifierName) const
{
  return getIdentifierInfo(identifierName).identifierKind;
}

const std::string &SymbolTable::getTypeOf(const std::string &identifierName) const
{
  return getIdentifierInfo(identifierName).identifierType;
}

int SymbolTable::getIndexOf(const std::string &identifierName) const
{
  return getIdentifierInfo(identifierName).identifierIndex;
}