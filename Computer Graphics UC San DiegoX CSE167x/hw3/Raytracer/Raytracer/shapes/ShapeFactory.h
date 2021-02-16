#pragma once

#include <array>
#include <memory>

#include "../Common.h"
#include "Sphere.h"
#include"Triangle.h"



class ShapeFactory
{
public:
  static std::unique_ptr<Triangle> createTriangle(const MateriaPropertiesAndAmbient &matProperties,
                                                  const std::array<Point3D, 3> &vertices);

  static std::unique_ptr<Sphere> createSphere(const MateriaPropertiesAndAmbient &matProperties,
                                              const Point3D center,
                                              float radius);
};

