#include "ClosestIntersectedShapeFinder.h"

#include "shapes/RayShapeIntersectorFactory.h"

//ClosestIntersectedShapeFinder::ClosestIntersectedShapeFinder()
//{
//  setIntersectionParametersToDefault_();
//}

void ClosestIntersectedShapeFinder::buildIntersectorMap(
  const std::vector<std::unique_ptr<const Shape>> &shapes)
{
  for (const auto &pShape : shapes)
  {
    shapeIntersectorMap_.try_emplace(pShape.get(),
      RayShapeIntersectorFactory::createIntersector(*pShape));
  }
}

IntersectionInfo ClosestIntersectedShapeFinder::getIntersectionInfoAtCloesestIntersectionPoint(
  const Ray &ray) const
{
  auto intersectionInfo = IntersectionInfo{};
  auto closestIntersectionPointDistance_ = std::numeric_limits <float>::max();

  for (const auto &[pShape, pShapeIntersector] : shapeIntersectorMap_)
  {
    const auto currentIntersectionInfo = pShapeIntersector->getIntersectionInfo(ray);

    if (currentIntersectionInfo.doesIntersectionPointExist())
    {
      const auto distance = currentIntersectionInfo.getIntersectionPointDistanceToOriginOfRay();
      if (distance < closestIntersectionPointDistance_)
      {
        closestIntersectionPointDistance_ = distance;
        intersectionInfo = currentIntersectionInfo;
      }      
    }
  }
  
  return intersectionInfo;
}