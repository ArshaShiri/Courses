
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

float RayShapeIntersector::getRayParameter() const
{
  if (!doesIntersectionPointExist())
    throw std::runtime_error("Intersection does not exist!");

  return rayParameterAtIntersection_;
}

void RayShapeIntersector::setIntersectionPoint_(const Point3D &point)
{
  intersectionPoint_ = point;
}

void RayShapeIntersector::setRayParameter_(float t)
{
  rayParameterAtIntersection_ = t;
}