#pragma once

#include "Common.h"
#include "Vector3D.h"

class Ray
{
  Ray(const Point &viewPoint, const Vector3D &direction);

private:
  Point viewPoint_;
  Vector3D unitDirection_;
};