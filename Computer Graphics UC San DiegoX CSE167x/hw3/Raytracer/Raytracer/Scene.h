/*Defines the scene*/

#pragma once

#include "Camera.h"

class Scene
{
public:
  Scene();
  void setWindowSize(int height, int width);
  void setCamera(const std::array<float, 3> &lookFrom,
                 const std::array<float, 3> &lookAt,
                 const std::array<float, 3> &upVector,
                 float fovYDir);

private:
  // Private attributes
  int height_;
  int width_;

  Camera camera_;
  bool cameraIsSet_;
};