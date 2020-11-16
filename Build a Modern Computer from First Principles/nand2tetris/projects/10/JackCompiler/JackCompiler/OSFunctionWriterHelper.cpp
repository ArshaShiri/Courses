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