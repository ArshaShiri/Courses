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

// ************************************ PushPopHelper ************************************ //

const std::string PushPopHelper::LCLRegister_ = "R1";
const std::string PushPopHelper::ARGRegister_ = "R2";
const std::string PushPopHelper::THISRegister_ = "R3";
const std::string PushPopHelper::THATRegister_ = "R4";
const std::string PushPopHelper::TEMPRegister_ = "R5";

PushPopHelper::PushPopHelper(
  const Parser &parser,
  const std::string &fileNameWithNoExtension,
  std::ofstream &outputFile) : TranslatorHelper{parser, fileNameWithNoExtension, outputFile}
{}

void PushPopHelper::pushConstant_()
{
  assignArg2ToD_();
  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::pushStatic_()
{
  const auto &fileNameWithNoExtension = getFileNameWithNoExtension_();
  const auto varName = fileNameWithNoExtension + '.' + std::to_string(getParser_().getArg2());

  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << varName << '\n';
  outputFile << "D=M\n";

  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::pushPointer_()
{
  auto &outputFile = getOutputFile_();
  const auto segmentIndex = getParser_().getArg2();

  auto thisThatPush = [&](const std::string &thisOrThatRegister)
  {
    outputFile << aCommandStart_ << thisOrThatRegister << '\n';
    outputFile << "D=M\n";
    assignDToStackPointer_();
    spPlusPlus_();
  };

  // Accessing THAT
  if (segmentIndex == 1)
    thisThatPush(THATRegister_);

  // Accessing THIS
  else if (segmentIndex == 0)
    thisThatPush(THISRegister_);

  else
    std::cout << "Pointer segment index is not supported!\n";
}

void PushPopHelper::doPush_(const std::string &baseAddressRegister)
{
  auto &outputFile = getOutputFile_();

  // Calculating the address relative to the base pointer.
  outputFile << aCommandStart_ << baseAddressRegister << '\n';

  if (baseAddressRegister == TEMPRegister_)
    outputFile << "D=A\n";
  else
    outputFile << "D=M\n";

  outputFile << aCommandStart_ + std::to_string(getParser_().getArg2()) + '\n';
  outputFile << "A=A+D\n";

  // *sp = *add
  outputFile << "D=M\n";
  assignDToStackPointer_();
  spPlusPlus_();
}

void PushPopHelper::push()
{
  const auto &segment = getParser_().getArg1();

  if (segment == "constant")
  {
    pushConstant_();
    return;
  }
  if (segment == "local")
  {
    doPush_(LCLRegister_);
    return;
  }
  if (segment == "argument")
  {
    doPush_(ARGRegister_);
    return;
  }
  if (segment == "this")
  {
    doPush_(THISRegister_);
    return;
  }
  if (segment == "that")
  {
    doPush_(THATRegister_);
    return;
  }
  if (segment == "static")
  {
    pushStatic_();
    return;
  }
  if (segment == "temp")
  {
    doPush_(TEMPRegister_);
    return;
  }
  if (segment == "pointer")
  {
    pushPointer_();
    return;
  }

  std::cout << "The given segment cannot be pushed to!\n";
}

void PushPopHelper::popPointer_()
{
  auto &outputFile = getOutputFile_();
  const auto segmentIndex = getParser_().getArg2();

  auto thisThatPOP = [&](const std::string &thisOrThatRegister)
  {
    spMinusMinus_();
    assignStackPointerToD_();
    outputFile << aCommandStart_ << thisOrThatRegister << '\n';
    outputFile << "M=D\n";
  };

  // Accessing THAT
  if (segmentIndex == 1)
    thisThatPOP(THATRegister_);

  // Accessing THIS
  else if (segmentIndex == 0)
    thisThatPOP(THISRegister_);
  
  else
    std::cout << "Pointer segment index is not supported!\n";
}

void PushPopHelper::popStatic_()
{
  spMinusMinus_();
  assignStackPointerToD_();

  const auto &fileNameWithNoExtension = getFileNameWithNoExtension_();
  const auto varName = fileNameWithNoExtension + '.' + std::to_string(getParser_().getArg2());

  auto &outputFile = getOutputFile_();
  outputFile << aCommandStart_ << varName << '\n';
  outputFile << "M=D\n";
}

void PushPopHelper::doPop_(const std::string &baseAddressRegister)
{
  auto &outputFile = getOutputFile_();

  // Calculating the address relative to the base pointer.
  outputFile << aCommandStart_ << baseAddressRegister << '\n';

  if (baseAddressRegister == TEMPRegister_)
    outputFile << "D=A\n";
  else
    outputFile << "D=M\n";

  outputFile << aCommandStart_ + std::to_string(getParser_().getArg2()) + '\n';
  outputFile << "D=A+D\n";
  outputFile << aCommandStart_ + helperRegister14 + '\n';
  outputFile << "M=D\n";

  spMinusMinus_();
  assignStackPointerToD_();
  outputFile << aCommandStart_ + helperRegister14 + '\n';
  outputFile << "A=M\n";
  outputFile << "M=D\n";
}

void PushPopHelper::pop()
{
  const auto &segment = getParser_().getArg1();

  if (segment == "local")
  {
    doPop_(LCLRegister_);
    return;
  }
  if (segment == "argument")
  {
    doPop_(ARGRegister_);
    return;
  }
  if (segment == "this")
  {
    doPop_(THISRegister_);
    return;
  }
  if (segment == "that")
  {
    doPop_(THATRegister_);
    return;
  }
  if (segment == "static")
  {
    popStatic_();
    return;
  }
  if (segment == "temp")
  {
    doPop_(TEMPRegister_);
    return;
  }
  if (segment == "pointer")
  {
    popPointer_();
    return;
  }

  std::cout << "The given segment cannot be popped to!\n";
}

// ************************************ ArtihmeticTranslatorHelper ************************************ //

ArtihmeticTranslatorHelper::ArtihmeticTranslatorHelper(
  const Parser &parser,
  const std::string &fileNameWithNoExtension,
  std::ofstream &outputFile) : TranslatorHelper{parser, fileNameWithNoExtension, outputFile},
                               eqLableCounter_{0},
                               gtLableCounter_{0},
                               ltLableCounter_{0}
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

void ArtihmeticTranslatorHelper::operateOnTwoTopValuesInStackAndStoreResultInD_(std::string operatorType)
{
  spMinusMinus_();
  assignStackPointerToD_();
  spMinusMinus_();

  auto &outputFile = getOutputFile_();
  outputFile << "A=M\n";

  outputFile << "D=M" << operatorType << "D\n";
}

void ArtihmeticTranslatorHelper::doAdd()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("+");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArtihmeticTranslatorHelper::doSub()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("-");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArtihmeticTranslatorHelper::doNeg()
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

void ArtihmeticTranslatorHelper::doComparison_(
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

void ArtihmeticTranslatorHelper::doEq()
{
  doComparison_("EQUAL_", "COMMON_INSTRUCTIONS_EQ_", std::to_string(eqLableCounter_++), "JEQ");
}

void ArtihmeticTranslatorHelper::doGt()
{
  doComparison_("GRATER_", "COMMON_INSTRUCTIONS_GT_", std::to_string(gtLableCounter_++), "JGT");
}

void ArtihmeticTranslatorHelper::doLt()
{
  doComparison_("LESSER_", "COMMON_INSTRUCTIONS_LT_", std::to_string(ltLableCounter_++), "JLT");
}

void ArtihmeticTranslatorHelper::doAnd()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("&");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArtihmeticTranslatorHelper::doOr()
{
  operateOnTwoTopValuesInStackAndStoreResultInD_("|");
  assignDToStackPointer_();
  spPlusPlus_();
}

void ArtihmeticTranslatorHelper::doNot()
{
  spMinusMinus_();
  assignStackPointerToD_();

  auto &outputFile = getOutputFile_();
  outputFile << "D=!D\n";

  assignDToStackPointer_();
  spPlusPlus_();
}