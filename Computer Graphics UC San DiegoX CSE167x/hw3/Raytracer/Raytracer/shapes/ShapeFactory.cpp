#include "ShapeFactory.h"



std::unique_ptr<Triangle> ShapeFactory::createTriangle(
  const MateriaPropertiesAndAmbient &matProperties,
  const TransformationMatrix &transformationMatrix,
  const std::array<Point3D, 3> &vertices)
{
  return std::make_unique<Triangle>(matProperties, transformationMatrix, vertices);
}

std::unique_ptr<Sphere> ShapeFactory::createSphere(
  const MateriaPropertiesAndAmbient &matProperties,
  const TransformationMatrix &transformationMatrix,
  const Point3D center,
  float radius)
{
  return std::make_unique<Sphere>(matProperties, transformationMatrix, center, radius);
}