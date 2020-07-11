#include <vector>

#include "AInstruction.h"

namespace
{
  std::string convertToBinary(int integer);
}  // End of unnamed namespace declaration.

std::string AInstruction::translate(const std::string &AInstruction)
{
  // A Instructions start with @ which is the 1st character.
  const auto integerValue = std::stoi(AInstruction.substr(1));
  return convertToBinary(integerValue);
}

namespace
{
  std::string convertToBinary(int integer)
  {
    const auto numberOfPlacesInBinaryValue = 16;
    auto binaryValye = std::vector<int>(16);

    for (auto &place : binaryValye)
    {
      place = integer % 2;
      integer = integer / 2;
    }

    auto binaryString = std::string{};
    for (auto it = binaryValye.rbegin(); it != binaryValye.rend(); ++it)
    {
      binaryString.append(std::to_string(*it));
    }

    return binaryString;
  }
}  // End of unnamed namespace definitio
