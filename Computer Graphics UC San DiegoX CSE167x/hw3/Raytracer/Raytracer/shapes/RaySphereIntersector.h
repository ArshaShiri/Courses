#pragma once

#include "RayShapeIntersector.h"
#include "Sphere.h"

class RaySphereIntersector : public RayShapeIntersector
{
public:
  RaySphereIntersector(const Sphere &sphere);
  void calculateIntersectionPointWithRay(const Ray &ray) override final;

private:
  const Sphere &sphere_;
};

