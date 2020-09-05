#pragma once

#include "translator helper.h"

class ArithmeticHelper : public TranslatorHelper
{
public:
  ArithmeticHelper(const Parser &parser,
                   std::ofstream &outputFile);
  void write();

private:
  // Private methods.
  void operateOnTwoTopValuesInStackAndStoreResultInD_(std::string operatorType);
  void doComparison_(const std::string &trueLable,
                     const std::string &commonInstructionLable,
                     const std::string & counter,
                     const std::string & jumpCondition);
  void popToRegister_(const std::string &regsiter);

  void add_();
  void sub_();
  void neg_();
  void eq_();
  void gt_();
  void lt_();
  void and_();
  void or_();
  void not_();

  // Private attributes.
  size_t eqLabelCounter_;
  size_t gtLabelCounter_;
  size_t ltLabelCounter_;
};

