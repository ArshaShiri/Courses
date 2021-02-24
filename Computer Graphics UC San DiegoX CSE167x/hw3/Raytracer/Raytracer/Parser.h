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
  Attenuation,
  Maxverts,
  Vertex,
  DirectionalLight,
  PointLight,
  Triangle,
  Sphere,
  Translate,
  Rotate,
  Scale,
  PushTransform,
  PopTransform,
  SetOutput,
  Maxdepth,
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

  void initializeDefaultValues_();

  CommandType getCommandType_(const std::string &commandName) const;

  void setCamera_(const std::vector<std::string> &args);
  void setWindowSize_(const std::vector<std::string> &args);

  void setAmbient_(const std::vector<std::string> &args);
  void setDiffuse_(const std::vector<std::string> &args);
  void setSpecular_(const std::vector<std::string> &args);
  void setEmission_(const std::vector<std::string> &args);
  void setShininess_(const std::vector<std::string> &args);
  void setAttenuation_(const std::vector<std::string> &args);

  void addVertex_(const std::vector<std::string> &args);
  std::array<float, 3> getThreeFloatArgumentsOfTheCommand_(const std::vector<std::string> &args);

  void addDirectionalLight_(const std::vector<std::string> &args);
  void addPointLight_(const std::vector<std::string> &args);
  void addTriangle_(const std::vector<std::string> &args);
  void addSphere_(const std::vector<std::string> &args);

  void addTranslation_(const std::vector<std::string> &args);
  void addRotation_(const std::vector<std::string> &args);
  void addScale_(const std::vector<std::string> &args);
  void pushTransformation_(const std::vector<std::string> &args);
  void popTransformation_(const std::vector<std::string> &args);
  void setOutput_(const std::vector<std::string> &args);
  void setMaxDepth_(const std::vector<std::string> &args);

  void setMaxNumberOfVertieces_(const std::vector<std::string> &args);

  // Private attributes
  Scene &scene_;
  MateriaPropertiesAndAmbient currentMatProperties_;
  Attenuation attenuation_;
  size_t maxNumberOfVertieces_;
};