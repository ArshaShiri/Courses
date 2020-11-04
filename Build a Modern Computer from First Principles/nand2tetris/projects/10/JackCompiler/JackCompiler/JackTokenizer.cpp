#include <fstream>
#include <iterator>
#include <sstream>
#include <string_view>

#include "JackTokenizer.h"
#include "TokenFactory.h"

namespace
{
void removeComments(std::string &string, bool &isMultiLineComment);
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
  if (token == '&')  return true;
  if (token == '|')  return true;

  if (token == '<')  return true;
  if (token == '>')  return true;
  if (token == '=')  return true;
  if (token == '~')  return true; 

  return false;
}

void JackTokenizer::extractTokensInString_(const std::string &txt)
{
  auto addToken = [this](std::string &token)
  {
    if (!token.empty())
    {
      tokens_.emplace_back(token);
      token.clear();
    }
  };

  auto token = std::string{};
  for (auto charIndex = 0; charIndex < txt.size(); ++charIndex)
  {
    const auto currentChar = txt.at(charIndex);

    // We want to capture string literals given in the input completely.
    if (currentChar == '\"')
    {
      token.push_back('\"');
      for (auto strIdx = charIndex + 1; strIdx < txt.size(); ++strIdx)
      {
        const auto strPart = txt.at(strIdx);
        token.push_back(strPart);

        if (strPart == '\"')
          break;

        charIndex = strIdx + 1;
      }

      addToken(token);
      continue;
    }

    // If we reach white space or tab, store the current token.
    if ((currentChar == ' ') || (currentChar == '\t'))
    {
      addToken(token);
      continue;
    }

    if (isSymbol(currentChar))
    {
      addToken(token);
      tokens_.emplace_back(std::string{currentChar});

      continue;
    }

    token.push_back(currentChar);
  }
}

void JackTokenizer::storeTokens_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};
  auto isMultiLineComment = false;

  while (std::getline(inputFile, currentLine))
  {
    removeComments(currentLine, isMultiLineComment);
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
void removeComments(std::string &string, bool &isMultiLineComment)
{
  // This function is limited. It assumes if a comment starts with /**, the entire line is a
  // comment. It will probably have issues when // and /** are used in the same line as well.

  const auto commentType1Start = "//";
  const auto multiLineCommentStart = "/**";
  const auto multiLineCommentEnd = "*/";

  const auto commentType1StartingPosition = string.find(commentType1Start);
  const auto multiLineCommentStartingPosition = string.find(multiLineCommentStart);
  const auto multiLineCommentEndPosition = string.find(multiLineCommentEnd);

  if (multiLineCommentStartingPosition != string.npos)
    isMultiLineComment = true;

  if (isMultiLineComment)
  {
    if (multiLineCommentEndPosition != string.npos)
    {
      string.erase(0, multiLineCommentEndPosition + 2);
      isMultiLineComment = false;
    }
    else
      string.clear();

    return;
  }

  // No comment in this string.
  if ((commentType1StartingPosition == string.npos) && 
      (multiLineCommentStartingPosition == string.npos)) return;

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
  removeComment(multiLineCommentStartingPosition);
}
} // end of namespace definition