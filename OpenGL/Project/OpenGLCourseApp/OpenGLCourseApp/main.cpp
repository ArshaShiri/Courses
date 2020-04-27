
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string>
#include <math.h>

#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window's dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shaderProgram, uniformModel;
const float TO_RAD = M_PI / 180.0f;

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
void main()                                                 \n\
{                                                           \n\
  gl_Position = model * vec4(pos, 1.0);                     \n\
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

void createTrinagnle()
{
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f};

  // Creating a vertex array on the graphic card.
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // vertex buffer objects (VBO). Memory created on GPU for vertices.
  // Sending data to the graphics card from the CPU is relatively slow, so wherever we can we try
  // to send as much data as possible at once.
  // id = 1
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
  // GL_STATIC_DRAW : the data is set only once and used many times.
  // GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
  // Copies the vertices data to the associated buffer.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Take a look at the shader: location: 0
  // The vertex shader allows us to specify any input we want in the form of vertex attributes and 
  // while this allows for great flexibility, it does mean we have to manually specify what part of 
  // our input data goes to which vertex attribute in the vertex shader.This means we have to
  // specify how OpenGL should interpret the vertex data before rendering.

  // Each vertex attribute takes its data from memory managed by a VBO and which VBO it takes its
  // data from(you can have multiple VBOs) is determined by the VBO currently bound to 
  // GL_ARRAY_BUFFER when calling glVertexAttribPointer.Since the previously defined VBO is still
  // bound before calling glVertexAttribPointer vertex attribute 0 is now associated with its
  // vertex data.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // Unbinding.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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

  uniformModel = glGetUniformLocation(shaderProgram, "model");
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

  // Setup viewport size
  glViewport(0, 0, bufferWidth, bufferHeight);

  createTrinagnle();
  compileShaders();

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
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the id of the shader program on our graphic card.
    glUseProgram(shaderProgram);
    
    // 4x4  identity matrix.
    glm::mat4 modelMatrix(1.0f);

    // Think about the order of translation and rotations. The order matters!

    // Making the translation matrix.
    // modelMatrix = glm::translate(modelMatrix, glm::vec3(triOffset, 0.0f, 0.0f));

    // Making the rotation matrix.
    // modelMatrix = glm::rotate(modelMatrix, currentAngle * TO_RAD, glm::vec3(0.0f, 0.0f, 1.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4, 0.4, 1.0f));

   glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));


    // Every shader and rendering call after glUseProgram will now use this program object 
    // (and thus the shaders).

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(pMainWindow);
  }

  return 0;
}