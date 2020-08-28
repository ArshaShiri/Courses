#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "command helper.h"

namespace
{
  bool isCommandArithmeticOrLogical(const std::string &commandTypeString);
} // end of namespace declaration

CommandHelper::CommandHelper(const std::string &command) : commandType_{CommandType::C_UNKNOWN}
{
  std::istringstream iss(command);

  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter(tokens_));

  if ((tokens_.size() > 3) || (tokens_.empty()))
    std::cout << "number of detected tokens is wrong!" << '\n';

  setCommandType_();
  setArg1_();

  if (tokens_.size() == 3)
    setArg2_();
}

void CommandHelper::setCommandType_()
{
  const auto firstTokken = tokens_.at(0);

  if (isCommandArithmeticOrLogical(firstTokken))
  {
    commandType_ = CommandType::C_ARITHMETIC;
    return;
  }

  if (firstTokken == "push")
  {
    commandType_ = CommandType::C_PUSH;
    return;
  }

  if (firstTokken == "pop")
  {
    commandType_ = CommandType::C_POP;
    return;
  }

  if (firstTokken == "label")
  {
    commandType_ = CommandType::C_LABLE;
    return;
  }

  if (firstTokken == "goto")
  {
    commandType_ = CommandType::C_GOTO;
    return;
  }

  if (firstTokken == "if-goto")
  {
    commandType_ = CommandType::C_IF;
    return;
  }
}

void CommandHelper::setArg1_()
{
  switch (commandType_)
  {
  case CommandType::C_ARITHMETIC:
  {
    arg1_ = tokens_.at(0);
    break;
  }
  case CommandType::C_RETURN:
  {
    std::cout << "arg1 is not meant for C_RETURN commands" << '\n';
    break;
  }
  default:
  {
    arg1_ = tokens_.at(1);
    break;
  }
  }
}

void CommandHelper::setArg2_()
{
  switch (commandType_)
  {
  case CommandType::C_PUSH:
  case CommandType::C_POP:
  case CommandType::C_FUNCTION:
  case CommandType::C_CALL:
  {
    arg2_ = std::stoi(tokens_.at(2));
    break;
  }
  default:
  {
    std::cout << "arg2 is not meant for this command" << '\n';
    break;
  }
  }
}

CommandType CommandHelper::getCommandType()  const
{
  return commandType_;
}

const std::string &CommandHelper::getArg1()  const
{
  return arg1_;
}

int CommandHelper::getArg2()  const
{
  return arg2_;
}

namespace
{
  bool isCommandArithmeticOrLogical(const std::string &commandTypeString)
  {
    if (commandTypeString == "add") return true;
    if (commandTypeString == "sub") return true;
    if (commandTypeString == "neg") return true;

    if (commandTypeString == "eq") return true;
    if (commandTypeString == "gt") return true;
    if (commandTypeString == "lt") return true;

    if (commandTypeString == "and") return true;
    if (commandTypeString == "or") return true;
    if (commandTypeString == "not") return true;

    return false;
  }
} // end of namespace definition