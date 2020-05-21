#pragma once

#include <array>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Constants.h"

#include <GLFW\glfw3.h>

class Camera
{
public:
  Camera(const glm::vec3 &position,
         const glm::vec3 &worldUp,
         GLfloat yaw,
         GLfloat pitch, 
         GLfloat moveSpeed, 
         GLfloat turnSpeed);

  void keyControl(const std::array<bool, NUMBER_OF_SUPPORTED_ASCII_CHARACTERS> keys, GLfloat deltaTime);
  void mouseControl(GLfloat xChange, GLfloat yChange);

  glm::mat4 calculateViewMatrix();

  ~Camera();

// Private methods
private:
  void update_();

// Private attributes.
private:
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 worldUp_;

  GLfloat yaw_;
  GLfloat pitch_;
  GLfloat movementSpeed_;
  GLfloat turnSpeed_;
};

