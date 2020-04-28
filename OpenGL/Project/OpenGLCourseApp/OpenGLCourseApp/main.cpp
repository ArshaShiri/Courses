
#define _USE_MATH_DEFINES

#include <math.h>
#include <memory>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float TO_RAD = M_PI / 180.0f;

auto mainWindow = Window{};
auto meshList = std::vector<std::unique_ptr<Mesh>>{};
auto shaderList = std::vector<std::unique_ptr<Shader>>{};

// Vertex shader.
// Location of the input variable via layout (location = 0)
// To set the output of the vertex shader we have to assign the position data to the predefined 
// gl_Position variable which is a vec4 behind the scenes.
const auto vShader = std::string{"Shaders/shader.vert"};

// Fragment shader.
const auto fShader = std::string{"Shaders/shader.frag"};

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
  shader1->createFromFiles(vShader, fShader);
  shaderList.emplace_back(std::move(shader1));
}

int main()
{
  mainWindow.initialize();
  createObjects();
  createShaders();

  GLuint uniformProjection = 0;
  GLuint uniformModel = 0;

  // Field of view top to bottom.
  glm::mat4 projectionMatrix = 
    glm::perspective(45.0f,
                     mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
                     0.1f,
                     100.0f);

  // Loop until window is closed.
  while (!mainWindow.shouldWindowClose())
  {
    // Get + handle user input events
    glfwPollEvents();

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
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    meshList.at(0)->renderMesh();

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, -2.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    meshList.at(1)->renderMesh();

    // Every shader and rendering call after glUseProgram will now use this program object 
    // (and thus the shaders).
    glUseProgram(0);

    mainWindow.swapBuffers();
  }

  return 0;
}