#include "Window.h"

Window::Window() : width_{800}, height_{600}
{}

Window::Window(const GLint windowWidth, const GLint windowHeight) :
  width_{windowWidth}, height_{windowHeight}
{}

int Window::initialize()
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
  pMainWindow_ = glfwCreateWindow(width_, height_, "Test window", nullptr, nullptr);

  if (!pMainWindow_)
  {
    printf("GLFW window creation failed!");
    glfwTerminate();
    return 1;
  }

  glfwGetFramebufferSize(pMainWindow_, &bufferWidth_, &bufferHeight_);

  // Set context for GLEW to use
  glfwMakeContextCurrent(pMainWindow_);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    printf("GLEW initialization failed!");
    glfwDestroyWindow(pMainWindow_);
    glfwTerminate();
    return 1;
  }

  // To check which part of the drawn shape is on top of the other parts of the shape.
  // This makes sure that the drawing is correct.
  glEnable(GL_DEPTH_TEST);

  // Setup viewport size
  glViewport(0, 0, bufferWidth_, bufferHeight_);
}

Window::~Window()
{
  glfwDestroyWindow(pMainWindow_);
  glfwTerminate();
}
