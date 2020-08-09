#pragma once

#include <fstream>
#include <string>

class Parser;

class Translator
{
public:
  Translator(const std::string &fileName);
  ~Translator();

  Translator(const Translator &) = delete;
  Translator& operator=(const Translator&) = delete;

  Translator(Translator &&otherTranslator);
  Translator& operator=(Translator &&otherTranslator);
 
  void translate();

private:
  // private attributes
  std::ofstream outputFile_;
  std::string fileName_;

  // private methods
  void write_(const Parser &parser);
  void writeArithmetic_(const Parser &parser);
  void writePushPop_(const Parser &parser);
};

