#include "ClosestIntersectedShapeFinder.h"

#include "shapes/RayShapeIntersectorFactory.h"

ClosestIntersectedShapeFinder::ClosestIntersectedShapeFinder()
{
  setIntersectionParametersToDefault_();
}

void ClosestIntersectedShapeFinder::buildIntersectorMap(
  const std::vector<std::unique_ptr<const Shape>> &shapes)
{
  for (const auto &pShape : shapes)
  {
    shapeIntersectorMap_.try_emplace(pShape.get(),
      RayShapeIntersectorFactory::createIntersector(*pShape));
  }
}

void ClosestIntersectedShapeFinder::intersectRay(const Ray &ray)
{
  setIntersectionParametersToDefault_();

  for (const auto &[pShape, pShapeIntersector] : shapeIntersectorMap_)
  {
    const auto intersectionInfo = pShapeIntersector->getIntersectionInfo(ray);

    if (intersectionInfo.doesIntersectionPointExist())
      updateClosestShapeIfPossible_(intersectionInfo, pShape);
  }
}

void ClosestIntersectedShapeFinder::updateClosestShapeIfPossible_(
  const IntersectionInfo &intersectionInfo,
  const Shape *pShape)
{
  const auto distance = intersectionInfo.getIntersectionPointDistanceToLookFrom();
  if (distance < closestIntersectionPointDistance_)
  {
    closestIntersectionPointDistance_ = distance;
    pClosestShape_ = pShape;
    closestIntersectionPoint_ = intersectionInfo.getIntersectionPoint();
    unitNormalAtIntersectionPoint_ = intersectionInfo.getUnitNormalOfShapeAtIntersectionPoint();
    isAnyOfTheShapesIntersectingWithRay_ = true;
  }
}

void ClosestIntersectedShapeFinder::setIntersectionParametersToDefault_()
{
  isAnyOfTheShapesIntersectingWithRay_ = false;
  closestIntersectionPointDistance_ = std::numeric_limits <float>::max();
}

bool ClosestIntersectedShapeFinder::isAnyOfTheShapesIntersectingWithRay() const
{
  return isAnyOfTheShapesIntersectingWithRay_;
}

const Shape *ClosestIntersectedShapeFinder::getClosestShape() const
{
  if (!isAnyOfTheShapesIntersectingWithRay())
    throw std::runtime_error("Intersection does not exist!");

  return pClosestShape_;
}

const Point3D &ClosestIntersectedShapeFinder::getClosestIntersectionPoint() const
{
  return closestIntersectionPoint_;
}

const Vector3D &ClosestIntersectedShapeFinder::getUnitNormalAtIntersectionPoint() const
{
  return unitNormalAtIntersectionPoint_;
}