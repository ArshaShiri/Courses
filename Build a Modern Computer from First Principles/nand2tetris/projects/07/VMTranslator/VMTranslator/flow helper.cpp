
#include "flow helper.h"

FlowHelper::FlowHelper(
  const Parser &parser,
  const std::string &fileNameWithNoExtension,
  std::ofstream &outputFile) : TranslatorHelper{parser, fileNameWithNoExtension, outputFile}
{}

void FlowHelper::write()
{
  const auto commandType = getParser_().getCommandType();

  if (commandType == CommandType::C_LABLE)
    lable_();
  else if (commandType == CommandType::C_GOTO)
    goto_();
  else
    ifGoto_();
}

void FlowHelper::lable_()
{
  const auto lableName = getParser_().getArg1();
  auto &outputFile = getOutputFile_();
  outputFile << "(" + lableName + ")\n";
}

void FlowHelper::goto_()
{
  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << getParser_().getArg1() << '\n';
  outputFile << "0;JMP\n";
}

void FlowHelper::ifGoto_()
{
  spMinusMinus_();
  assignStackPointerToD_();

  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << getParser_().getArg1() << '\n';
  outputFile << "D;JNE\n";
}