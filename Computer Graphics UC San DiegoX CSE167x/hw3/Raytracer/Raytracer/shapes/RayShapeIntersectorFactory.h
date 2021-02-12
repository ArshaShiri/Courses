#pragma once

#include <memory>

#include "RayShapeIntersector.h"
#include "Shape.h"

class RayShapeIntersectorFactory
{
public:
  static std::unique_ptr<RayShapeIntersector> createIntersector(const Shape &shape);
};