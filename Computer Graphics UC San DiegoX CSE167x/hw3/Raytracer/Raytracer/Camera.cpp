#define _USE_MATH_DEFINES

#include <math.h>

#include "Camera.h"

void Camera::setCamerProperties(const std::array<float, 3> &lookFrom,
                                const std::array<float, 3> &lookAt,
                                const std::array<float, 3> &upVector,
                                float fovYDir)
{
  lookFrom_ = lookFrom;
  lookAt_ = lookAt;
  upVector_ = upVector;
  fovY_ = fovYDir * (float)M_PI / 180.0f;
  calculateCameraCoordinatesSystem_();
}

void Camera::setHeightAndWidthOfImagePlane(int height, int width)
{
  height_ = height; 
  width_ = width;
}

void Camera::calculateCameraCoordinatesSystem_()
{
  w_ = (lookFrom_ - lookAt_).normalize();
  u_ = upVector_.cross(w_);
  v_ = w_.cross(v_);
}

Ray Camera::calculateRayThroughPixel(int piXelHeight, int piXelWidth)
{
  // The image plane height goes from 0 to height_ (from top to bottom) and its width goes from 
  // 0 to width_ (from left to right). Since the image plane is defined with respect to camera's
  // location and via camera's coordinates system, we need to define the equation of a ray shot
  // from camera's location based on w u v orthonormal vectors.
  // The center of the image plane lies in -w with respect to camera's location. A pixel on the 
  // image plane at height i and width j can be represented in w u v coordinates system as:
  // (alpha * u, beta * v, -w) where alpha and beta are:
  // alpha = tan(fovx / 2) * ((j - w / 2) / (w / 2))
  // beta = tan(fovy / 2) * ((h / 2 - i) / (h / 2)
  // i and j depend on the pixel. However both terms tan(fovx / 2) / (w / 2)) and 
  // tan(fovy / 2) / (h / 2) can be precomputed.

  static auto tanHalfFovY = tan(fovY_ / 2.0f);
  static auto tanHalfFovX = tanHalfFovY * width_ / height_;

  static auto halfHeight = height_ / 2.0f;
  static auto halfWidth = width_ / 2.0f;

  static auto betaonst = tanHalfFovY / halfHeight;
  static auto alphaConst = tanHalfFovX / halfWidth;

  // The ray goes through the middle of the pixel.
  const auto midHeight = 0.5f * piXelHeight;
  const auto midWidth = 0.5f * piXelWidth;

  const auto beta = betaonst * (halfHeight - piXelHeight);
  const auto alpha = alphaConst * (halfWidth - piXelHeight);

  const auto rayDirection = u_ * alpha + v_ * beta - w_;
  
  return {lookFrom_, rayDirection.normalize()};
}