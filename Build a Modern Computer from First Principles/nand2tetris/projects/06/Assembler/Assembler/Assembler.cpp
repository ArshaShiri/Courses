#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <sstream>

#include "Assembler.h"
#include "AInstruction.h"
#include "CInstruction.h"

namespace
{
  void removeWhiteSpaces(std::string &input);
  void removeCommentsAndGetNewString(std::string &string);
  void insertReservedVariables(std::unordered_map<std::string, std::string> &variablesMap);
  bool is_number(const std::string &s);
}  // End of unnamed namespace declaration.

Assembler::Assembler(const std::string &fileName) : fileName_{fileName}
{
  const auto inputFileExtention = ".asm";
  auto inputFile = std::ifstream(fileName_ + inputFileExtention);

  if (!inputFile) 
  {
    std::cout << "Cannot open input file.\n";
    return;
  }

  readAndStoreProgramLines_(inputFile);
}

void Assembler::readAndStoreProgramLines_(std::ifstream &inputFile)
{ 
  auto lineNumber = size_t{0};
  auto currentLine = std::string{};

  std::unordered_map<std::string, std::string> variablesMap;
  insertReservedVariables(variablesMap);

  while (std::getline(inputFile, currentLine))
  {
    removeCommentsAndGetNewString(currentLine);
    removeWhiteSpaces(currentLine);
    if (currentLine.empty()) continue;

    // store the line next to the label symbols
    const auto openParenthesisLocaton = currentLine.find("(");
    if (openParenthesisLocaton != currentLine.npos)
    {
      const auto lableName = currentLine.substr(openParenthesisLocaton + 1, currentLine.size() - 2);
      variablesMap.emplace(lableName, std::to_string(lineNumber));
      continue;
    }

    programLines_.emplace_back(currentLine);
    lineNumber++;
  }

  processVariablesAndSymbols(variablesMap);
}

Instruction Assembler::getInstructionType(const std::string &line)
{
  const auto startOfATypeCommand = "@";
  const auto startPositionOfATypeCommand = line.find(startOfATypeCommand);

  if (startPositionOfATypeCommand != line.npos) return Instruction::AInstruction;
  else return Instruction::CInstruction;
}

void Assembler::translate()
{
  auto lineNumber = size_t{1};

  for (const auto &line : programLines_)
  {
    const auto commandType = getInstructionType(line);
    switch (commandType)
    {
      case Instruction::AInstruction:
      {
        machineCodeLines_.emplace_back(AInstruction::translate(line));
        break;
      }
      case Instruction::CInstruction:
      {
        machineCodeLines_.emplace_back(CInstruction::translate(line));
        break;
      }
      default:
      {
        std::cout << "Command in line " << lineNumber  << " is not supported!\n";
      }
      lineNumber++;
    }
  }
}

void Assembler::saveFile()
{
  const auto outputFileExtention = ".hack";
  auto output = std::ofstream{};
  output.open(fileName_ + outputFileExtention);
  
  for (const auto machineCode : machineCodeLines_)
  {
    output << machineCode << '\n';
  }
  output.close();
}

void Assembler::writeProgramLinesToAfile()
{
  // Write the program line after all the white spaces and comments are removed and variable and
  // symbols are replaced with a corresponding number.
  const auto outputFileExtention = "MyL.asm";
  std::ofstream output;
  output.open(fileName_ + outputFileExtention);

  for (const auto programLine : programLines_)
  {
    output << programLine << '\n';
  }
  output.close();
}

void Assembler::processVariablesAndSymbols(std::unordered_map<std::string, std::string> &variablesMap)
{
  // According to the specification, variable numberings start from 16.
  auto variableNumber = size_t{16};

  for (auto &line : programLines_)
  {
    if (getInstructionType(line) == Instruction::AInstruction)
    {
      const auto startOfATypeCommand = "@";
      const auto startPositionOfATypeCommand = line.find(startOfATypeCommand);
      const auto symbolOrVariable = line.substr(startPositionOfATypeCommand + 1);

      // In case there is a number after @, we do not need to do anything at this stage.
      if (is_number(symbolOrVariable))
        continue;

      const auto symbolOrVariableTranslation = variablesMap.find(symbolOrVariable);
      auto processedVar = std::string{};
      if (symbolOrVariableTranslation != variablesMap.end())
        processedVar = symbolOrVariableTranslation->second;
      else
      {
        processedVar = std::to_string(variableNumber);
        variablesMap.emplace(symbolOrVariable, processedVar);
        variableNumber++;
      }

      line = startOfATypeCommand + processedVar;
    }
  }
}

namespace
{
  void removeCommentsAndGetNewString(std::string &string)
  {
    const auto commentStart = "//";
    const auto commentStartingPosition = string.find(commentStart);

    // No comment in this string.
    if (commentStartingPosition == string.npos) return;
    
    // The entire string is a comment.
    if (commentStartingPosition == 0)
    {
      string.clear();
      return;
    }

    // Remove the comment.
    string.erase(commentStartingPosition);
  }

  void removeWhiteSpaces(std::string &input)
  {
    input.erase(
      std::remove_if(input.begin(),
        input.end(),
        [](char &c) {return std::isspace<char>(c, std::locale::classic()); }),
      input.end());
  }

  void insertReservedVariables(std::unordered_map<std::string, std::string> &variablesMap)
  {
    variablesMap.emplace("SP", "0");
    variablesMap.emplace("LCL", "1");
    variablesMap.emplace("ARG", "2");
    variablesMap.emplace("THIS", "3");
    variablesMap.emplace("THAT", "4");

    variablesMap.emplace("R0", "0");
    variablesMap.emplace("R1", "1");
    variablesMap.emplace("R2", "2");
    variablesMap.emplace("R3", "3");
    variablesMap.emplace("R4", "4");
    variablesMap.emplace("R5", "5");
    variablesMap.emplace("R6", "6");
    variablesMap.emplace("R7", "7");
    variablesMap.emplace("R8", "8");
    variablesMap.emplace("R9", "9");
    variablesMap.emplace("R10", "10");
    variablesMap.emplace("R11", "11");
    variablesMap.emplace("R12", "12");
    variablesMap.emplace("R13", "13");
    variablesMap.emplace("R14", "14");
    variablesMap.emplace("R15", "15");

    variablesMap.emplace("SCREEN", "16384");
    variablesMap.emplace("KBD", "24576");
  }

  bool is_number(const std::string& s)
  {
    return !s.empty() && std::find_if(s.begin(),
      s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
  }
}  // End of unnamed namespace definition.