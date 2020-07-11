#include <unordered_map>

#include "CInstruction.h"

namespace
{
  class CInstructionParser
  {
  public:
    // Translation of a c instruction is like:
    // 1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
    CInstructionParser(const std::string &cInstruction);
    std::string getTranslation() const;

  private:
    // Private methods.
    void buildCompToBinMap_();
    void buildDestToBinMap_();
    void buildJumpToBinMap_();
    void translateCompToBin_(std::string &binary) const;
    void translateDestToBin_(std::string &binary) const;
    void translateJumpToBin_(std::string &binary) const;

    std::string getCompPart_() const;
    std::string getDestPart_() const;
    std::string getJumpPart_() const;
    
    // Private attributes.
    static std::unordered_map<std::string, std::string> compToBinAEqualOne_;
    static std::unordered_map<std::string, std::string> compToBinAEqualZero_;
    static std::unordered_map<std::string, std::string> destToBin_;
    static std::unordered_map<std::string, std::string> jumpToBin_;
    size_t equalPosition_;
    size_t semicolonPosition_;
    const std::string &cInstruction_;
  };
}  // End of unnamed namespace declaration.

std::string CInstruction::translate(const std::string &CInstruction)
{
  // The machine code corresponding to a C instruction always starts with 111.
  const auto startCode = "111";
  const auto cInstructionParser = CInstructionParser{CInstruction};
  auto translation = std::string{};
  translation.append(startCode);

  return translation.append(cInstructionParser.getTranslation());
}

namespace
{
  CInstructionParser::CInstructionParser(const std::string &CInstruction) : cInstruction_{CInstruction}
  {
    // dest = comp; jump 
    equalPosition_ = CInstruction.find("=");
    semicolonPosition_ = CInstruction.find(";");

    if (compToBinAEqualOne_.empty() || compToBinAEqualZero_.empty()) buildCompToBinMap_();
    if (destToBin_.empty()) buildDestToBinMap_();
    if (jumpToBin_.empty()) buildJumpToBinMap_();
  }

  std::string CInstructionParser::getTranslation() const
  {
    auto translation = std::string{};
    translateCompToBin_(translation);
    translateDestToBin_(translation);
    translateJumpToBin_(translation);

    return translation;
  }

  // dest = comp; jump 
  std::string CInstructionParser::getCompPart_() const
  {
    if ((equalPosition_ == cInstruction_.npos) && (semicolonPosition_ == cInstruction_.npos))
      return cInstruction_;

    else if ((equalPosition_ != cInstruction_.npos) && (semicolonPosition_ == cInstruction_.npos))
      return cInstruction_.substr(equalPosition_ + 1);

    else if ((equalPosition_ == cInstruction_.npos) && (semicolonPosition_ != cInstruction_.npos))
      return cInstruction_.substr(0, semicolonPosition_);

    else
      return cInstruction_.substr(equalPosition_, semicolonPosition_);
  }

  std::string CInstructionParser::getDestPart_() const
  {
    if (equalPosition_ != cInstruction_.npos)
      return cInstruction_.substr(0, equalPosition_);

    else return "";
  }

  std::string CInstructionParser::getJumpPart_() const
  {
    if (semicolonPosition_ != cInstruction_.npos)
      return cInstruction_.substr(semicolonPosition_ + 1);

    else return "";
  }

  void CInstructionParser::translateCompToBin_(std::string &binary) const
  {
    const auto compString = getCompPart_();
    auto translate = compToBinAEqualZero_.find(compString);

    if (translate != compToBinAEqualZero_.end())
    {
      binary.append("0");
      binary.append(std::move(translate->second));
      return;
    }

    translate = compToBinAEqualOne_.find(compString);
    if (translate != compToBinAEqualOne_.end())
    {
      binary.append("1");
      binary.append(std::move(translate->second));
      return;
    }
  }

  void CInstructionParser::translateDestToBin_(std::string &binary) const
  {
    const auto destString = getDestPart_();
    auto translate = destToBin_.find(destString);
    if (translate != destToBin_.end())
    {
      binary.append(std::move(translate->second));
      return;
    }

    const auto nullDestValue = "000";
    binary.append(nullDestValue);
  }

  void CInstructionParser::translateJumpToBin_(std::string &binary) const
  {
    const auto jumpString = getJumpPart_();
    auto translate = jumpToBin_.find(jumpString);
    if (translate != jumpToBin_.end())
    {
      binary.append(std::move(translate->second));
      return;
    }

    const auto nullDestValue = "000";
    binary.append(nullDestValue);
  }

  std::unordered_map<std::string, std::string> CInstructionParser::compToBinAEqualOne_;
  std::unordered_map<std::string, std::string> CInstructionParser::compToBinAEqualZero_;
  std::unordered_map<std::string, std::string> CInstructionParser::destToBin_;
  std::unordered_map<std::string, std::string> CInstructionParser::jumpToBin_;

  void CInstructionParser::buildCompToBinMap_()
  {
    compToBinAEqualZero_.emplace("0", "101010");
    compToBinAEqualZero_.emplace("1", "111111");
    compToBinAEqualZero_.emplace("-1", "111010");

    compToBinAEqualZero_.emplace("D", "001100");
    compToBinAEqualZero_.emplace("A", "110000");
    compToBinAEqualZero_.emplace("!D", "001101");

    compToBinAEqualZero_.emplace("!A", "110001");
    compToBinAEqualZero_.emplace("-D", "001111");
    compToBinAEqualZero_.emplace("-A", "110011");

    compToBinAEqualZero_.emplace("D+1", "011111");
    compToBinAEqualZero_.emplace("A+1", "110111");
    compToBinAEqualZero_.emplace("D-1", "001110");

    compToBinAEqualZero_.emplace("A-1", "110010");
    compToBinAEqualZero_.emplace("D+A", "000010");
    compToBinAEqualZero_.emplace("D-A", "010011");

    compToBinAEqualZero_.emplace("A-D", "000111");
    compToBinAEqualZero_.emplace("D&A", "000000");
    compToBinAEqualZero_.emplace("D|A", "010101");


    compToBinAEqualOne_.emplace("M", "110000");
    compToBinAEqualOne_.emplace("!M", "110001");

    compToBinAEqualOne_.emplace("-M", "110011");
    compToBinAEqualOne_.emplace("M+1", "110111");

    compToBinAEqualOne_.emplace("M-1", "110010");
    compToBinAEqualOne_.emplace("D+M", "000010");

    compToBinAEqualOne_.emplace("D-M", "010011");
    compToBinAEqualOne_.emplace("M-D", "000111");

    compToBinAEqualOne_.emplace("D&M", "000000");
    compToBinAEqualOne_.emplace("D|M", "010101");
  }

  void CInstructionParser::buildDestToBinMap_()
  {
    destToBin_.emplace("M", "001");
    destToBin_.emplace("D", "010");
    destToBin_.emplace("MD", "011");
    destToBin_.emplace("A", "100");
    destToBin_.emplace("AM", "101");
    destToBin_.emplace("AD", "110");
    destToBin_.emplace("AMD", "111");
  }

  void CInstructionParser::buildJumpToBinMap_()
  {
    jumpToBin_.emplace("JGT", "001");
    jumpToBin_.emplace("JEQ", "010");
    jumpToBin_.emplace("JGE", "011");
    jumpToBin_.emplace("JLT", "100");
    jumpToBin_.emplace("JNE", "101");
    jumpToBin_.emplace("JLE", "110");
    jumpToBin_.emplace("JMP", "111");
  }
}  // End of unnamed namespace definition.