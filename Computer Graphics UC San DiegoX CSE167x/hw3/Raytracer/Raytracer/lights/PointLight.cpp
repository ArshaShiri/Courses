#include "PointLight.h"

PointLight::PointLight(const Point3D &point, const Color &rgb) :
  Light{rgb},
  point_{point}
{}