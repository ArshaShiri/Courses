#include <iostream>
#include <utility>

#include "arithmetic helper.h"
#include "flow helper.h"
#include "function helper.h"
#include "parser.h"
#include "push pop helper.h"
#include "translator helper.h"
#include "translator.h"

#if defined(WIN32) || defined(_WIN32) 
  #define PATH_SEPARATOR "\\" 
#else 
  #define PATH_SEPARATOR "/" 
#endif 

Translator::Translator(const std::string &filePath) : filePath_{filePath}
{
  // Create output file.
  const auto extensionIndicator = '.';
  const auto extensionIndicatorLocation = filePath.find_last_of(extensionIndicator);

  if (extensionIndicatorLocation == filePath.npos)
    std::cout << "Filename does not have an extension!" << '\n';

  const auto outputExtension = ".asm";
  const auto outputFilePath = filePath.substr(0, extensionIndicatorLocation) + outputExtension;
  outputFile_.open(outputFilePath);

  const auto fileNameAndExtensionLocation = filePath.find_last_of(PATH_SEPARATOR);

  if (fileNameAndExtensionLocation != filePath.npos)
    fileNameWithNoExtension_ = filePath.substr(fileNameAndExtensionLocation + 1, 
                                               extensionIndicatorLocation - fileNameAndExtensionLocation  - 1);
  else
    fileNameWithNoExtension_ = filePath.substr(0, extensionIndicatorLocation);
}

Translator::Translator(Translator &&otherTranslator)
{
  filePath_ = std::move(otherTranslator.filePath_);
  outputFile_ = std::move(otherTranslator.outputFile_);
}

Translator& Translator::operator=(Translator &&otherTranslator)
{
  filePath_ = std::move(otherTranslator.filePath_);
  outputFile_ = std::move(otherTranslator.outputFile_);
  return *this;
}

Translator::~Translator()
{
  outputFile_.close();
}

void Translator::translate()
{
  auto parser = Parser{filePath_};

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
  case CommandType::C_LABLE: 
  case CommandType::C_GOTO:
  case CommandType::C_IF:
  {
    writeFlowCommand_(parser);
    break;
  }
  case CommandType::C_CALL:
  case CommandType::C_FUNCTION:
  case CommandType::C_RETURN:
  {
    writeFunctionCommand_(parser);
    break;
  }
  default:
    std::cout << "Command type is not supported yet " << __FUNCTION__ << '\n';
    break;
  }
}

void Translator::writeArithmetic_(const Parser &parser)
{
  if (pArithmeticTranslatorHelper_ == nullptr)
    pArithmeticTranslatorHelper_ = std::make_unique<ArithmeticHelper>(parser, fileNameWithNoExtension_, outputFile_);

  pArithmeticTranslatorHelper_->write();
}

void Translator::writePushPop_(const Parser &parser)
{
  if (pPushPopHelper_ == nullptr)
    pPushPopHelper_ = std::make_unique<PushPopHelper>(parser, fileNameWithNoExtension_, outputFile_);

  pPushPopHelper_->write();
}

void Translator::writeFlowCommand_(const Parser &parser)
{
  if (pFlowHelper_ == nullptr)
    pFlowHelper_ = std::make_unique<FlowHelper>(parser, fileNameWithNoExtension_, outputFile_);

  pFlowHelper_->write();
}

void Translator::writeFunctionCommand_(const Parser &parser)
{
  if (pFunctionHelper_ == nullptr)
    pFunctionHelper_ = std::make_unique<FunctionHelper>(parser, fileNameWithNoExtension_, outputFile_);

  pFunctionHelper_->write();
}