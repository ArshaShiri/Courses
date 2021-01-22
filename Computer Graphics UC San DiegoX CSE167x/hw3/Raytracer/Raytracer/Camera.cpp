#include "Camera.h"

Camera::Camera(const std::array<float, 3> &lookFrom,
               const std::array<float, 3> &lookAt,
               const std::array<float, 3> &upVector,
               float fovYDir) : 
  lookFrom_{lookFrom},
  lookAt_{lookAt},
  upVector_{upVector},
  fovYDir_{fovYDir}
{
  calculateCameraCoordinatesSystem_();
}

void Camera::calculateCameraCoordinatesSystem_()
{
  w_ = (lookFrom_ - lookAt_).normalize();
  u_ = upVector_.cross(w_);
  v_ = w_.cross(v_);
}