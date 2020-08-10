#pragma once

#include<fstream>
#include <string>

#include "parser.h"

class TranslatorHelper
{
public:
  TranslatorHelper(const Parser &parser, std::ofstream &outputFile);

protected:
  void spMinusMinus_();
  void spPlusPlus_();
  void assignArg2ToD_();
  void assignDToStackPointer_();
  void assignStackPointerToD_();

  std::ofstream& getOutputFile_();

  static const std::string aCommandStart_;
  static const std::string stackPointerRegister_;

private:
  std::ofstream &outputFile_;
  const Parser &parser_;
};

class PushPopHelper : public TranslatorHelper
{
public:
  PushPopHelper(const Parser &parser, std::ofstream &outputFile);
  void pushConstant();
};

class ArtihmeticTranslatorHelper : public TranslatorHelper
{
public:
  ArtihmeticTranslatorHelper(const Parser &parser, std::ofstream &outputFile);
  void add();

private:
  void popToRegister_(const std::string &regsiter);
};