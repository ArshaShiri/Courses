#pragma once

#include<fstream>
#include <string>

#include "parser.h"

class TranslatorHelper
{
public:
  TranslatorHelper(const Parser &parser,
                   std::ofstream &outputFile);

protected:
  void spMinusMinus_();
  void spPlusPlus_();
  void assignArg2ToD_();
  void assignDToStackPointer_();
  void assignStackPointerToD_();

  std::ofstream& getOutputFile_();
  const Parser& getParser_() const;

  static const std::string aCommandStart_;

  // Points to the next available location in the stack.
  static const std::string stackPointerRegister_;

  // Points to the base address of the segment.
  static const std::string LCLRegister_;
  static const std::string ARGRegister_;
  static const std::string THISRegister_;
  static const std::string THATRegister_;
  static const std::string TEMPRegister_;

private:
  std::ofstream &outputFile_;
  const Parser &parser_;
};