#pragma once

#include <fstream>
#include <string>

enum class MemorySegment
{
  CONST,
  ARG,
  LOCAL,
  STATIC,
  THIS,
  THAT,
  POINTER,
  TEMPT 
};

enum class ArithmeticCommand
{
  ADD,
  SUB,
  NEG,
  EQ,
  GT,
  LT,
  AND,
  OR,
  NOT
};

class VMWriter
{
public:
  VMWriter(const std::string &otputFilepath);

  void writePush(MemorySegment memorySegment, int index);
  void writePop(MemorySegment memorySegment, int index);
  void writeArithmetic(ArithmeticCommand command);
  void writeLabel(const std::string &label);
  void writeGoTo(const std::string &label);

  void writeIf(const std::string &label);
  void writeCall(const std::string &name, int numberOfArgs);
  void writeFunction(const std::string &name, int numberOfLocals);
  void writeReturn();
  void close();

private:
  std::ofstream outputFile_;
};

