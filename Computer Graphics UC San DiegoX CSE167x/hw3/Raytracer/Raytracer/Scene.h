/*Defines the scene*/

#pragma once

#include "GLMWrapper.h"

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
  struct Camera
  {
    GLM::Vec3D lookFrom;
    GLM::Vec3D lookAt;
    GLM::Vec3D upVector;

    float fovYDir;
  };

  // Private attributes
  int height_;
  int width_;

  Camera camera_;
  bool cameraIsSet_;
};