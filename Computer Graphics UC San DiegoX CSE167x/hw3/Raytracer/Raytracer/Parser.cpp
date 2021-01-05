#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <sstream>

#include "Parser.h"

Parser::Parser(const std::string &fileName) : scene_{}
{
  auto inputFile = std::ifstream(fileName);

  if (!inputFile)
    throw std::runtime_error("Could not open file " + fileName);

  doParse_(inputFile);
}

void Parser::doParse_(std::ifstream &inputFile)
{
  auto currentLine = std::string{};

  while (std::getline(inputFile, currentLine))
  {
    if (isLineComment_(currentLine) || currentLine.empty()) continue;
    parseCommand_(currentLine);
  }
}

void Parser::parseCommand_(const std::string &command)
{
  // This assumes that a line in the input file cannot start with a space.
  const auto endOfCommandNameIdx = command.find_first_of(" ");
  const auto commandName = command.substr(0, endOfCommandNameIdx);
  const auto commandArgs = command.substr(endOfCommandNameIdx + 1, 
                                          command.size() - endOfCommandNameIdx - 1);

  auto iss = std::istringstream(commandArgs);
  const auto commandArgsSeparated = std::vector<std::string>{std::istream_iterator<std::string>{iss},
                                                             std::istream_iterator<std::string>{}};


  switch (getCommandType_(commandName))
  {
  case CommandType::Size: setWindowSize_(commandArgsSeparated); break;
  case CommandType::Camera: setCamera_(commandArgsSeparated); break;
  default:
    throw std::runtime_error("Unsupported command: " + commandName);
  }
}

void Parser::setWindowSize_(const std::vector<std::string> &args)
{
  // The format of a size command is as below:
  // size Height Width
  const auto sizeOfSizeArgs = 2;
  if (args.size() != sizeOfSizeArgs)
    throw std::runtime_error(
      "Wrong number of camera command arguments: " + std::to_string(args.size()));

  scene_.setWindowSize(std::stoi(args.at(0)), std::stoi(args.at(1)));
}

void Parser::setCamera_(const std::vector<std::string> &args)
{
  // The format of a camera command is as below:
  // camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov

  const auto sizeOfCameraArgs = 10;
  if (args.size() != sizeOfCameraArgs)
    throw std::runtime_error(
      "Wrong number of camera command arguments: " + std::to_string(args.size()));

  std::vector<float> cameraArgs;
  std::for_each(args.begin(),
                args.end(),
                [&cameraArgs](const std::string &arg) {cameraArgs.emplace_back(std::stof(arg));});

  const auto lookFrom = std::array<float, 3>{cameraArgs.at(0), cameraArgs.at(1), cameraArgs.at(2)};
  const auto lookAt = std::array<float, 3>{cameraArgs.at(3), cameraArgs.at(4), cameraArgs.at(5)};
  const auto upVector = std::array<float, 3>{cameraArgs.at(6), cameraArgs.at(7), cameraArgs.at(8)};

  scene_.setCamera(lookFrom, lookAt, upVector, cameraArgs.at(9));
}

bool Parser::isLineComment_(const std::string &line) const
{
  const auto commentStart = "#";
  return line.find(commentStart) != line.npos;
}

Parser::CommandType Parser::getCommandType_(const std::string &commandName) const
{
  if (commandName == "camera") return CommandType::Camera;
  if (commandName == "size") return CommandType::Size;

  return CommandType::Unknown;
}