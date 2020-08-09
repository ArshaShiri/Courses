#include <iostream>
#include <utility>

#include "parser.h"
#include "translator.h"

namespace
{
  const std::string aCommandStart = {"@"};
  const std::string stackPointerRegister = {"R0"};

  void pushConstant(std::ofstream &file, int arg2);
  void popToRegister(std::ofstream &file, const std::string &regsiter);

  void spMinusMinus(std::ofstream &file);
  void spPlusPlus(std::ofstream &file);
  void assignArg2ToD(std::ofstream &file, int arg2);
  void assignDToStackPointer(std::ofstream &file);
  void assignStackPointerToD(std::ofstream &file);
} // end of namespace declaration

Translator::Translator(const std::string &fileName) : fileName_{fileName}
{
  const auto extensionIndicator = '.';
  const auto extensionIndicatorLocation = fileName.find(extensionIndicator);

  if (extensionIndicatorLocation == fileName.npos)
    std::cout << "Filename does not have an extension!" << '\n';

  const auto fileNameWithNoExtension = fileName.substr(0, extensionIndicatorLocation);
  const auto outputExtension = ".asm";
  outputFile_.open(fileNameWithNoExtension + outputExtension);
}

Translator::Translator(Translator &&otherTranslator)
{
  fileName_ = std::move(otherTranslator.fileName_);
  outputFile_ = std::move(otherTranslator.outputFile_);
}

Translator& Translator::operator=(Translator &&otherTranslator)
{
  fileName_ = std::move(otherTranslator.fileName_);
  outputFile_ = std::move(otherTranslator.outputFile_);
  return *this;
}

Translator::~Translator()
{
  outputFile_.close();
}

void Translator::translate()
{
  auto parser = Parser{fileName_};
  while (parser.hasMoreCommands())
  {
    parser.advance();
    write_(parser);
  }
}

void Translator::write_(const Parser &parser)
{
  // Write the vm command on the file for debugging purposes.
  outputFile_ << "// " + parser.getCurrentVMCommand() << '\n';
  
  const auto commandType = parser.getCommandType();
  switch (commandType)
  {
  case CommandType::C_ARITHMETIC:
  {
    writeArithmetic_(parser);
    break;
  }
  case CommandType::C_POP:
  case CommandType::C_PUSH:
  {
    writePushPop_(parser);
    break;
  }
  default:
    std::cout << "Command type is not supported yet " << __FUNCTION__ << '\n';
    break;
  }
}

void Translator::writeArithmetic_(const Parser &parser)
{
  const auto &arg1 = parser.getArg1();

  if (arg1 == "add")
  {
    popToRegister(outputFile_, "R14");
    spMinusMinus(outputFile_);
    assignStackPointerToD(outputFile_);

    // store value of D in the given register
    outputFile_ << aCommandStart + "R14" << '\n';
    outputFile_ << "M=D+M\n";
    outputFile_ << "D=M\n";

    assignDToStackPointer(outputFile_);
    spPlusPlus(outputFile_);
  }
}

void Translator::writePushPop_(const Parser &parser)
{
  const auto commandType = parser.getCommandType();
  if (commandType == CommandType::C_PUSH)
  {
    pushConstant(outputFile_, parser.getArg2());
  }
  else
  {
    // pop command
  }
}

namespace
{
  // Increment the stack pointer.
  // sp++
  void spPlusPlus(std::ofstream &file)
  {
    file << aCommandStart << stackPointerRegister << '\n';
    file << "M=M+1\n";
  }

  // Decrement the stack pointer.
  // sp--
  void spMinusMinus(std::ofstream &file)
  {
    file << aCommandStart << stackPointerRegister << '\n';
    file << "M=M-1\n";
  }

  // Assign the value of arg2 to the D register.
  // D = arg2
  void assignArg2ToD(std::ofstream &file, int arg2)
  {
    file << aCommandStart << std::to_string(arg2) << '\n';
    file << "D=A\n";
  }

  // Assign the value of D register to where the stack pointer is pointing to.
  // *sp = D
  void assignDToStackPointer(std::ofstream &file)
  {
    file << aCommandStart << stackPointerRegister << '\n';
    file << "A=M\n";
    file << "M=D\n";
  }

  // Assign the value pointed by stack pointer to D register.
  // D = *sp
  void assignStackPointerToD(std::ofstream &file)
  {
    file << "A=M\n";
    file << "D=M\n";
  }

  void pushConstant(std::ofstream &file, int arg2)
  {
    assignArg2ToD(file, arg2);
    assignDToStackPointer(file);
    spPlusPlus(file);
  }

  // This is useful for arithmetic commands.
  void popToRegister(std::ofstream &file, const std::string &regsiter)
  {
    spMinusMinus(file);
    assignStackPointerToD(file);

    // store value of D in the given register
    file << aCommandStart + regsiter + '\n';
    file << "M=D\n";
  }

  //void writePushCommand(std::ofstream &file, const std::string &arg1, int arg2)
  //{
  //  const auto aCommandStart = '@';
  //  const auto stackPointerRegister = 'sp';

  //  file << aCommandStart + std::to_string(arg2) << '\n';

  //  // Set the correct address to A register.
  //  if (arg1 != "constant")
  //  {
  //    file << "D=A\n";

  //    const auto basePointer = getBasePointer(arg1);
  //    file << aCommandStart + basePointer << '\n';
  //    file << "D=D+M\n";

  //    // store the address in register 13
  //    const auto addressRegister = "R13";
  //    file << aCommandStart + addressRegister << '\n';
  //    file << "M=D\n";
  //  }

  //  file << "D=A\n";

  //  // *sp = D
  //  file << aCommandStart + stackPointerRegister << '\n';
  //  file << "A=M\n";
  //  file << "M=D\n";

  //  // sp++
  //  file << aCommandStart + stackPointerRegister << '\n';
  //  file << "M=M+1\n";
  //}

  //void writePopCommand(std::ofstream &file, const std::string &arg1, int arg2)
  //{
  //  const auto aCommandStart = '@';
  //  const auto stackPointerRegister = 'sp';
  //  file << aCommandStart + std::to_string(arg2) << '\n';

  //  // this part is duplicated with the function above.
  //  // Set the correct address to A register.
  //  file << "D=A\n";

  //  const auto basePointer = getBasePointer(arg1);
  //  file << aCommandStart + basePointer << '\n';
  //  file << "D=D+M\n";

  //  // store the address in register 13
  //  const auto addressRegister = "R13";
  //  file << aCommandStart + addressRegister << '\n';
  //  file << "M=D\n";
  // 
  //  // sp--
  //  file << aCommandStart + stackPointerRegister + '\n';
  //  file << "M=M-1\n";

  //  // D = *sp
  //  file << "A=M\n";
  //  file << "D=M\n";

  //  file << aCommandStart + addressRegister << '\n';
  //  file << "M=D\n";
  //}

  //std::string getBasePointer(const std::string &memeorySegmentName)
  //{
  //  
  //}
} // end of namespace definition