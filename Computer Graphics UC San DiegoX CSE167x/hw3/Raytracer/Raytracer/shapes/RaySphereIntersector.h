#pragma once

#include <optional>

#include "RayShapeIntersector.h"
#include "Sphere.h"

class RaySphereIntersector : public RayShapeIntersector
{
public:
  RaySphereIntersector(const Sphere &sphere);

protected:
  void doCalculateIntersectionPointWithRay(const Ray &ray) override final;

private:
  // Private methods
  std::optional<std::pair<float, float>> calculateRoots_(
    const Point3D &p0, const Vector3D &p1) const;
  std::pair<Point3D, Vector3D> getActualIntersectionPointAndNormal_(
    const Point3D &p0, const Vector3D &p1, float smallestPositiveRoot) const;

  // Private attributes
  const Sphere &sphere_;
  const TransformationMatrix &transformationMat_;
  const TransformationMatrix inverseTransform_;
};

