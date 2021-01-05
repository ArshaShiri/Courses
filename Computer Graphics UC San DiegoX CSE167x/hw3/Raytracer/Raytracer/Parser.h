/* Parses the given input file and converts it to a scene object. */

#pragma once

#include <string>
#include <vector>

#include "scene.h"

class Parser
{
public:
  Parser(const std::string &fileName);
  
private:
  enum class CommandType
  {
    Camera,
    Size,
    Unknown
  };

  // Private methods.
  void doParse_(std::ifstream &inputFile);
  void parseCommand_(const std::string &command);

  void setCamera_(const std::vector<std::string> &args);
  void setWindowSize_(const std::vector<std::string> &args);

  bool isLineComment_(const std::string &line) const;
  CommandType getCommandType_(const std::string &commandName) const;

  // Private attributes
  Scene scene_;
};

