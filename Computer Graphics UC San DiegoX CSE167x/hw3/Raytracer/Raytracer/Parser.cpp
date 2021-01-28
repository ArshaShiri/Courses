#include <algorithm>
#include <array>
#include <fstream>
#include <iterator>
#include <sstream>

#include "Color.h"
#include "Parser.h"

namespace
{
void checkArgsSize(const std::vector<std::string> &args,
                    const std::string &command,
                    const size_t expectedSize);
} // End of namespace declaration

Parser::Parser(const std::string &fileName, Scene &scene) : scene_{scene}
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
  parse_(commandName, commandArgsSeparated);
}

bool Parser::isLineComment_(const std::string &line) const
{
  const auto commentStart = "#";
  return line.find(commentStart) != line.npos;
}

void Parser::parse_(
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

  case CommandType::DirectionalLight: addDirectionalLight_(args); break;
  case CommandType::PointLight: addPointLight_(args); break;
  case CommandType::Triangle: addTriangle_(args); break;

  default:
    throw std::runtime_error("Unsupported command: " + commandName);
  }
}

void Parser::initializeDefaultValuesForMaterialPropertiesAndAmbient_()
{
  currentMatProperties_.ambient = std::array<float, 3>{0.2f, 0.2f, 0.2f};
  currentMatProperties_.diffuse = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.specular = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.emission = std::array<float, 3>{0.0f, 0.0f, 0.0f};
  currentMatProperties_.shininess = 0.0f;
}

CommandType Parser::getCommandType_(const std::string &commandName) const
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

  if (commandName == "directional") return CommandType::DirectionalLight;
  if (commandName == "point") return CommandType::PointLight;
  if (commandName == "tri") return CommandType::Triangle;

  return CommandType::Unknown;
}

void Parser::setWindowSize_(const std::vector<std::string> &args)
{
  // Command format:
  // size Height Width
  const auto expectedArgsSize = 2;
  checkArgsSize(args, "window", expectedArgsSize);

  scene_.setWindowSize_(std::stoi(args.at(0)), std::stoi(args.at(1)));
}

void Parser::setCamera_(const std::vector<std::string> &args)
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

  scene_.setCamera_(lookFrom, lookAt, upVector, cameraArgs.at(9));
}

void Parser::setAmbient_(const std::vector<std::string> &args)
{
  currentMatProperties_.ambient = getThreeFloatArgumentsOfTheCommand_(args);
}

void Parser::setDiffuse_(const std::vector<std::string> &args)
{
  currentMatProperties_.diffuse = getThreeFloatArgumentsOfTheCommand_(args);
}

void Parser::setSpecular_(const std::vector<std::string> &args)
{
  currentMatProperties_.specular = getThreeFloatArgumentsOfTheCommand_(args);
}

void Parser::setEmission_(const std::vector<std::string> &args)
{
  currentMatProperties_.emission = getThreeFloatArgumentsOfTheCommand_(args);
}

void Parser::addVertex_(const std::vector<std::string> &args)
{
  if (scene_.getNumberOfVertices() == maxNumberOfVertieces_)
    throw std::runtime_error("Max number of vertices is reached!");

  scene_.addVertex_(getThreeFloatArgumentsOfTheCommand_(args));
}

void Parser::addDirectionalLight_(const std::vector<std::string> &args)
{
  // Command format:
  // directional x y z r g b
  const auto sizeOfCameraArgs = 6;
  checkArgsSize(args, "directional light", sizeOfCameraArgs);

  const auto direction =
    Vector3D{std::stof(args.at(0)), std::stof(args.at(1)), std::stof(args.at(2))};
  const auto color =
    Color{std::stof(args.at(3)), std::stof(args.at(4)), std::stof(args.at(5))};

  scene_.addDirectionalLight_(direction, color);
}

void Parser::addPointLight_(const std::vector<std::string> &args)
{
  // Command format:
  // point x y z r g b
  const auto sizeOfCameraArgs = 6;
  checkArgsSize(args, "point light", sizeOfCameraArgs);

  const auto point =
    Point3D{std::stof(args.at(0)), std::stof(args.at(1)), std::stof(args.at(2))};
  const auto color =
    Color{std::stof(args.at(3)), std::stof(args.at(4)), std::stof(args.at(5))};

  scene_.addPointLight_(point, color);
}

void Parser::addTriangle_(const std::vector<std::string> &args)
{
  // Command format:
  // tri v1 v2 v3
  const auto sizeOfCameraArgs = 3;
  checkArgsSize(args, "triangle", sizeOfCameraArgs);

  scene_.addTriangle_(currentMatProperties_,
    {std::stoi(args.at(0)), std::stoi(args.at(1)), std::stoi(args.at(2))});
}

std::array<float, 3> Parser::getThreeFloatArgumentsOfTheCommand_(const std::vector<std::string> &args)
{
  // Command format:
  // Any command followed by three float numbers as arguments, RGB, Vertex etc
  const auto expectedArgsSize = 3;
  checkArgsSize(args, "RGB", expectedArgsSize);

  return {std::stof(args.at(0)), std::stof(args.at(1)), std::stof(args.at(2))};
}

void Parser::setShininess_(const std::vector<std::string> &args)
{
  // Command format:
  // shininess s
  const auto expectedArgsSize = 1;
  checkArgsSize(args, "Shininess", expectedArgsSize);

  currentMatProperties_.shininess = std::stof(args.at(1));
}

void Parser::setMaxNumberOfVertieces_(const std::vector<std::string> &args)
{
  // Command format:
  // maxvertnorms number
  const auto expectedArgsSize = 1;
  checkArgsSize(args, "maxvertnorms ", expectedArgsSize);

  maxNumberOfVertieces_ = std::stoi(args.at(0));
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