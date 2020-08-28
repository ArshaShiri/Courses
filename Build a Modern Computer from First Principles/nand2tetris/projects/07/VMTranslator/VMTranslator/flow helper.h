#pragma once

#include "translator helper.h"

class FlowHelper : public TranslatorHelper
{
public:
  FlowHelper(const Parser &parser,
             const std::string &fileNameWithNoExtension,
             std::ofstream &outputFile);

  void write();

private:
  // Private methods.
  void lable_();
  void goto_();
  void ifGoto_();
};

