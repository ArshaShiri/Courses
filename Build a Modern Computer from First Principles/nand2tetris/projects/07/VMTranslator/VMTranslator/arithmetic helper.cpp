#include <iostream>

#include "arithmetic helper.h"

ArithmeticHelper::ArithmeticHelper(
  const Parser &parser,
  std::ofstream &outputFile) : TranslatorHelper{parser, outputFile},
                               eqLabelCounter_{0},
                               gtLabelCounter_{0},
                               ltLabelCounter_{0}
{}

void ArithmeticHelper::write()
{
  const auto &arg1 = getParser_().getArg1();

  if (arg1 == "add")
  {
    add_();
    return;
  }
  if (arg1 == "sub")
  {
    sub_();
    return;
  }
  if (arg1 == "neg")
  {
    neg_();
    return;
  }
  if (arg1 == "eq")
  {
    eq_();
    return;
  }
  if (arg1 == "gt")
  {
    gt_();
    return;
  }
  if (arg1 == "lt")
  {
    lt_();
    return;
  }
  if (arg1 == "and")
  {
    and_();
    return;
  }
  if (arg1 == "or")
  {
    or_();
    return;
  }
  if (arg1 == "not")
  {
    not_();
    return;
  }

  std::cout << "Arithmetic operation is not supported!" << '\n';
}

// This is useful for arithmetic commands.
void ArithmeticHelper::popToRegister_(const std::string &regsiter)
{
  spMinusMinus_();
  assignStackPointerToD_();
  auto &outputFile = getOutputFile_();

  // store value of D in the given register
  outputFile << aCommandStart_ + regsiter + '\n';
  outputFile << "M=D\n";
}

void ArithmeticHelper::operateOnTwoTopValuesInStackAndStoreResultInD_(std::string operatorType)
{
  spMinusMinus_();
  assignStackPointerToD_();
  spMinusMinus_();

  auto &outputFile = getOutputFile_();
  outputFile << "A=M\n";

  outputFile << "D=M" << operatorType << "D\n";
}

void ArithmeticHelper::add_()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("+");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::sub_()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("-");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::neg_()
{
  // Store the top value in D register.
  spMinusMinus_();
  assignStackPointerToD_();
  auto &outputFile = getOutputFile_();

  // Negate the value stored in D.
  outputFile << "D=-D\n";
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::doComparison_(
  const std::string &trueLable,
  const std::string &commonInstructionLable,
  const std::string &counter,
  const std::string &jumpCondition)
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("-");
  auto &outputFile = getOutputFile_();

  // Create some labels to control jump instructions.
  const auto currentTrueLable = trueLable + counter;
  const auto currentCommonInstructions = commonInstructionLable + counter;

  // Jump if the condition is true.
  outputFile << aCommandStart_ << currentTrueLable << '\n';
  outputFile << "D;" << jumpCondition << '\n';

  // When condition is false.
  outputFile << "D=0\n";

  outputFile << aCommandStart_ << currentCommonInstructions << '\n';
  outputFile << "0;" << "JMP\n";

  // When condition is true, we jump to here.
  outputFile << "(" + currentTrueLable + ")" << '\n';
  outputFile << "D=-1\n";

  outputFile << "(" + currentCommonInstructions + ")" << '\n';
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::eq_()
{
  doComparison_("EQUAL_", "COMMON_INSTRUCTIONS_EQ_", std::to_string(eqLabelCounter_++), "JEQ");
}

void ArithmeticHelper::gt_()
{
  doComparison_("GRATER_", "COMMON_INSTRUCTIONS_GT_", std::to_string(gtLabelCounter_++), "JGT");
}

void ArithmeticHelper::lt_()
{
  doComparison_("LESSER_", "COMMON_INSTRUCTIONS_LT_", std::to_string(ltLabelCounter_++), "JLT");
}

void ArithmeticHelper::and_()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("&");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::or_()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("|");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArithmeticHelper::not_()
{
  spMinusMinus_();
  assignStackPointerToD_();

  auto &outputFile = getOutputFile_();
  outputFile << "D=!D\n";

  assignDToStackPointer_();
  spPlusPlus_();
}