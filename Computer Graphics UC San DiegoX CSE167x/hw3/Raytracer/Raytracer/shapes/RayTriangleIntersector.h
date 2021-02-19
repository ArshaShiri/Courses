/*Base class for intersecting a ray with a perimitive. The ray equation is: P = P0 + P1 * t */

#pragma once

#include "RayShapeIntersector.h"
#include "Triangle.h"

class RayTriangleIntersector :  public RayShapeIntersector
{
public:
  RayTriangleIntersector(const Triangle &triangle);
  
  IntersectionInfo getIntersectionInfo(const Ray &ray) const override final;

private:
  const Triangle &triangle_;
  Vector3D unitNormal_;
};