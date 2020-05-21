#include "Shader.h"



Shader::Shader() : shaderID_{0}, uniformModel_{0}, uniformProjection_{0}
{}

void Shader::createFromString(const std::string vertexCode, const std::string fragmanetCode)
{
  compileShader_(vertexCode, fragmanetCode);
}

void Shader::createFromFiles(const std::string vertexCodeLocation,
                             const std::string fragmanetCodeLocation)
{
  const auto vertexCode = readFile_(vertexCodeLocation);
  const auto fragmanetCode = readFile_(fragmanetCodeLocation);
  compileShader_(vertexCode, fragmanetCode);
}

void Shader::clearShader()
{
  if (shaderID_ != 0)
  {
    glDeleteProgram(shaderID_);
    shaderID_ = 0;
  }

  uniformModel_ = 0;
  uniformProjection_ = 0;
}

void Shader::useShader() const
{
  // We can check if the ID is nonzero here.
  glUseProgram(shaderID_);
}

GLuint Shader::getProjectionLocation() const
{
  return uniformProjection_;
}

GLuint Shader::getModelLocation() const
{
  return uniformModel_;
}

GLuint Shader::getViewLocation() const
{
  return uniformView_;
}

void Shader::addShader_(const GLuint shaderProgram,
                        const std::string shaderCode,
                        const GLenum shaderType) const
{
  // Shader id.
  GLuint theShader = glCreateShader(shaderType);

  // Attach the shader source code to the shader
  // The second argument specifies how many strings we're passing as source code
  const auto length = static_cast<GLint>(shaderCode.size());
  const auto shaderCodeInC = shaderCode.c_str();
  glShaderSource(theShader, 1, &shaderCodeInC, &length);
  glCompileShader(theShader);

  GLint result = 0;
  GLchar elog[1024] = {0};

  // Check if compilation was successful
  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    // If compilation failed, we can retrieve the error message.
    glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
    printf("Error compiling the %d shader: '%s'\n", shaderType, elog);
    return;
  }

  glAttachShader(shaderProgram, theShader);
}

void Shader::compileShader_(const std::string vertexCode, const std::string fragmanetCode)
{
  // creates a program and returns the ID reference to the newly created program object.
  shaderID_ = glCreateProgram();

  if (!shaderID_)
  {
    printf("Error in creating the shader program");
    return;
  }

  addShader_(shaderID_, vertexCode, GL_VERTEX_SHADER);
  addShader_(shaderID_, fragmanetCode, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar elog[1024] = {0};

  glLinkProgram(shaderID_);
  // The result is a program object that we can activate by calling glUseProgram with the newly
  // created program object as its argument.

  glGetProgramiv(shaderID_, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderID_, sizeof(elog), NULL, elog);
    printf("Error linking program: '%s'\n", elog);
    return;
  }

  glValidateProgram(shaderID_);
  glGetProgramiv(shaderID_, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderID_, sizeof(elog), NULL, elog);
    printf("Error validating program: '%s'\n", elog);
    return;
  }

  // Place the id of the location into their corresponding variable.
  uniformProjection_ = glGetUniformLocation(shaderID_, "projection");
  uniformModel_ = glGetUniformLocation(shaderID_, "model");
  uniformView_ = glGetUniformLocation(shaderID_, "view");
}

std::string Shader::readFile_(const std::string fileLocation) const
{
  auto fileContent = std::string{};
  auto fileStream = std::ifstream{fileLocation, std::ios::in};

  if (!fileStream.is_open())
  {
    std::cout << "Failed to read " + fileLocation + ". File does not exist!";
    return {};
  }

  auto newLine = std::string{};
  while (!fileStream.eof())
  {
    std::getline(fileStream, newLine);
    fileContent.append(newLine + "\n");
  }

  fileStream.close();
  return fileContent;
}

Shader::~Shader()
{
  clearShader();
}
