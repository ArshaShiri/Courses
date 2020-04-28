#pragma once

#include <iostream>

#include<GL\glew.h>

#include <GLFW\glfw3.h>

class Window
{
public:
  Window();
  Window(const GLint windowWidth, const GLint windowHeight);

  int initialize();

  GLfloat getBufferWidth() const { return (GLfloat)bufferWidth_; }
  GLfloat getBufferHeight() const { return (GLfloat)bufferHeight_; }

  bool shouldWindowClose() const { return glfwWindowShouldClose(pMainWindow_); }

  void swapBuffers() { glfwSwapBuffers(pMainWindow_); }

  ~Window();

private:
  GLFWwindow *pMainWindow_;
  GLint width_;
  GLint height_;
  GLint bufferWidth_;
  GLint bufferHeight_;
};