#pragma once

#include <string>

#include "common.h"

class CommandHelper
{
public:
  CommandHelper() = default;
  CommandHelper(const std::string &command);
  CommandType getCommandType() const;
  const std::string &getArg1() const;
  int getArg2() const;

private:
  // private methods
  void setCommandType_();
  void setArg1_();
  void setArg2_();

  // private attributes
  std::vector<std::string> tokens_;
  CommandType commandType_;
  std::string arg1_;
  int arg2_;
};

