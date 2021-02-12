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

protected:
  void setIntersectionPoint_(const Point3D &point);

private:
  std::optional<Point3D> intersectionPoint_;
};

