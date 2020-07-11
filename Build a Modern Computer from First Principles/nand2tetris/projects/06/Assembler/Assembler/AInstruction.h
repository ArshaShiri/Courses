#pragma once

#include <string>

// AInstruction is in the form of @value where value is either a non-negative decimal number
// or a symbol referring to such number.
class AInstruction
{
public:
  static std::string translate(const std::string &AInstruction);
};