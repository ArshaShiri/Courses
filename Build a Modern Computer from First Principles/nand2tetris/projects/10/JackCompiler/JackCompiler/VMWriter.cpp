#include "VMWriter.h"

/* Create output file and prepare it for writing.*/
VMWriter::VMWriter(const std::string &otputFilepath)
{
  outputFile_.open(otputFilepath);
}

void VMWriter::writePush(MemorySegment memorySegment, int index)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writePop(MemorySegment memorySegment, int index)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeArithmetic(ArithmeticCommand command)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeLabel(const std::string &label)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeGoTo(const std::string &label)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeIf(const std::string &label)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeCall(std::string &name, int numberOfArgs)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeFunction(std::string &name, int numberOfLocals)
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::writeReturn()
{
  throw std::runtime_error(__FUNCTION__  " is not implemented");
}

void VMWriter::close()
{
  outputFile_.close();
}
