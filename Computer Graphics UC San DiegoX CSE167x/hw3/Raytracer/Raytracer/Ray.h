#pragma once

#include "Point3D.h"
#include "Vector3D.h"

class Ray
{
public:
  Ray(const Point3D &viewPoint, const Vector3D &direction);
  const Vector3D &getUnitDirection() const { return unitDirection_; }
  const Point3D &getViewPoint() const { return viewPoint_; }

private:
  // The equation of the ray is:
  // P = viewPoint_ + unitDirection_ * t
  Point3D viewPoint_;
  Vector3D unitDirection_;
};