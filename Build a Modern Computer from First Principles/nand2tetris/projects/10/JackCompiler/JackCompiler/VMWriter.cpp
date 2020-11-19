#include "VMWriter.h"

std::string getMemorySegmentName(MemorySegment memorySegment)
{
  switch (memorySegment)
  {
  case MemorySegment::CONST: return "constant";
  case MemorySegment::ARG: return "argument";
  case MemorySegment::LOCAL: return "local";
  case MemorySegment::STATIC: return "static";
  case MemorySegment::THIS: return "this";
  case MemorySegment::THAT: return "that";
  case MemorySegment::POINTER: return "pointer";
  case MemorySegment::TEMPT: return "temp";

  default: throw std::runtime_error("Memory segment is not recognized in " __FUNCTION__);
  }
}

/* Create output file and prepare it for writing.*/
VMWriter::VMWriter(const std::string &otputFilepath)
{
  outputFile_.open(otputFilepath);
}

void VMWriter::close()
{
  outputFile_.close();
}

void VMWriter::writePush(MemorySegment memorySegment, int index)
{
  outputFile_ << "push " + getMemorySegmentName(memorySegment) + " " + std::to_string(index) << '\n';
}

void VMWriter::writePop(MemorySegment memorySegment, int index)
{
  outputFile_ << "pop " + getMemorySegmentName(memorySegment) + " " + std::to_string(index) << '\n';
}

void VMWriter::writeArithmetic(ArithmeticCommand command)
{
  switch (command)
  {
  case ArithmeticCommand::ADD:
    outputFile_ << "add" << '\n'; break;
  case ArithmeticCommand::SUB:
    outputFile_ << "sub" << '\n'; break;
  case ArithmeticCommand::NEG:
    outputFile_ << "neg" << '\n'; break;
  case ArithmeticCommand::EQ:
    outputFile_ << "eq" << '\n'; break;
  case ArithmeticCommand::GT:
    outputFile_ << "gt" << '\n'; break;
  case ArithmeticCommand::LT:
    throw std::runtime_error("Arithmetic operator is not implemented.");
  case ArithmeticCommand::AND:
    outputFile_ << "and" << '\n'; break;
  case ArithmeticCommand::OR:
    throw std::runtime_error("Arithmetic operator is not implemented.");
  case ArithmeticCommand::NOT:
    outputFile_ << "not" << '\n'; break;
  default:
    throw std::runtime_error("Arithmetic operator is not supported."); break;
  }
}

void VMWriter::writeLabel(const std::string &label)
{
  outputFile_ << "label " + label << '\n';
}

void VMWriter::writeGoTo(const std::string &label)
{
  outputFile_ << "goto " + label << '\n';
}

void VMWriter::writeIf(const std::string &label)
{
  outputFile_ << "if-goto " + label << '\n';
}

void VMWriter::writeCall(const std::string &name, int numberOfArgs)
{
  outputFile_ << "call " + name + " " + std::to_string(numberOfArgs) << '\n';
}

void VMWriter::writeFunction(const std::string &name, int numberOfLocals)
{
  outputFile_ << "function " + name + " " + std::to_string(numberOfLocals) << '\n';
}

void VMWriter::writeReturn()
{
  outputFile_ << "return" << '\n';
}