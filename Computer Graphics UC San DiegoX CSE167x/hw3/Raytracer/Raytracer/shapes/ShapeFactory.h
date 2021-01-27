#pragma once

#include <memory>
#include <vector>

#include "../Common.h"
#include"Triangle.h"



class ShapeFactory
{
public:
  static std::unique_ptr<Triangle> createTriangle(const MateriaPropertiesAndAmbient &matProperties,
                                                  const std::vector<Point3D> &vertices,
                                                  const std::array<int, 3> &cornerNodeIndices);
};

