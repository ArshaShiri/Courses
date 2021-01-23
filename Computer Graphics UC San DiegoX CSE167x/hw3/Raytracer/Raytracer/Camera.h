#pragma once

#include <array>

#include "Common.h"
#include "Vector3D.h"

class Camera
{
public:
  Camera() = default;
  Camera(const std::array<float, 3> &lookFrom,
         const std::array<float, 3> &lookAt,
         const std::array<float, 3> &upVector,
         float fovYDir);

private:
  void calculateCameraCoordinatesSystem_();

  Vector3D lookFrom_;
  Vector3D lookAt_;
  Vector3D upVector_;

  // The normalized coordinates system at the location of the camera.
  Vector3D w_;
  Vector3D u_;
  Vector3D v_;

  float fovYDir_;
};