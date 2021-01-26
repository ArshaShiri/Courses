#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <sstream>

#include "Parser.h"

namespace
{
void checkArgsSize(const std::vector<std::string> &args,
                    const std::string &command,
                    const size_t expectedSize);
} // End of namespace declaration

Parser::Parser(const std::string &fileName)
{
  auto inputFile = std::ifstream(fileName);

  if (!inputFile)
    throw std::runtime_error("Could not open file " + fileName);

  parseCommands_(inputFile);
}

void Parser::parseCommands_(std::ifstream &inputFile)
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
  parserHelper_.parse(commandName, commandArgsSeparated);
}

bool Parser::isLineComment_(const std::string &line) const
{
  const auto commentStart = "#";
  return line.find(commentStart) != line.npos;
}

/* ParserHelper */

ParserHelper::ParserHelper()
{
  initializeDefaultValuesForMaterialPropertiesAndAmbient_();
}

void ParserHelper::parse(
  const std::string &commandName, const std::vector<std::string> &args)
{
  switch (getCommandType_(commandName))
  {
  case CommandType::Size: setWindowSize_(args); break;
  case CommandType::Camera: setCamera_(args); break;

  case CommandType::Ambient: setAmbient_(args); break;
  case CommandType::Diffuse: setDiffuse_(args); break;
  case CommandType::Specular: setSpecular_(args); break;
  case CommandType::Emission: setEmission_(args); break;
  case CommandType::Shininess: setShininess_(args); break;

  case CommandType::Maxverts: setMaxNumberOfVertieces_(args); break;
  case CommandType::Vertex: addVertex_(args); break;

  default:
    throw std::runtime_error("Unsupported command: " + commandName);
  }
}

void ParserHelper::initializeDefaultValuesForMaterialPropertiesAndAmbient_()
{
  currentMatProperties_.ambient = std::array<float, 3>{0.2f, 0.2f, 0.2f};
  currentMatProperties_.diffuse = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.specular = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.emission = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.shininess = 0.0f;
}

CommandType ParserHelper::getCommandType_(const std::string &commandName) const
{
  if (commandName == "camera") return CommandType::Camera;
  if (commandName == "size") return CommandType::Size;

  if (commandName == "ambient") return CommandType::Ambient;
  if (commandName == "diffuse") return CommandType::Diffuse;
  if (commandName == "specular") return CommandType::Specular;
  if (commandName == "emission") return CommandType::Emission;
  if (commandName == "shininess") return CommandType::Shininess;

  if (commandName == "maxverts") return CommandType::Maxverts;
  if (commandName == "vertex") return CommandType::Vertex;

  return CommandType::Unknown;
}

void ParserHelper::setWindowSize_(const std::vector<std::string> &args)
{
  // Command format:
  // size Height Width
  const auto expectedArgsSize = 2;
  checkArgsSize(args, "window", expectedArgsSize);

  scene_.setWindowSize(std::stoi(args.at(0)), std::stoi(args.at(1)));
}

void ParserHelper::setCamera_(const std::vector<std::string> &args)
{
  // Command format:
  // camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
  const auto sizeOfCameraArgs = 10;
  checkArgsSize(args, "camera", sizeOfCameraArgs);

  std::vector<float> cameraArgs;
  std::for_each(args.begin(),
    args.end(),
    [&cameraArgs](const std::string &arg) { cameraArgs.emplace_back(std::stof(arg)); });

  const auto lookFrom = std::array<float, 3>{cameraArgs.at(0), cameraArgs.at(1), cameraArgs.at(2)};
  const auto lookAt = std::array<float, 3>{cameraArgs.at(3), cameraArgs.at(4), cameraArgs.at(5)};
  const auto upVector = std::array<float, 3>{cameraArgs.at(6), cameraArgs.at(7), cameraArgs.at(8)};

  scene_.setCamera(lookFrom, lookAt, upVector, cameraArgs.at(9));
}

void ParserHelper::setAmbient_(const std::vector<std::string> &args)
{
  currentMatProperties_.ambient = getThreeFloatArgumentsOfTheCommand_(args);
}

void ParserHelper::setDiffuse_(const std::vector<std::string> &args)
{
  currentMatProperties_.diffuse = getThreeFloatArgumentsOfTheCommand_(args);
}

void ParserHelper::setSpecular_(const std::vector<std::string> &args)
{
  currentMatProperties_.specular = getThreeFloatArgumentsOfTheCommand_(args);
}

void ParserHelper::setEmission_(const std::vector<std::string> &args)
{
  currentMatProperties_.emission = getThreeFloatArgumentsOfTheCommand_(args);
}

void ParserHelper::addVertex_(const std::vector<std::string> &args)
{
  if (scene_.getNumberOfVertices() == maxNumberOfVertieces_)
    throw std::runtime_error("Max number of vertices is reached!");
  
  scene_.addVertex(getThreeFloatArgumentsOfTheCommand_(args));
}

std::array<float, 3> ParserHelper::getThreeFloatArgumentsOfTheCommand_(const std::vector<std::string> &args)
{
  // Command format:
  // Any command followed by three float numbers as arguments, RGB, Vertex etc
  const auto expectedArgsSize = 3;
  checkArgsSize(args, "RGB", expectedArgsSize);

  return {std::stof(args.at(0)), std::stof(args.at(1)), std::stof(args.at(1))};
}

void ParserHelper::setShininess_(const std::vector<std::string> &args)
{
  // Command format:
  // shininess s
  const auto expectedArgsSize = 1;
  checkArgsSize(args, "Shininess", expectedArgsSize);

  currentMatProperties_.shininess = std::stof(args.at(1));
}

void ParserHelper::setMaxNumberOfVertieces_(const std::vector<std::string> &args)
{
  // Command format:
  // maxvertnorms number
  const auto expectedArgsSize = 1;
  checkArgsSize(args, "maxvertnorms ", expectedArgsSize);

  maxNumberOfVertieces_ = std::stoi(args.at(1));
}

namespace
{
void checkArgsSize(const std::vector<std::string> &args,
                    const std::string &command,
                    const size_t expectedSize)
{
  if (args.size() != expectedSize)
    throw std::runtime_error(
      "Wrong number of " + command + " command arguments: " + std::to_string(args.size()));
}
} // End of namespace definition