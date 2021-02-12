#include "RayShapeIntersectorFactory.h"
#include "RayTriangleIntersector.h"
#include "Triangle.h"

std::unique_ptr<RayShapeIntersector> RayShapeIntersectorFactory::createIntersector(
  const Shape &shape)
{
  switch (shape.getType())
  {
  case ShapeType::Triangle:
    return std::make_unique<RayTriangleIntersector>(dynamic_cast<const Triangle&>(shape));

  default:
    throw std::runtime_error("Shape is unknown!");
  }
}