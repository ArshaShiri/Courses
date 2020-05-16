#include "Window.h"

Window::Window() : width_{800}, height_{600}
{
  initializeKeys_();
}

Window::Window(const GLint windowWidth, const GLint windowHeight) :
  width_{windowWidth}, height_{windowHeight}
{
  initializeKeys_();
}

void Window::initializeKeys_()
{
  for (auto &el : keys_)
  {
    el = false;
  }
}

void Window::createCallbacks()
{
  glfwSetKeyCallback(pMainWindow_, handleKeys_);
  glfwSetCursorPosCallback(pMainWindow_, handleMouse_);
}

GLfloat Window::getXChange()
{
  auto change = xChange_;
  xChange_ = 0;
  return change;
}

GLfloat Window::getYChange()
{
  auto change =yChange_;
  yChange_ = 0;
  return change;
}

void Window::handleKeys_(GLFWwindow *pWindow, int key, int code, int action, int mode)
{
  auto pTheWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));

  if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
  {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }

  if ((key >= 0) && (key < numberOfAsciiCharacters_))
  {
    if (action == GLFW_PRESS)
    {
      // Set the pressed key to true.
      pTheWindow->keys_[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
      // Set the released key to false.
      pTheWindow->keys_[key] = false;
    }
  }
}

void Window::handleMouse_(GLFWwindow *pWindow, double xPos, double yPos)
{
  auto pTheWindow = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));

  if (pTheWindow->isMouseMovedAtFirst_)
  {
    pTheWindow->lastX_ = (GLfloat)xPos;
    pTheWindow->lastY_ = (GLfloat)yPos;
    pTheWindow->isMouseMovedAtFirst_ = false;
  }

  pTheWindow->xChange_ = (GLfloat)xPos - pTheWindow->lastX_;

  // Avoid inverted up and down.
  pTheWindow->yChange_ = pTheWindow->lastY_ - (GLfloat)yPos;

  pTheWindow->lastX_ = (GLfloat)xPos;
  pTheWindow->lastY_ = (GLfloat)yPos;
}

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
  
  // Handle key and mouse input.
  createCallbacks();

  // Lock the mouse cursor to the screen.
  glfwSetInputMode(pMainWindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  // Used for call back to the handle key function. 
  glfwSetWindowUserPointer(pMainWindow_, this);
}

Window::~Window()
{
  glfwDestroyWindow(pMainWindow_);
  glfwTerminate();
}
