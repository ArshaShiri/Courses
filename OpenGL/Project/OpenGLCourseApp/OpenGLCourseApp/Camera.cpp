#include "Camera.h"



Camera::Camera(const glm::vec3 &position,
               const glm::vec3 &worldUp,
               GLfloat yaw,
               GLfloat pitch,
               GLfloat moveSpeed,
               GLfloat turnSpeed) : position_{position},
                                    worldUp_{worldUp},
                                    yaw_{yaw},
                                    pitch_{pitch},
                                    movementSpeed_{moveSpeed},
                                    turnSpeed_{turnSpeed},
                                    front_{glm::vec3(0.0f, 0.0f, -1.0f)}
{
  update_();
}

void Camera::keyControl(const std::array<bool, NUMBER_OF_SUPPORTED_ASCII_CHARACTERS> keys, GLfloat deltaTime)
{
  GLfloat velocity = movementSpeed_ * deltaTime;

  if (keys.at(GLFW_KEY_W))
  {
    position_ += front_ * velocity;
  }

  if (keys.at(GLFW_KEY_S))
  {
    position_ -= front_ * velocity;
  }

  if (keys.at(GLFW_KEY_D))
  {
    position_ += right_ * velocity;
  }

  if (keys.at(GLFW_KEY_A))
  {
    position_ -= right_ * velocity;
  }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
  xChange *= turnSpeed_;
  yChange *= turnSpeed_;

  yaw_ += xChange;
  pitch_ += yChange;
  
  // Limit the amount that we can look up.
  if (pitch_ > 89.0f)
  {
    pitch_ = 89.0;
  }

  // Limit the amount that we can look down.
  if (pitch_ < -89.0f)
  {
    pitch_ = -89.0;
  }

  update_();
}

glm::mat4 Camera::calculateViewMatrix()
{
  // Our position plus the direction to look at.
  return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::update_()
{
  front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_.y = sin(glm::radians(pitch_));
  front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_ = glm::normalize(front_);

  right_ = glm::normalize(glm::cross(front_, worldUp_));
  up_ = glm::normalize(glm::cross(right_, front_));
}

Camera::~Camera()
{}