#include <algorithm>
#include <fstream> 
#include <iostream>
#include <locale>

#include "parser.h"

namespace
{
  void removeComments(std::string &string);
} // end of namespace declaration

Parser::Parser(const std::string &fileName) : currentCommandLine_{0}
{
  auto inputFile = std::ifstream(fileName);

  if (!inputFile)
  {
    std::cout << "Cannot open input file.\n";
    return;
  }

  storeVMProgram_(inputFile);
}

void Parser::storeVMProgram_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};

  while (std::getline(inputFile, currentLine))
  {
    removeComments(currentLine);
    if (currentLine.empty()) continue;

    VMProgram_.emplace_back(currentLine);
  }
}

bool Parser::hasMoreCommands() const
{
  if (currentCommandLine_ == 0)
    return !VMProgram_.empty();

  return currentCommandIt_ + 1 != VMProgram_.end();
}

void Parser::advance()
{
  if (currentCommandLine_++ == 0)
    currentCommandIt_ = VMProgram_.begin();
  else
    ++currentCommandIt_;

  currentCommandHelper_ = CommandHelper{*currentCommandIt_};
}

const std::string &Parser::getCurrentVMCommand() const
{
  return *currentCommandIt_;
}

CommandType Parser::getCommandType() const
{
  return currentCommandHelper_.getCommandType();
}

const std::string &Parser::getArg1() const
{
  return currentCommandHelper_.getArg1();
}

int Parser::getArg2() const
{
  return currentCommandHelper_.getArg2();
}

namespace
{
void removeComments(std::string &string)
{
  const auto commentStart = "//";
  const auto commentStartingPosition = string.find(commentStart);

  // No comment in this string.
  if (commentStartingPosition == string.npos) return;

  // The entire string is a comment.
  if (commentStartingPosition == 0)
  {
    string.clear();
    return;
  }

  // Remove the comment.
  string.erase(commentStartingPosition);
}
} // end of namespace definition