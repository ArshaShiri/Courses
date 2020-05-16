#pragma once

#include<array>
#include <iostream>

#include<GL\glew.h>

#include <GLFW\glfw3.h>

class Window
{
private:
  static const int numberOfAsciiCharacters_ = 1024;

public:
  Window();
  Window(const GLint windowWidth, const GLint windowHeight);

  int initialize();

  GLfloat getBufferWidth() const { return (GLfloat)bufferWidth_; }
  GLfloat getBufferHeight() const { return (GLfloat)bufferHeight_; }
  const std::array<bool, numberOfAsciiCharacters_> &getKeys() const { return keys_; }

  GLfloat getXChange();
  GLfloat getYChange();

  bool shouldWindowClose() const { return glfwWindowShouldClose(pMainWindow_); }

  void swapBuffers() { glfwSwapBuffers(pMainWindow_); }

  ~Window();

private:
// Private Methods
  void initializeKeys_();
  void createCallbacks();
  static void handleKeys_(GLFWwindow *pWindow, int key, int code, int action, int mode);
  static void handleMouse_(GLFWwindow *pWindow, double xPos, double yPos);

// Private Attributes
  GLFWwindow *pMainWindow_;

  GLint width_;
  GLint height_;
  GLint bufferWidth_;
  GLint bufferHeight_;

  GLfloat lastX_;
  GLfloat lastY_;
  GLfloat xChange_;
  GLfloat yChange_;
  bool isMouseMovedAtFirst_;

  std::array<bool, numberOfAsciiCharacters_> keys_;
};