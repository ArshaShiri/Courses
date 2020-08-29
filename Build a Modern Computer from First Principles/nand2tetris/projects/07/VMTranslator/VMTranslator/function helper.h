#pragma once

#include "translator helper.h"

class FunctionHelper : public TranslatorHelper
{
public:
  FunctionHelper(const Parser &parser,
                 const std::string &fileNameWithNoExtension,
                 std::ofstream &outputFile);

  void write();

private:
  // Private methods.
  void pushToStack_(const std::string &argumentToPush);
  void function_();
  void call_();
  void return_();
};

