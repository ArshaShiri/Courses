#include <array>
#include <stdexcept>
#include <string>

#include "Scene.h"
#include "GLMWrapper.h"

Scene::Scene() : height_{0}, width_{0}, camera_{}, cameraIsSet_{false}
{}

void Scene::setWindowSize(int height, int width)
{
  if ((height < 0) || (width < 0))
    throw std::runtime_error(
      "Invalid height: " + std::to_string(height) + "or width: " + std::to_string(width));

  height_ = height;
  width_ = width;
}

void Scene::setCamera(const std::array<float, 3> &lookFrom,
                      const std::array<float, 3> &lookAt,
                      const std::array<float, 3> &upVector,
                      float fovYDir)
{
  if (cameraIsSet_)
    throw std::runtime_error("Camera is already set!");

  camera_ = Camera{lookFrom, lookAt, upVector, fovYDir};
  cameraIsSet_ = true;
}