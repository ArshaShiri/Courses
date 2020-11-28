#include <algorithm>
#include <fstream>
#include <sstream>
#include <string_view>

#include "Helper.h"
#include "JackTokenizer.h"

namespace
{
void removeComments(std::string &string, bool &isMultiLineComment);
std::string extractStringLiteral(const std::string &txt,
                                 size_t &txtIdx);

/* These methods classify the tokens type according to Jack documentation. */
bool isKeyword(const std::string &token);
bool isSymbol(const std::string &token);
bool isIntegerConst(const std::string &token);
bool isStringConst(const std::string &token);
bool isIdentifier(const std::string &token);
} // end of namespace declaration

JackTokenizer::JackTokenizer(const std::string &inputFilepath) : currentTokenNumber_{0}, 
                                                                 waitingForFirstCall_{true}
{
  auto inputFile = std::ifstream(inputFilepath);

  if (!inputFile)
    throw std::runtime_error("Could not open file " + inputFilepath);

  tokenize_(inputFile);
}

bool JackTokenizer::hasMoreTokens() const
{
  return currentTokenNumber_ < tokens_.size();
}

void JackTokenizer::advance()
{
  if (waitingForFirstCall_)
  {
    currentTokenNumber_ = 0;
    waitingForFirstCall_ = false;
  }
  else
    ++currentTokenNumber_;
}

const Token& JackTokenizer::getCurrentToken() const
{
  return tokens_.at(currentTokenNumber_);
}

void JackTokenizer::tokenize_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};
  auto isMultiLineComment = false;

  while (std::getline(inputFile, currentLine))
  {
    removeComments(currentLine, isMultiLineComment);
    if (currentLine.empty()) continue;
    auto tokens = extractTokensInString_(currentLine);
    buildTokens_(std::move(tokens));
  }
}

std::vector<std::string> JackTokenizer::extractTokensInString_(const std::string &txt) const
{
  auto tokens = std::vector<std::string>{};

  auto addAndClearToken = [&tokens](std::string &token)
  {
    if (!token.empty())
    {
      tokens.emplace_back(token);
      token.clear();
    }
  };

  // We walk the given string(txt) one character at a time and extract tokens as soon as we reach 
  // them. Tokens are usually separated by white space. Symbols are mostly an exception to this 
  // rule.
  auto token = std::string{};
  for (auto charIndex = size_t{0}; charIndex < txt.size(); ++charIndex)
  {
    const auto currentChar = txt.at(charIndex);

    // We want to store the string literals given in the input file with "".
    if (currentChar == '\"')
    {
      token = extractStringLiteral(txt, charIndex);
      addAndClearToken(token);
      continue;
    }

    // If we reach white space or tab, store the current token.
    if ((currentChar == ' ') || (currentChar == '\t'))
    {
      addAndClearToken(token);
      continue;
    }

    // We check if we found a symbol.
    if (isSymbol(currentChar))
    {
      // Add the toke that has been build before symbol.
      addAndClearToken(token);

      // Now add the symbol as another token.
      tokens.emplace_back(std::string{currentChar});

      continue;
    }

    token.push_back(currentChar);
  }

  return tokens;
}

void JackTokenizer::buildTokens_(std::vector<std::string> &&tokens)
{
  for (auto &token : tokens)
    buildToken_(std::move(token));
}

void JackTokenizer::buildToken_(std::string &&token)
{
  if (isKeyword(token))
  {
    tokens_.emplace_back(Token{std::move(token), JackTokenType::KEYWORD});
    return;
  }

  if (isSymbol(token))
  {
    tokens_.emplace_back(Token{std::move(token), JackTokenType::SYMBOL});
    return;
  }

  if (isIntegerConst(token))
  {
    tokens_.emplace_back(Token{std::move(token), JackTokenType::INTEGERCONSTATNT});
    return;
  }

  if (isStringConst(token))
  {
    // Remove the quotation marks from the string.
    const auto tokenNameWithoutQuotation = token.substr(1, token.size() - 2);
    tokens_.emplace_back(Token{std::move(tokenNameWithoutQuotation), JackTokenType::STRINGCONSTANT});
    return;
  }

  if (isIdentifier(token))
  {
    tokens_.emplace_back(Token{std::move(token), JackTokenType::IDENTIFIER});
    return;
  }

  throw std::runtime_error("Cannot tokenize the current token: " + token);
}

namespace
{
void removeComment(size_t startPos, std::string &string)
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
}

void removeComments(std::string &string, bool &isMultiLineComment)
{
  // This function is limited. It assumes if a comment starts with /**, the entire line is a
  // comment. It will probably have issues when // and /** are used in the same line.

  const auto commentType1Start = "//";
  const auto multiLineCommentStart = "/**";
  const auto multiLineCommentEnd = "*/";

  const auto commentType1StartingPosition = string.find(commentType1Start);
  const auto multiLineCommentStartingPosition = string.find(multiLineCommentStart);
  const auto multiLineCommentEndPosition = string.find(multiLineCommentEnd);

  // isMultiLineComment can be true from previous calls to this function. It is important to check
  // this first because a line of multi-line comment can have no indication of it being a comment
  // and can be only identified from the previous calls:
  /*  the line starts with /* which can be recognized.
      this line cannot be recognized as a comment just by itself. We should know it from the
      previous line that the lines in the middle are part of multi-line comments.
  */
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

  removeComment(commentType1StartingPosition, string);
  removeComment(multiLineCommentStartingPosition, string);
}

std::string extractStringLiteral(const std::string &txt,
                                 size_t &txtIdx)
{
  auto token = std::string{};

  token.push_back('\"');
  for (auto strIdx = txtIdx + 1; strIdx < txt.size(); ++strIdx)
  {
    const auto strPart = txt.at(strIdx);
    token.push_back(strPart);

    if (strPart == '\"')
    {
      // We captured the string literal, now set the index to the next character.
      txtIdx = strIdx + 1;
      break;
    }
  }

  return token;
}

bool isKeyword(const std::string &token)
{
  if (token == "class")        return true;
  if (token == "method")       return true;
  if (token == "function")     return true;

  if (token == "constructor")  return true;
  if (token == "int")          return true;
  if (token == "boolean")      return true;

  if (token == "char")         return true;
  if (token == "void")         return true;
  if (token == "var")          return true;

  if (token == "static")       return true;
  if (token == "field")        return true;
  if (token == "let")          return true;

  if (token == "do")           return true;
  if (token == "if")           return true;
  if (token == "else")         return true;

  if (token == "while")        return true;
  if (token == "return")       return true;
  if (token == "true")         return true;

  if (token == "false")        return true;
  if (token == "null")         return true;
  if (token == "this")         return true;

  return false;
}

bool isSymbol(const std::string &token)
{
  if (::isSymbol(token.at(0))) return true;

  return false;
}

bool isIntegerConst(const std::string &token)
{
  return (!token.empty()) && (std::all_of(token.begin(), token.end(), ::isdigit));
}

bool isStringConst(const std::string &token)
{
  const auto startsWithQuotation = token.find_first_of("\"") == 0;
  const auto endsWithQuotation = token.find_last_of("\"") == token.size() - 1;

  return startsWithQuotation && endsWithQuotation;
}

bool isIdentifier(const std::string &token)
{
  return !token.empty();
}
} // end of namespace definition