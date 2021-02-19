/*Base class for shapes that will support intersecting with a ray. */

#pragma once

#include <optional>

#include "../Ray.h"

class IntersectionInfo
{
public:
  IntersectionInfo() = default;
  bool doesIntersectionPointExist() const;
  const Point3D &getIntersectionPoint() const;
  const Vector3D &getUnitNormalOfShapeAtIntersectionPoint() const;
  float getIntersectionPointDistanceToLookFrom() const;

private:
  friend class RayTriangleIntersector;
  friend class RaySphereIntersector;

  void setIntersectionPoint_(const Point3D &point);
  void setIntersectionPointDistanceToLookFrom_(float t);
  void setUnitNormalOfShape_(const Vector3D &unitNormal);

  // Private attributes
  Vector3D unitNormal_;
  std::optional<Point3D> intersectionPoint_;
  float rayParameterAtIntersection_;
};

class RayShapeIntersector
{
public:
  RayShapeIntersector() = default;

  virtual IntersectionInfo getIntersectionInfo(const Ray &ray) const = 0;

  virtual ~RayShapeIntersector() = default;
};

