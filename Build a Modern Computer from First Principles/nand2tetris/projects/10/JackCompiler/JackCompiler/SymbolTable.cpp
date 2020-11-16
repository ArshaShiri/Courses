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
    classSymbolTable_.emplace(identifierName,
                              IdentifierInfo{identifierType, kind, argCounter_++});
    break;
  }
  case IdentifierKind::VAR:
  {
    classSymbolTable_.emplace(identifierName,
                              IdentifierInfo{identifierType, kind, varCounter_++});
    break;
  }

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
    if (symbolTableType.empty())
      return count;

    for (const auto &symbol : symbolTableType)
    {
      if (symbol.second.identifierKind == kind)
        ++count;
    }

    return count;
  };


  switch (kind)
  {
  case IdentifierKind::STATIC:
  case IdentifierKind::FIELD:
    return getVarCount(classSymbolTable_, kind);

  case IdentifierKind::ARG:
  case IdentifierKind::VAR:
    return getVarCount(subroutineSymbolTable_, kind);

  default:
    throw std::runtime_error("Identifier kind cannot be categorized in " __FUNCTION__);
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

const std::string &SymbolTable::getTypeOf(const std::string &identifierName) const
{
  return getIdentifierInfo_(identifierName).identifierType;
}

int SymbolTable::getIndexOf(const std::string &identifierName) const
{
  return getIdentifierInfo_(identifierName).identifierIndex;
}

const IdentifierInfo &SymbolTable::getIdentifierInfo_(const std::string &identifierName) const
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