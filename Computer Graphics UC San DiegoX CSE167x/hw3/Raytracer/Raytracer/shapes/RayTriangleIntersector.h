/*Base class for intersecting a ray with a perimitive. The ray equation is: P = P0 + P1 * t */

#pragma once

#include "RayShapeIntersector.h"
#include "Triangle.h"

class RayTriangleIntersector :  public RayShapeIntersector
{
public:
  RayTriangleIntersector(const Triangle &triangle);
  
protected:
  void doCalculateIntersectionPointWithRay(const Ray &ray) override final;

private:
  const Triangle &triangle_;
};