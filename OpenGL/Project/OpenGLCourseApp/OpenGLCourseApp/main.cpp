
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>


#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include <memory>

// Window's dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint shaderProgram, uniformModel, uniformProjection;
const float TO_RAD = M_PI / 180.0f;

std::vector<std::unique_ptr<Mesh>> meshList;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex shader.
// Location of the input variable via layout (location = 0)
// To set the output of the vertex shader we have to assign the position data to the predefined 
// gl_Position variable which is a vec4 behind the scenes.
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
out vec4 vCol;                                              \n\
uniform mat4 model;                                         \n\
uniform mat4 projection;                                    \n\
void main()                                                 \n\
{                                                           \n\
  gl_Position = projection * model * vec4(pos, 1.0);        \n\
  vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                \n\
}";

// Fragment shader.
static const char* fShader = "       \n\
#version 330                         \n\
                                     \n\
in vec4 vCol;                        \n\
out vec4 color;                      \n\
void main()                          \n\
{                                    \n\
  color = vCol;                      \n\
}";

void createPyramid()
{
  unsigned indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2
  };

  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f};

  auto meshObject = std::make_unique<Mesh>();
  meshObject->createMesh(vertices, indices, 12, 12);
  meshList.emplace_back(std::move(meshObject));
}

void addShader(GLuint shaderProgram, const char *shaderCode, GLenum shaderType)
{
  // Shader id.
  GLuint theShader = glCreateShader(shaderType);

  const GLchar *theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);
  
  // Attach the shader source code to the shader
  // The second argument specifies how many strings we're passing as source code
  glShaderSource(theShader, 1, theCode, codeLength);
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

void compileShaders()
{
  // creates a program and returns the ID reference to the newly created program object.
  shaderProgram = glCreateProgram();
  if (!shaderProgram)
  {
    printf("Error in creating the shader program");
    return;
  }

  addShader(shaderProgram, vShader, GL_VERTEX_SHADER);
  addShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar elog[1024] = {0};

  glLinkProgram(shaderProgram);
  // The result is a program object that we can activate by calling glUseProgram with the newly
  // created program object as its argument.

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderProgram, sizeof(elog), NULL, elog);
    printf("Error linking program: '%s'\n", elog);
    return;
  }

  glValidateProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
  if (!result)
  {
    glGetProgramInfoLog(shaderProgram, sizeof(elog), NULL, elog);
    printf("Error validating program: '%s'\n", elog);
    return;
  }

  // Place the id of the location into their corresponding variable.
  uniformModel = glGetUniformLocation(shaderProgram, "model");
  uniformProjection = glGetUniformLocation(shaderProgram, "projection");
}

int main()
{
  // Initialize GLFW
  if (!glfwInit())
  {
    printf("GLFW initialization failed!");
    glfwTerminate();
    return 1;
  }

  // OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Core profile = No backwards compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Allow forward compatibility
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Setup GLFW window properties
  GLFWwindow *pMainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", nullptr, nullptr);

  if (!pMainWindow)
  {
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  // Get buffer size information
  auto bufferWidth = 0;
  auto bufferHeight = 0;

  glfwGetFramebufferSize(pMainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(pMainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    printf("GLEW initialization failed!");
    glfwDestroyWindow(pMainWindow);
    glfwTerminate();
    return 1;
  }

  // To check which part of the drawn shape is on top of the other parts of the shape.
  // This makes sure that the drawing is correct.
  glEnable(GL_DEPTH_TEST);

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  createPyramid();
  compileShaders();

  // Field of view top to bottom.
  glm::mat4 projectionMatrix =
    glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

  // Loop until window is closed.
  while (!glfwWindowShouldClose(pMainWindow))
  {
    // Get + handle user input events
    glfwPollEvents();

    if (direction)
    {
      triOffset += triIncrement;
    }
    else
    {
      triOffset -= triIncrement;
    }

    if (abs(triOffset) >= triMaxOffset)
    {
      direction = !direction;
    }

    currentAngle += 0.01f;
    if (currentAngle >= 360)
    {
      currentAngle -= 360;
    }

    if (sizeDirection)
    {
      currentSize += 0.0001f;
    }
    else
    {
      currentSize -= 0.0001f;
    }

    if ((currentSize >= maxSize) || (currentSize <= minSize))
    {
      sizeDirection = !sizeDirection;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the id of the shader program on our graphic card.
    glUseProgram(shaderProgram);
    
    // 4x4  identity matrix.
    glm::mat4 modelMatrix(1.0f);

    // Think about the order of translation and rotations. The order matters!

    // Making the translation matrix.
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.5f));

    // Making the rotation matrix.
    modelMatrix = glm::rotate(modelMatrix, currentAngle * TO_RAD, glm::vec3(0.0f, 1.0f, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));

   glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
   glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Every shader and rendering call after glUseProgram will now use this program object 
    // (and thus the shaders).

   meshList.at(0)->renderMesh();
   glUseProgram(0);

    glfwSwapBuffers(pMainWindow);
  }

  return 0;
}