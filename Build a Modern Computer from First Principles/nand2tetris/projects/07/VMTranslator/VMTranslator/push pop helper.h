#pragma once

#include "translator helper.h"

class PushPopHelper : public TranslatorHelper
{
public:
  PushPopHelper(const Parser &parser,
                std::ofstream &outputFile);
  void setFileNameWithNoExtension(const std::string &fileNameWithNoExtension);
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
  const std::string helperRegister14 = "R14";
  std::string fileNameWithNoExtension_;
};