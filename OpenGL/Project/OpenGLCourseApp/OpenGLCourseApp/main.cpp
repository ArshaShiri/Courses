
#define _USE_MATH_DEFINES

#include <math.h>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

// Window's dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

const float TO_RAD = M_PI / 180.0f;

std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<std::unique_ptr<Shader>> shaderList;

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

void createObjects()
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

  auto meshObject1 = std::make_unique<Mesh>();
  meshObject1->createMesh(vertices, indices, 12, 12);
  meshList.emplace_back(std::move(meshObject1));

  auto meshObject2 = std::make_unique<Mesh>();
  meshObject2->createMesh(vertices, indices, 12, 12);
  meshList.emplace_back(std::move(meshObject2));
}

void createShaders()
{
  auto shader1 = std::make_unique<Shader>();
  shader1->createFromString(vShader, fShader);
  shaderList.emplace_back(std::move(shader1));
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

  createObjects();
  createShaders();

  GLuint uniformProjection = 0;
  GLuint uniformModel = 0;

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
    const auto &pShader1 = shaderList.at(0);
    pShader1->useShader();
    uniformModel = pShader1->getModelLocation();
    uniformProjection = pShader1->getProjectionLocation();
    
    // 4x4  identity matrix.
    glm::mat4 modelMatrix(1.0f);

    // Think about the order of translation and rotations. The order matters!

    // Making the translation matrix.
    modelMatrix = glm::translate(modelMatrix, glm::vec3(triOffset, 0.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    meshList.at(0)->renderMesh();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-triOffset, 1.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    meshList.at(1)->renderMesh();

    // Every shader and rendering call after glUseProgram will now use this program object 
    // (and thus the shaders).
    glUseProgram(0);

    glfwSwapBuffers(pMainWindow);
  }

  return 0;
}
