// There are number of functions which are provided by the host operating system. The classes here 
// help facilitate calling those functions and return their names.

#pragma once

#include <string>

class OSMemoryWriterHelper
{
public:
  static std::string allocName();
  static int allocnumberOfArgs();

private:
  static const std::string name_;
};

class OSMathWriterHelper
{
public:
  static std::string multName();
  static int multnumberOfArgs();
  static std::string divName();
  static int divnumberOfArgs();

private:
  static const std::string name_;
};

class OSStringWriterHelper
{
public:
  static std::string stringNew();
  static int stringNewNumberOfArgs();
  static std::string stringAppendChar();
  static int stringAppendCharNumberOfArgs();

private:
  static const std::string name_;
};
