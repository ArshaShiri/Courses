#include "ShapeFactory.h"



std::unique_ptr<Triangle> ShapeFactory::createTriangle(
  const MateriaPropertiesAndAmbient &matProperties,
  const std::array<Point3D, 3> &vertices)
{
  return std::make_unique<Triangle>(matProperties, vertices);
}

std::unique_ptr<Sphere> ShapeFactory::createSphere(
  const MateriaPropertiesAndAmbient &matProperties,
  const Point3D center,
  float radius)
{
  return std::make_unique<Sphere>(matProperties, center, radius);
}