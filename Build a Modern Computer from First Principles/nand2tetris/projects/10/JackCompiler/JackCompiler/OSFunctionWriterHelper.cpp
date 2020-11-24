#include "OSFunctionWriterHelper.h"

const std::string OSMemoryWriterHelper::name_ = "Memory.";

std::string OSMemoryWriterHelper::allocName()
{
  return name_ + "alloc";
}

int OSMemoryWriterHelper::allocnumberOfArgs()
{
  return 1;
}

const std::string OSMathWriterHelper::name_ = "Math.";

std::string OSMathWriterHelper::multName()
{
  return name_ + "multiply";
}

int OSMathWriterHelper::multnumberOfArgs()
{
  return 2;
}

std::string OSMathWriterHelper::divName()
{
  return name_ + "divide";
}

int OSMathWriterHelper::divnumberOfArgs()
{
  return 2;
}

const std::string OSStringWriterHelper::name_ = "String.";

std::string OSStringWriterHelper::stringNew()
{
  return name_ + "new";
}

int OSStringWriterHelper::stringNewNumberOfArgs()
{
  return 1;
}

std::string OSStringWriterHelper::stringAppendChar()
{
  return name_ + "appendChar";
}

int OSStringWriterHelper::stringAppendCharNumberOfArgs()
{
  return 2;
}