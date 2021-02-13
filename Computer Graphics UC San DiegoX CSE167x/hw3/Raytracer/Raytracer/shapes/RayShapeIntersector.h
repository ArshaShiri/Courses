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
  float getRayParameter() const;

  virtual ~RayShapeIntersector() = default;

protected:
  void setIntersectionPoint_(const Point3D &point);
  void setRayParameter_(float t);

private:
  std::optional<Point3D> intersectionPoint_;
  float rayParameterAtIntersection_;
};

