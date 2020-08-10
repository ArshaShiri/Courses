#include <iostream>
#include <utility>

#include "parser.h"
#include "translator helper.h"
#include "translator.h"

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
  auto artihmeticTranslatorHelper = ArtihmeticTranslatorHelper(parser, outputFile_);

  if (arg1 == "add")
  {
    artihmeticTranslatorHelper.add();
    return;
  }
}

void Translator::writePushPop_(const Parser &parser)
{
  const auto commandType = parser.getCommandType();
  auto pushPopHelper = PushPopHelper(parser, outputFile_);

  if (commandType == CommandType::C_PUSH)
  {
    pushPopHelper.pushConstant();
  }
  else
  {
    // pop command
  }
}

namespace
{
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