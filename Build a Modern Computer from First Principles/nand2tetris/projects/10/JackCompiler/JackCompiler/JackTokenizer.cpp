#include <fstream>
#include <iterator>
#include <sstream>

#include "JackTokenizer.h"
#include "TokenFactory.h"

namespace
{
void removeComments(std::string &string);
} // end of namespace declaration

JackTokenizer::JackTokenizer(const std::string &inputFilepath) : 
  currentLineNumber_{0}, currentTokenNumberInLine_{0}
{
  auto inputFile = std::ifstream(inputFilepath);

  if (!inputFile)
    throw std::runtime_error("Could not open file" + inputFilepath);

  storeProgram_(inputFile);
  hasMoreTokens_ = !program_.empty();
}

bool JackTokenizer::hasMoreTokens() const
{
  return hasMoreTokens_;
}

void JackTokenizer::advance()
{
  if (currentLineNumber_ < program_.size())
  {
    if ((stringTokensInCurrentLine_.empty()) ||
        (currentTokenNumberInLine_ == stringTokensInCurrentLine_.size()))
      setStringTokensInNextLine_();
  }

  if (currentTokenNumberInLine_ < stringTokensInCurrentLine_.size())
  {
    auto tokenFactory = TokenFactory{stringTokensInCurrentLine_.at(currentTokenNumberInLine_)};
    pToken_ = tokenFactory.tokenize();
    ++currentTokenNumberInLine_;
  }
}

void JackTokenizer::setStringTokensInNextLine_()
{
  const auto &currentLine = program_.at(currentLineNumber_);
  std::istringstream iss(currentLine);

  stringTokensInCurrentLine_.clear();

  for (auto it = std::istream_iterator<std::string>{iss}; it !=
       std::istream_iterator<std::string>{};
       ++it)
    stringTokensInCurrentLine_.emplace_back((*it));

  currentTokenNumberInLine_ = 0;
  currentLineNumber_++;
}

void JackTokenizer::storeProgram_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};

  while (std::getline(inputFile, currentLine))
  {
    removeComments(currentLine);
    if (currentLine.empty()) continue;

    program_.emplace_back(currentLine);
  }
}

const Token* JackTokenizer::getCurrentToken() const
{
  return pToken_.get();
}

namespace
{
void removeComments(std::string &string)
{
  // This function is limited. It assumes if a comment starts with /**, the entire line is a
  // comment. It will probably have issues when // and /** are used in the same line as well.

  const auto commentType1Start = "//";
  const auto commentType2Start = "/**";
  const auto commentType1StartingPosition = string.find(commentType1Start);
  const auto commentType2StartingPosition = string.find(commentType2Start);

  // No comment in this string.
  if ((commentType1StartingPosition == string.npos) && 
      (commentType2StartingPosition == string.npos)) return;

  
  if (commentType1StartingPosition != string.npos)
  { 
    // The entire string is a comment.
    if (commentType1StartingPosition == 0)
      string.clear();

    return;
  }

  // Remove the comment.
  string.erase(commentType2StartingPosition);
}
} // end of namespace definition