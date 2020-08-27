#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "translator helper.h"

class Parser;

class Translator
{
public:
  Translator(const std::string &filePath);
  ~Translator();

  Translator(const Translator &) = delete;
  Translator& operator=(const Translator&) = delete;

  Translator(Translator &&otherTranslator);
  Translator& operator=(Translator &&otherTranslator);
 
  void translate();

private:
  // private methods
  void write_(const Parser &parser);
  void writeArithmetic_(const Parser &parser);
  void writePushPop_(const Parser &parser);

  // private attributes
  std::ofstream outputFile_;
  std::string filePath_;
  std::string fileNameWithNoExtension_;
  std::unique_ptr<ArtihmeticTranslatorHelper> pArtihmeticTranslatorHelper_;
  std::unique_ptr <PushPopHelper> pPushPopHelper_;
};

