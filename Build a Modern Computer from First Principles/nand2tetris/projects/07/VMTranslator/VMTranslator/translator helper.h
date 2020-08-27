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

class PushPopHelper : public TranslatorHelper
{
public:
  PushPopHelper(const Parser &parser,
                const std::string &fileNameWithNoExtension,
                std::ofstream &outputFile);
  void push();
  void pop();

private:
  // Private methods.
  void doPush_(const std::string &baseAddressRegister);
  void pushConstant_();
  void pushStatic_();
  void pushPointer_();

  void doPop_(const std::string &baseAddressRegister);
  void popStatic_();
  void popPointer_();

  // Private attributes

  // Points to the base address of the segment.
  static const std::string LCLRegister_;
  static const std::string ARGRegister_;
  static const std::string THISRegister_;
  static const std::string THATRegister_;
  static const std::string TEMPRegister_;

  const std::string helperRegister14 = "R14";
};

class ArtihmeticTranslatorHelper : public TranslatorHelper
{
public:
  ArtihmeticTranslatorHelper(const Parser &parser,
                             const std::string &fileNameWithNoExtension,
                             std::ofstream &outputFile);

  void doAdd();
  void doSub();
  void doNeg();
  void doEq();
  void doGt();
  void doLt();
  void doAnd();
  void doOr();
  void doNot();


private:
  void operateOnTwoTopValuesInStackAndStoreResultInD_(std::string operatorType);
  void doComparison_(const std::string &trueLable,
                     const std::string &commonInstructionLable,
                     const std::string & counter,
                     const std::string & jumpCondition);
  void popToRegister_(const std::string &regsiter);

  // const std::string helperRegister14 = "R14";
  size_t eqLableCounter_;
  size_t gtLableCounter_;
  size_t ltLableCounter_;
};