#pragma once

#include "Common.h"
#include "Vector3D.h"

class Ray
{
  Ray(const Point3D &viewPoint, const Vector3D &direction);

private:
  Point3D viewPoint_;
  Vector3D unitDirection_;
};