#include <iostream>
#include <dirent.h>
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

Translator::Translator(const std::string &fileOrDirPath)
{
  const auto extensionIndicator = '.';
  const auto extensionIndicatorLocation = fileOrDirPath.find_last_of(extensionIndicator);
  isFilePathADirectory_ = extensionIndicatorLocation == fileOrDirPath.npos;

  const auto lastDirectorySeparatorLocation = fileOrDirPath.find_last_of(PATH_SEPARATOR);
  isDirectoryOrFileInCurrentDir_ = lastDirectorySeparatorLocation == fileOrDirPath.npos;

  // First we determine the output file path.
  const auto outputFilePath = getOutputFilePath_(fileOrDirPath,
                                                 lastDirectorySeparatorLocation,
                                                 extensionIndicatorLocation);
  outputFile_.open(outputFilePath);

  // populate address map.
  if (isFilePathADirectory_)
  {
  }
  else
  {
    auto inputFileNameWithoutExtention = std::string{};
    if (isDirectoryOrFileInCurrentDir_)
    {
      inputFileNameWithoutExtention = fileOrDirPath.substr(0, extensionIndicatorLocation);
    }
    else
    {
      inputFileNameWithoutExtention =
        fileOrDirPath.substr(lastDirectorySeparatorLocation + 1,
                             extensionIndicatorLocation - lastDirectorySeparatorLocation - 1);

    }
    inputFilePathsAndNamesWithoutExtension_.emplace_back(fileOrDirPath, inputFileNameWithoutExtention);
  }
}

std::string Translator::getOutputFilePath_(
  const std::string &fileOrDirPath,
  size_t lastDirectorySeparatorLocation,
  size_t extensionIndicatorLocation) const
{
  auto outputFilePath = std::string{};
  const auto outputExtension = ".asm";

  if (isFilePathADirectory_)
  {
    if (isDirectoryOrFileInCurrentDir_)
      outputFilePath = fileOrDirPath + fileOrDirPath + outputExtension;
    else
      outputFilePath = 
        fileOrDirPath + fileOrDirPath.substr(lastDirectorySeparatorLocation + 1) + outputExtension;
  }
  else
    outputFilePath = fileOrDirPath.substr(0, extensionIndicatorLocation) + outputExtension;

  //std::cout << "path is " << fileOrDirPath << '\n';
  //std::cout << "outputFilePath is " << outputFilePath << '\n';
  return outputFilePath;
}

Translator::Translator(Translator &&otherTranslator)
{
  inputFilePathsAndNamesWithoutExtension_ = std::move(otherTranslator.inputFilePathsAndNamesWithoutExtension_);
  outputFile_ = std::move(otherTranslator.outputFile_);
}

Translator& Translator::operator=(Translator &&otherTranslator)
{
  inputFilePathsAndNamesWithoutExtension_ = std::move(otherTranslator.inputFilePathsAndNamesWithoutExtension_);
  outputFile_ = std::move(otherTranslator.outputFile_);
  return *this;
}

Translator::~Translator()
{
  outputFile_.close();
}

void Translator::translate()
{
  for (const auto inputFilePathAndNameWithoutExtension : inputFilePathsAndNamesWithoutExtension_)
  {
    auto parser = Parser{inputFilePathAndNameWithoutExtension.first};

    while (parser.hasMoreCommands())
    {
      parser.advance();
      write_(parser, inputFilePathAndNameWithoutExtension.second);
    }
  }
}

void Translator::write_(const Parser &parser, 
                        const std::string &inputFilePathAndNameWithoutExtension)
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
    writePushPop_(parser, inputFilePathAndNameWithoutExtension);
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
    pArithmeticTranslatorHelper_ = std::make_unique<ArithmeticHelper>(parser, outputFile_);

  pArithmeticTranslatorHelper_->write();
}

void Translator::writePushPop_(const Parser &parser, 
                               const std::string &inputFilePathAndNameWithoutExtension)
{
  if (pPushPopHelper_ == nullptr)
    pPushPopHelper_ = std::make_unique<PushPopHelper>(parser, inputFilePathAndNameWithoutExtension, outputFile_);

  pPushPopHelper_->write();
}

void Translator::writeFlowCommand_(const Parser &parser)
{
  if (pFlowHelper_ == nullptr)
    pFlowHelper_ = std::make_unique<FlowHelper>(parser, outputFile_);

  pFlowHelper_->write();
}

void Translator::writeFunctionCommand_(const Parser &parser)
{
  if (pFunctionHelper_ == nullptr)
    pFunctionHelper_ = std::make_unique<FunctionHelper>(parser, outputFile_);

  pFunctionHelper_->write();
}