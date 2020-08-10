#include "translator helper.h"

const std::string TranslatorHelper::aCommandStart_ = {"@"};
const std::string TranslatorHelper::stackPointerRegister_ = {"R0"};

TranslatorHelper::TranslatorHelper(const Parser &parser, std::ofstream &outputFile) :
  parser_{parser}, outputFile_{outputFile}
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

// ************************************ PushPopHelper ************************************ //

PushPopHelper::PushPopHelper(const Parser &parser, std::ofstream &outputFile) :
  TranslatorHelper{parser, outputFile}
{}

void PushPopHelper::pushConstant()
{
  assignArg2ToD_();
  assignDToStackPointer_();
  spPlusPlus_();
}

// ************************************ ArtihmeticTranslatorHelper ************************************ //

ArtihmeticTranslatorHelper::ArtihmeticTranslatorHelper(const Parser &parser, std::ofstream &outputFile) :
  TranslatorHelper{parser, outputFile}
{}

// This is useful for arithmetic commands.
void ArtihmeticTranslatorHelper::popToRegister_(const std::string &regsiter)
{
  spMinusMinus_();
  assignStackPointerToD_();
  auto &outputFile = getOutputFile_();

  // store value of D in the given register
  outputFile << aCommandStart_ + regsiter + '\n';
  outputFile << "M=D\n";
}

void ArtihmeticTranslatorHelper::add()
{
  popToRegister_("R14");
  spMinusMinus_();
  assignStackPointerToD_();

  auto &outputFile = getOutputFile_();

  // store value of D in the given register
  outputFile << aCommandStart_ + "R14" << '\n';
  outputFile << "M=D+M\n";
  outputFile << "D=M\n";

  assignDToStackPointer_();
  spPlusPlus_();
}