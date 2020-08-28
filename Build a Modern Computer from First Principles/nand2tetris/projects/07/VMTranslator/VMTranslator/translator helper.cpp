#include <iostream>

#include "translator helper.h"

const std::string TranslatorHelper::aCommandStart_ = {"@"};
const std::string TranslatorHelper::stackPointerRegister_ = {"R0"};

TranslatorHelper::TranslatorHelper(
  const Parser &parser,
  const std::string &fileNameWithNoExtension,
  std::ofstream &outputFile) : parser_{parser},
                               outputFile_{outputFile},
                               fileNameWithNoExtension_{fileNameWithNoExtension}
{}

// Increment the stack pointer.
// sp++
void TranslatorHelper::spPlusPlus_()
{
  outputFile_ << aCommandStart_ << stackPointerRegister_ << '\n';
  outputFile_ << "M=M+1\n";
}

// Decrement the stack pointer.
// sp--
void TranslatorHelper::spMinusMinus_()
{
  outputFile_ << aCommandStart_ << stackPointerRegister_ << '\n';
  outputFile_ << "M=M-1\n";
}

// Assign the value of arg2 to the D register.
// D = arg2
void TranslatorHelper::assignArg2ToD_()
{
  outputFile_ << aCommandStart_ << std::to_string(parser_.getArg2()) << '\n';
  outputFile_ << "D=A\n";
}

// Assign the value of D register to where the stack pointer is pointing to.
// *sp = D
void TranslatorHelper::assignDToStackPointer_()
{
  outputFile_ << aCommandStart_ << stackPointerRegister_ << '\n';
  outputFile_ << "A=M\n";
  outputFile_ << "M=D\n";
}

// Assign the value pointed by stack pointer to D register.
// D = *sp
void TranslatorHelper::assignStackPointerToD_()
{
  outputFile_ << "A=M\n";
  outputFile_ << "D=M\n";
}

std::ofstream& TranslatorHelper::getOutputFile_()
{
  return outputFile_;
}

const std::string& TranslatorHelper::getFileNameWithNoExtension_() const
{
  return fileNameWithNoExtension_;
}

const Parser& TranslatorHelper::getParser_() const
{
  return parser_;
}