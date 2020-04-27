#pragma once

#include <fstream>
#include <iostream>
#include <string>
// #include <stdio.h>

#include <GL\glew.h>

class Shader
{
public:
  Shader();

  void createFromString(const std::string vertexCode, const std::string fragmanetCode);
  void clearShader();
  void useShader() const;

  GLuint getProjectionLocation() const;
  GLuint getModelLocation() const;

  ~Shader();

private:
  // Private methods
  void compileShader_(const std::string vertexCode, const std::string fragmanetCode);
  void addShader_(const GLuint shaderProgram, const std::string shaderCode, const GLenum shaderType) const;

  // Private attributes
  GLuint shaderID_;
  GLuint uniformProjection_;
  GLuint uniformModel_;
};

