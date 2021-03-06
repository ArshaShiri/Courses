
#include "RayShapeIntersector.h"


bool IntersectionInfo::doesIntersectionPointExist() const
{
  return intersectionPoint_ != std::nullopt;
}

const Point3D &IntersectionInfo::getIntersectionPoint() const
{
  if (!doesIntersectionPointExist())
    throw std::runtime_error("Intersection does not exist!");

  return intersectionPoint_.value();
}

float IntersectionInfo::getIntersectionPointDistanceToOriginOfRay() const
{
  if (!doesIntersectionPointExist())
    throw std::runtime_error("Intersection does not exist!");

  return rayParameterAtIntersection_;
}

const Vector3D &IntersectionInfo::getUnitNormalOfShapeAtIntersectionPoint() const
{
  return unitNormal_;
}

const Shape *IntersectionInfo::getUnderlyingShape() const
{
  return underlyingShape_;
}

void IntersectionInfo::setIntersectionPoint_(const Point3D &point)
{
  intersectionPoint_ = point;
}

void IntersectionInfo::setIntersectionPointDistanceToLookFrom_(float t)
{
  rayParameterAtIntersection_ = t;
}

void IntersectionInfo::setUnitNormalOfShape_(const Vector3D &unitNormal)
{
  unitNormal_ = unitNormal;
}

void IntersectionInfo::setUnderlyingShape_(const Shape* underlyingShape)
{
  underlyingShape_ = underlyingShape;
}