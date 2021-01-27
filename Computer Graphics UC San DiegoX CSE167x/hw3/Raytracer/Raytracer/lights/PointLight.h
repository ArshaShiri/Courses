#pragma once

#include "../Common.h"
#include "Light.h"

class PointLight : public Light
{
public:
  PointLight(const Point3D &point, const Color &rgb);

private:
  Point3D point_;
};

