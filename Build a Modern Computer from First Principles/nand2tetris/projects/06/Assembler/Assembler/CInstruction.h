#pragma once

#include <string>

// CInstruction is in the form of dest = comp; jump 
// Either the dest or jump fields may be empty.
// If dest is empty, the '=' is omitted;
// If jump is empty, the ';' is omitted.
class CInstruction
{
public:
  static std::string translate(const std::string &CInstruction);
};