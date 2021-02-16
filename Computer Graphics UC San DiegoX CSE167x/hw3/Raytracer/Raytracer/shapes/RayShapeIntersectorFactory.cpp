#include "RayShapeIntersectorFactory.h"
#include "RaySphereIntersector.h"
#include "RayTriangleIntersector.h"
#include "Sphere.h"
#include "Triangle.h"

std::unique_ptr<RayShapeIntersector> RayShapeIntersectorFactory::createIntersector(
  const Shape &shape)
{
  switch (shape.getType())
  {
  case ShapeType::Triangle:
    return std::make_unique<RayTriangleIntersector>(static_cast<const Triangle&>(shape));
  case ShapeType::Sphere:
    return std::make_unique<RaySphereIntersector>(static_cast<const Sphere&>(shape));

  default:
    throw std::runtime_error("Shape is unknown!");
  }
}