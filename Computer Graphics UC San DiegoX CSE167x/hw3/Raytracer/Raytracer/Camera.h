/* Represents the camera. The camera is located at lookFrom_. It looks in the direction of -w. 
The image plane is 1 unit away from the camera location in w direction (at -w).*/

#pragma once

#include <array>

#include "Common.h"
#include "Ray.h"
#include "Vector3D.h"

class Camera
{
public:
  Camera() = default;
  void setCamerProperties(const std::array<float, 3> &lookFrom,
                          const std::array<float, 3> &lookAt,
                          const std::array<float, 3> &upVector,
                          float fovYDir);
  void setHeightAndWidthOfImagePlane(int height, int width);

  Ray calculateRayThroughPixel(int height, int width);

private:
  void calculateCameraCoordinatesSystem_();

  // Private attributes
  int height_;
  int width_;

  Point3D lookFrom_;
  Point3D lookAt_;
  Vector3D upVector_;

  // The normalized coordinates system at the location of the camera.
  Vector3D w_;
  Vector3D u_;
  Vector3D v_;

  float fovY_;
};