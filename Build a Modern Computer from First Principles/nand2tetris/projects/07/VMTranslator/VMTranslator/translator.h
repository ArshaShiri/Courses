#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

class ArithmeticHelper;
class FlowHelper;
class FunctionHelper;
class Parser;
class PushPopHelper;

class Translator
{
public:
  Translator(const std::string &fileOrDirPath);
  ~Translator();

  Translator(const Translator &) = delete;
  Translator& operator=(const Translator&) = delete;

  Translator(Translator &&otherTranslator);
  Translator& operator=(Translator &&otherTranslator);
 
  void translate();

private:
  // private methods
  void write_(const Parser &parser, 
              const std::string &inputFilePathAndNameWithoutExtension);

  void writeArithmetic_(const Parser &parser);
  void writePushPop_(const Parser &parser, 
                     const std::string &inputFilePathAndNameWithoutExtension);
  void writeFlowCommand_(const Parser &parser);
  void writeFunctionCommand_(const Parser &parser);

  std::string getOutputFilePath_(const std::string &fileOrDirPath,
                                 size_t lastDirectorySeparator,
                                 size_t extensionIndicatorLocation) const;

  // private attributes
  std::ofstream outputFile_;
  bool isFilePathADirectory_;
  bool isDirectoryOrFileInCurrentDir_;
  std::vector<std::pair<std::string, std::string>> inputFilePathsAndNamesWithoutExtension_;

  std::unique_ptr<ArithmeticHelper> pArithmeticTranslatorHelper_;
  std::unique_ptr <PushPopHelper> pPushPopHelper_;
  std::unique_ptr <FlowHelper> pFlowHelper_;
  std::unique_ptr <FunctionHelper> pFunctionHelper_;
};

