#include "LightCalculator.h"

LightCalculator::LightCalculator(const std::vector<std::unique_ptr<const Light>> &lights,
                                 const ClosestIntersectedShapeFinder &closestIntersectedShapeFinder) :
  lights_{lights},
  closestIntersectedShapeFinder_{closestIntersectedShapeFinder}
{}

Color LightCalculator::getLightsContribution(
  const Vector3D &incomingRayUnitDir,
  const IntersectionInfo &intersectionInfo) const
{
  auto color = Color{};

  const auto pCurrentShape = intersectionInfo.getUnderlyingShape();
  const auto &shapeMatProperties = pCurrentShape->getMateriaPropertiesAndAmbient();
  const auto &currentIntersection = intersectionInfo.getIntersectionPoint();
  const auto &unitNormalAtIntersectionPoint = intersectionInfo.getUnitNormalOfShapeAtIntersectionPoint();

  // Now calculate the contribution of all lights.
  for (const auto &pLight : lights_)
  {
    const auto intersectionPointDistanceToLight = pLight->getDistanceToPoint(currentIntersection);
    const auto shadowRay = pLight->getRayTowardsLightFromPoint(currentIntersection);    
    const auto &shadowRayUnitDir = shadowRay.getUnitDirection();
    const auto shapeProperties = ShapeProperties{shapeMatProperties,
                                                 unitNormalAtIntersectionPoint,
                                                 intersectionPointDistanceToLight};
    const auto shadowRayIntersectionInfo = 
      closestIntersectedShapeFinder_.getIntersectionInfoAtCloesestIntersectionPoint(shadowRay);

    const auto doesShadowRayReachTheCurrentLight = 
      doesShadowRayReachTheCurrentLight_(shadowRayIntersectionInfo,
                                         currentIntersection,
                                         intersectionPointDistanceToLight);

    if (doesShadowRayReachTheCurrentLight)
      color += pLight->getContributionOnObject(shapeProperties,
                                               incomingRayUnitDir,
                                               shadowRayUnitDir);
  }

  return color;
}

bool LightCalculator::doesShadowRayReachTheCurrentLight_(
  const IntersectionInfo &shadowRayIntersectionInfo,
  const Point3D &currentIntersection,
  float intersectionPointDistanceToLight) const
{
  if (shadowRayIntersectionInfo.doesIntersectionPointExist())
  {
    const auto &shadowRayIntersectionPoint = shadowRayIntersectionInfo.getIntersectionPoint();
    const auto distanceToIntersectionPoint = shadowRayIntersectionPoint.distance(currentIntersection);

    // Check if shadow ray can reach the light before intersecting any other shape.
    return intersectionPointDistanceToLight < distanceToIntersectionPoint;
  }

  // Shadow ray does not intersect any other ray and can reach the light.
  return true;
}