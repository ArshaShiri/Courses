#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Instruction
{
  AInstruction,
  CInstruction,
  Unknown
};

class Assembler
{
public:
  Assembler(const std::string &fileName);
  void translate();
  void saveFile();
  void writeProgramLinesToAfile();

private:
  // Private methods
  void readAndStoreProgramLines_(std::ifstream &inputFile);
  Instruction getInstructionType(const std::string &line);
  void processVariablesAndSymbols(std::unordered_map<std::string, std::string> &variablesMap);

  // Private attributes
  std::vector<std::string> programLines_;
  std::vector<std::string> machineCodeLines_;
  std::string fileName_;
};