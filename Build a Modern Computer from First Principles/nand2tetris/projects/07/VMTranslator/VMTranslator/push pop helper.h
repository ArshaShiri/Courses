#pragma once

#include "translator helper.h"

class PushPopHelper : public TranslatorHelper
{
public:
  PushPopHelper(const Parser &parser,
                const std::string &fileNameWithNoExtension,
                std::ofstream &outputFile);
  void write();

private:
  // Private methods.
  void push_();
  void pop_();
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