#pragma once

#include <memory>
#include <vector>

#include "../Common.h"
#include "Sphere.h"
#include"Triangle.h"



class ShapeFactory
{
public:
  static std::unique_ptr<Triangle> createTriangle(const MateriaPropertiesAndAmbient &matProperties,
                                                  const std::vector<Point3D> &vertices,
                                                  const std::array<int, 3> &cornerNodeIndices);

  static std::unique_ptr<Sphere> createSphere(const MateriaPropertiesAndAmbient &matProperties,
                                              const Point3D center,
                                              float radius);
};

