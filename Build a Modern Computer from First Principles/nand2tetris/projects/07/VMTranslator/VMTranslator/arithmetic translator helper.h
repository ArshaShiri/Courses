#pragma once

#include "translator helper.h"

class ArithmeticTranslatorHelper : public TranslatorHelper
{
public:
  ArithmeticTranslatorHelper(const Parser &parser,
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

