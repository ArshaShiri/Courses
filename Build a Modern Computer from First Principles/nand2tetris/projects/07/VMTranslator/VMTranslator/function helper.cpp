#include "function helper.h"

FunctionHelper::FunctionHelper(
  const Parser &parser,
  std::ofstream &outputFile) : TranslatorHelper{parser, outputFile},
                               returnAddressLabelCounter_{0}
{}

void FunctionHelper::write()
{
  const auto commandType = getParser_().getCommandType();

  if (commandType == CommandType::C_FUNCTION)
    function_();
  else if (commandType == CommandType::C_CALL)
    call_();
  else
    return_();
}

void FunctionHelper::pushToStack_(const std::string &argumentToPush)
{

}

void FunctionHelper::function_()
{
  const auto &parser = getParser_();
  const auto functionLable = parser.getArg1();
  const auto numberOfLocalArguments = parser.getArg2();

  // Declare a label for the function entry
  auto &outputFile = getOutputFile_();
  outputFile << "(" + functionLable + ")\n";

  for (auto i = size_t{0}; i < numberOfLocalArguments; ++i)
  {
    outputFile << aCommandStart_ + "0\n";
    outputFile << "D=A\n";
    assignDToStackPointer_();
    spPlusPlus_();
  }
}

void FunctionHelper::call_()
{
  const auto returnAddressLable = "return" + std::to_string(returnAddressLabelCounter_++);
  auto &outputFile = getOutputFile_();

  // Push return address
  outputFile << aCommandStart_ + returnAddressLable + '\n';
  outputFile << "D=A\n";
  assignDToStackPointer_();
  spPlusPlus_();

  auto pushBasePointerRegister = [&outputFile, this](const std::string &basePointerRegisterName)
  {
    // D = basePointerRegisterName
    outputFile << aCommandStart_ + basePointerRegisterName + '\n';
    outputFile << "D=M\n";

    assignDToStackPointer_();
  };

  pushBasePointerRegister(LCLRegister_);
  pushBasePointerRegister(ARGRegister_);
  pushBasePointerRegister(THISRegister_);
  pushBasePointerRegister(THATRegister_);

  // ARG = SP - n - 5 (n is number of args)
  const auto numberOfArgs = std::to_string(getParser_().getArg2());

  // First: D = SP - n - 5
  outputFile << aCommandStart_ + stackPointerRegister_ + '\n';
  outputFile << "D=M\n";
  outputFile << aCommandStart_ + numberOfArgs + '\n';
  outputFile << "D=D-A\n";
  outputFile << aCommandStart_ + "5\n";
  outputFile << "D=D-A\n";

  // Then ARG = D
  outputFile << aCommandStart_ + ARGRegister_ + '\n';
  outputFile << "M=D\n";

  // LCL = SP
  outputFile << aCommandStart_ + stackPointerRegister_ + '\n';
  outputFile << "D=M\n";
  outputFile << aCommandStart_ + LCLRegister_ + '\n';
  outputFile << "M=D\n";

  // goto f
  const auto functionLable = getParser_().getArg1();
  outputFile << aCommandStart_ + functionLable + '\n';
  outputFile << "0;JMP\n";

  // (return address)
  outputFile << "(" + returnAddressLable + ")\n";
}

void FunctionHelper::return_()
{
  auto &outputFile = getOutputFile_();

  // FRAME = LCL
  outputFile << aCommandStart_ + LCLRegister_ + '\n';
  outputFile << "D=M\n";
  outputFile << aCommandStart_ + "FRAME\n";
  outputFile << "M=D\n";

  // RET = *(FRAME-5)
  // D has the value of LCL(FRAME)
  outputFile << aCommandStart_ + "5\n";
  outputFile << "A=D-A\n";

  // D = *(FRAME-5)
  outputFile << "D=M\n";
  outputFile << aCommandStart_ + "RET\n";
  outputFile << "M=D\n";

  // *ARG = pop()
  spMinusMinus_();
  assignStackPointerToD_();
  outputFile << aCommandStart_ + ARGRegister_ + '\n';
  outputFile << "A=M\n";
  outputFile << "M=D\n";

  // SP = ARG + 1
  outputFile << aCommandStart_ + ARGRegister_ + '\n';
  outputFile << "D=M+1\n";
  outputFile << aCommandStart_ << stackPointerRegister_ << '\n';
  outputFile << "M=D\n";

  // basePointerRegister = *(FRAME - n)
  auto resetBasePointerBasedOnFrame = [&outputFile](const std::string &baseRegister,
                                                    const std::string &n)
  { 
    // D = *(FRAME - n)
    outputFile << aCommandStart_ + n + '\n';
    outputFile << "D=A\n";
    outputFile << aCommandStart_ + "FRAME\n";
    outputFile << "A=M-D\n";
    outputFile << "D=M\n";

    // basePointerRegister = D
    outputFile << aCommandStart_ + baseRegister + '\n';
    outputFile << "M=D\n";
  };

  // THAT = *(FRAME - 1)
  resetBasePointerBasedOnFrame(THATRegister_, "1");

  // THIS = *(FRAME - 2)
  resetBasePointerBasedOnFrame(THISRegister_, "2");

  // ARG = *(FRAME - 3)
  resetBasePointerBasedOnFrame(ARGRegister_, "3");

  // THAT = *(FRAME - 4)
  resetBasePointerBasedOnFrame(LCLRegister_, "4");

  // goto RET
  outputFile << aCommandStart_ + "RET\n";
  outputFile << "0;JMP\n";
}