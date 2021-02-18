#pragma once

#include <optional>

#include "../Ray.h"

class RayShapeIntersector
{
public:
  RayShapeIntersector() = default;

  void calculateIntersectionPointWithRay(const Ray &ray);

  bool doesIntersectionPointExist() const;
  const Point3D &getIntersectionPoint() const;
  float getIntersectionPointDistanceToOrigin() const;
  const Vector3D &getUnitNormalOfShapeAtIntersectionPoint() const;

  virtual ~RayShapeIntersector() = default;

protected:
  void setIntersectionPoint_(const Point3D &point);
  void setIntersectionPointDistanceToOrigin_(float t);
  void setUnitNormalOfShape_(const Vector3D &unitNormal);
  virtual void doCalculateIntersectionPointWithRay(const Ray &ray) = 0;

private:
  // Private methods
  void resetIntersector_();

  // Private attributes
  Vector3D unitNormal_;
  std::optional<Point3D> intersectionPoint_;
  float rayParameterAtIntersection_;
};

