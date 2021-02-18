#pragma once

#include <optional>

#include "../Ray.h"

class RayShapeIntersector
{
public:
  RayShapeIntersector() = default;

  virtual void calculateIntersectionPointWithRay(const Ray &ray) = 0;

  bool doesIntersectionPointExist() const;
  const Point3D &getIntersectionPoint() const;
  float getIntersectionPointDistanceToOrigin() const;
  const Vector3D &getUnitNormalOfShapeAtIntersectionPoint() const;

  virtual ~RayShapeIntersector() = default;

protected:
  void setIntersectionPoint_(const Point3D &point);
  void setIntersectionPointDistanceToOrigin_(float t);
  void setUnitNormalOfShape_(const Vector3D &unitNormal);

private:
  Vector3D unitNormal_;
  std::optional<Point3D> intersectionPoint_;
  float rayParameterAtIntersection_;
};

