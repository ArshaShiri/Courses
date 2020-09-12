#pragma once

#include <string>
#include <vector>

#include "command helper.h"
#include "common.h"

class Parser
{
public:
  Parser(const std::string &fileName, bool addSisInitCall = false);

  bool hasMoreCommands() const;
  void advance();

  const std::string &getCurrentVMCommand() const;
  CommandType getCommandType() const;
  const std::string &getArg1() const;
  int getArg2() const;

private:
  // Private methods
  void storeVMProgram_(std::ifstream &inputFile);

  // Private attributes.
  int currentCommandLine_;
  CommandHelper currentCommandHelper_;
  std::vector<std::string> VMProgram_;
  std::vector<std::string>::const_iterator currentCommandIt_;
};