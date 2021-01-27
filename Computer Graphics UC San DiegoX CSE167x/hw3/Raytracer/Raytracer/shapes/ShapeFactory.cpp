#include "ShapeFactory.h"



std::unique_ptr<Triangle> ShapeFactory::createTriangle(
  const MateriaPropertiesAndAmbient &matProperties,
  const std::vector<Point3D> &vertices,
  const std::array<int, 3> &cornerNodeIndices)
{
  return std::make_unique<Triangle>(matProperties, vertices, cornerNodeIndices);
}