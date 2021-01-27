#include "Triangle.h"

Triangle::Triangle(const MateriaPropertiesAndAmbient &matProperties,
                   const std::vector<Point3D> &vertices,
                   const std::array<int, 3> &cornerNodeIndices) :
  Shape{matProperties, vertices},
  cornerNodeIndices_{cornerNodeIndices}
{}

std::optional<Point3D> Triangle::getIntersection(const Ray &ray)
{
  return std::nullopt;
}