#include <functional>
#include <iostream>

#include "push pop helper.h"

PushPopHelper::PushPopHelper(
  const Parser &parser,
  std::ofstream &outputFile) : TranslatorHelper{parser, outputFile}
{}

void PushPopHelper::write()
{
  const auto commandType = getParser_().getCommandType();

  if (commandType == CommandType::C_PUSH)
    push_();
  else
    pop_();
}

void PushPopHelper::setFileNameWithNoExtension(const std::string &fileNameWithNoExtension)
{
  fileNameWithNoExtension_ = fileNameWithNoExtension;
}

void PushPopHelper::push_()
{
  const auto &segment = getParser_().getArg1();

  if (segment == "constant")
  {
    pushConstant_();
    return;
  }
  if (segment == "local")
  {
    doPush_(LCLRegister_);
    return;
  }
  if (segment == "argument")
  {
    doPush_(ARGRegister_);
    return;
  }
  if (segment == "this")
  {
    doPush_(THISRegister_);
    return;
  }
  if (segment == "that")
  {
    doPush_(THATRegister_);
    return;
  }
  if (segment == "static")
  {
    pushStatic_();
    return;
  }
  if (segment == "temp")
  {
    doPush_(TEMPRegister_);
    return;
  }
  if (segment == "pointer")
  {
    pushPointer_();
    return;
  }

  std::cout << "The given segment cannot be pushed to!\n";
}

void PushPopHelper::pop_()
{
  const auto &segment = getParser_().getArg1();

  if (segment == "local")
  {
    doPop_(LCLRegister_);
    return;
  }
  if (segment == "argument")
  {
    doPop_(ARGRegister_);
    return;
  }
  if (segment == "this")
  {
    doPop_(THISRegister_);
    return;
  }
  if (segment == "that")
  {
    doPop_(THATRegister_);
    return;
  }
  if (segment == "static")
  {
    popStatic_();
    return;
  }
  if (segment == "temp")
  {
    doPop_(TEMPRegister_);
    return;
  }
  if (segment == "pointer")
  {
    popPointer_();
    return;
  }

  std::cout << "The given segment cannot be popped to!\n";
}

void PushPopHelper::doPush_(const std::string &baseAddressRegister)
{
  auto &outputFile = getOutputFile_();

  // Calculating the address relative to the base pointer.
  outputFile << aCommandStart_ << baseAddressRegister << '\n';

  if (baseAddressRegister == TEMPRegister_)
    outputFile << "D=A\n";
  else
    outputFile << "D=M\n";

  outputFile << aCommandStart_ + std::to_string(getParser_().getArg2()) + '\n';
  outputFile << "A=A+D\n";

  // *sp = *add
  outputFile << "D=M\n";
  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::doPop_(const std::string &baseAddressRegister)
{
  auto &outputFile = getOutputFile_();

  // Calculating the address relative to the base pointer.
  outputFile << aCommandStart_ << baseAddressRegister << '\n';

  if (baseAddressRegister == TEMPRegister_)
    outputFile << "D=A\n";
  else
    outputFile << "D=M\n";

  outputFile << aCommandStart_ + std::to_string(getParser_().getArg2()) + '\n';
  outputFile << "D=A+D\n";
  outputFile << aCommandStart_ + helperRegister14 + '\n';
  outputFile << "M=D\n";

  spMinusMinus_();
  assignStackPointerToD_();
  outputFile << aCommandStart_ + helperRegister14 + '\n';
  outputFile << "A=M\n";
  outputFile << "M=D\n";
}

void PushPopHelper::pushConstant_()
{
  assignArg2ToD_();
  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::pushStatic_()
{
  const auto varName = fileNameWithNoExtension_ + '.' + std::to_string(getParser_().getArg2());

  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << varName << '\n';
  outputFile << "D=M\n";

  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::pushPointer_()
{
  auto &outputFile = getOutputFile_();
  const auto segmentIndex = getParser_().getArg2();

  auto thisThatPush = [&](const std::string &thisOrThatRegister)
  {
    outputFile << aCommandStart_ << thisOrThatRegister << '\n';
    outputFile << "D=M\n";
    assignDToStackPointer_();
    spPlusPlus_();
  };

  // Accessing THAT
  if (segmentIndex == 1)
    thisThatPush(THATRegister_);

  // Accessing THIS
  else if (segmentIndex == 0)
    thisThatPush(THISRegister_);

  else
    std::cout << "Pointer segment index is not supported!\n";
}

void PushPopHelper::popPointer_()
{
  auto &outputFile = getOutputFile_();
  const auto segmentIndex = getParser_().getArg2();

  auto thisThatPOP = [&](const std::string &thisOrThatRegister)
  {
    spMinusMinus_();
    assignStackPointerToD_();
    outputFile << aCommandStart_ << thisOrThatRegister << '\n';
    outputFile << "M=D\n";
  };

  // Accessing THAT
  if (segmentIndex == 1)
    thisThatPOP(THATRegister_);

  // Accessing THIS
  else if (segmentIndex == 0)
    thisThatPOP(THISRegister_);

  else
    std::cout << "Pointer segment index is not supported!\n";
}

void PushPopHelper::popStatic_()
{
  spMinusMinus_();
  assignStackPointerToD_();

  const auto varName = fileNameWithNoExtension_ + '.' + std::to_string(getParser_().getArg2());
  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << varName << '\n';
  outputFile << "M=D\n";
}