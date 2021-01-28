/* Parses the given input file and converts it to a scene object. */

#pragma once

#include <string>
#include <vector>

#include "Scene.h"

enum class CommandType
{
  Camera,
  Size,
  Ambient,
  Diffuse,
  Specular,
  Emission,
  Shininess,
  Maxverts,
  Vertex,
  DirectionalLight,
  PointLight,
  Triangle,
  Unknown
};

class Parser
{
public:
  Parser(const std::string &fileName, Scene &scene);
  
private:
  // Private methods
  void parseCommands_(std::ifstream &inputFile);
  void parseCommand_(const std::string &command);
  bool isLineComment_(const std::string &line) const;

  void parse_(const std::string &commandName, const std::vector<std::string> &args);

  void initializeDefaultValuesForMaterialPropertiesAndAmbient_();

  CommandType getCommandType_(const std::string &commandName) const;

  void setCamera_(const std::vector<std::string> &args);
  void setWindowSize_(const std::vector<std::string> &args);

  void setAmbient_(const std::vector<std::string> &args);
  void setDiffuse_(const std::vector<std::string> &args);
  void setSpecular_(const std::vector<std::string> &args);
  void setEmission_(const std::vector<std::string> &args);
  void addVertex_(const std::vector<std::string> &args);
  std::array<float, 3> getThreeFloatArgumentsOfTheCommand_(const std::vector<std::string> &args);

  void addDirectionalLight_(const std::vector<std::string> &args);
  void addPointLight_(const std::vector<std::string> &args);
  void addTriangle_(const std::vector<std::string> &args);

  void setMaxNumberOfVertieces_(const std::vector<std::string> &args);
  void setShininess_(const std::vector<std::string> &args);

  // Private attributes
  Scene &scene_;
  MateriaPropertiesAndAmbient currentMatProperties_;
  size_t maxNumberOfVertieces_;
};