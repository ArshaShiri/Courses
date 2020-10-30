#include <fstream>
#include <iterator>
#include <sstream>
#include <string_view>

#include "JackTokenizer.h"
#include "TokenFactory.h"

namespace
{
void removeComments(std::string &string);
} // end of namespace declaration

JackTokenizer::JackTokenizer(const std::string &inputFilepath) : 
  currentTokenNumber_{0}//, currentTokenNumberInLine_{0}
{
  auto inputFile = std::ifstream(inputFilepath);

  if (!inputFile)
    throw std::runtime_error("Could not open file" + inputFilepath);

  storeTokens_(inputFile);
  hasMoreTokens_ = !tokens_.empty();
}

bool JackTokenizer::hasMoreTokens() const
{
  return currentTokenNumber_ < tokens_.size();
}

void JackTokenizer::advance()
{
  auto tokenFactory = TokenFactory{tokens_.at(currentTokenNumber_)};
  pToken_ = tokenFactory.tokenize();
  ++currentTokenNumber_;
}

// Move this function to a common place....
bool isSymbol(const char  &token)
{
  if (token == '}')  return true;
  if (token == '(')  return true;
  if (token == '{')  return true;
               
  if (token == ')')  return true;
  if (token == '[')  return true;
  if (token == ']')  return true;

  if (token == '.')  return true;
  if (token == ',')  return true;
  if (token == ';')  return true;

  if (token == '+')  return true;
  if (token == '-')  return true;
  if (token == '*')  return true;

  if (token == '/')  return true;
  if (token == '*')  return true;
  if (token == '|')  return true;

  if (token == '<')  return true;
  if (token == '>')  return true;
  if (token == '=')  return true;

  // if (tokenName_ == "")        return true; Not sure what is this in the specs??

  return false;
}

void JackTokenizer::extractTokensInString_(const std::string &txt)
{
  auto token = std::string{};
  for (const auto currentChar : txt)
  {
    if (currentChar == ' ')
    {
      if (!token.empty())
      {
        tokens_.emplace_back(token);
        token.clear();
      }

      continue;
    }

    if (isSymbol(currentChar))
    {
      if (!token.empty())
      {
        tokens_.emplace_back(token);
        token.clear();
      }
      tokens_.emplace_back(std::string{currentChar});

      continue;
    }

    token.push_back(currentChar);
  }
}

void JackTokenizer::storeTokens_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};

  while (std::getline(inputFile, currentLine))
  {
    removeComments(currentLine);
    if (currentLine.empty()) continue;
    extractTokensInString_(currentLine);
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

  auto removeComment = [&string](size_t startPos)
  {
    if (startPos != string.npos)
    {
      // The entire string is a comment.
      if (startPos == 0)
        string.clear();
      else
        // Remove the comment.
        string.erase(startPos);
    }
  };

  removeComment(commentType1StartingPosition);
  removeComment(commentType2StartingPosition);
}
} // end of namespace definition