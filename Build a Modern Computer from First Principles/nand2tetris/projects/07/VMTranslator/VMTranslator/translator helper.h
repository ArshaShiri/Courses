#pragma once

#include<fstream>
#include <string>

#include "parser.h"

class TranslatorHelper
{
public:
  TranslatorHelper(const Parser &parser,
                   const std::string &fileNameWithNoExtension,
                   std::ofstream &outputFile);

protected:
  void spMinusMinus_();
  void spPlusPlus_();
  void assignArg2ToD_();
  void assignDToStackPointer_();
  void assignStackPointerToD_();

  std::ofstream& getOutputFile_();
  const std::string& getFileNameWithNoExtension_() const;
  const Parser& getParser_() const;

  static const std::string aCommandStart_;

  // Points to the next available location in the stack.
  static const std::string stackPointerRegister_;

private:
  std::ofstream &outputFile_;
  const std::string &fileNameWithNoExtension_;
  const Parser &parser_;
};