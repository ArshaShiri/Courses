#include <array>
#include <stdexcept>
#include <string>

#include "Scene.h"

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

  camera_.lookFrom[0] = lookFrom.at(0);
  camera_.lookFrom[1] = lookFrom.at(1);
  camera_.lookFrom[2] = lookFrom.at(2);

  camera_.lookAt[0] = lookAt.at(0);
  camera_.lookAt[1] = lookAt.at(1);
  camera_.lookAt[2] = lookAt.at(2);

  camera_.upVector[0] = upVector.at(0);
  camera_.upVector[1] = upVector.at(1);
  camera_.upVector[2] = upVector.at(2);

  camera_.fovYDir = fovYDir;
  cameraIsSet_ = true;
}