
#include "RayShapeIntersector.h"


bool RayShapeIntersector::doesIntersectionPointExist() const
{
  return intersectionPoint_ != std::nullopt;
}

const Point3D &RayShapeIntersector::getIntersectionPoint() const
{
  if (!doesIntersectionPointExist())
    throw std::runtime_error("Intersection does not exist!");

  return intersectionPoint_.value();
}

float RayShapeIntersector::getIntersectionPointDistanceToOrigin() const
{
  if (!doesIntersectionPointExist())
    throw std::runtime_error("Intersection does not exist!");

  return rayParameterAtIntersection_;
}

const Vector3D &RayShapeIntersector::getUnitNormalOfShapeAtIntersectionPoint() const
{
  return unitNormal_;
}

void RayShapeIntersector::resetIntersector()
{
  intersectionPoint_ = std::nullopt;
}

void RayShapeIntersector::setIntersectionPoint_(const Point3D &point)
{
  intersectionPoint_ = point;
}

void RayShapeIntersector::setIntersectionPointDistanceToOrigin_(float t)
{
  rayParameterAtIntersection_ = t;
}

void RayShapeIntersector::setUnitNormalOfShape_(const Vector3D &unitNormal)
{
  unitNormal_ = unitNormal;
}