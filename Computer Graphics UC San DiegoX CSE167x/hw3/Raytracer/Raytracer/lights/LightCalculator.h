/*Class to calculate the contribution of all lights on an object where the incoming ray 
  intersects it. */

#pragma once

#include <memory>
#include <vector>

#include "Light.h"

#include "../ClosestIntersectedShapeFinder.h"

class LightCalculator
{
public:
  LightCalculator(const std::vector<std::unique_ptr<const Light>> &lights,
                  const ClosestIntersectedShapeFinder &closestIntersectedShapeFinder);

  Color getLightsContribution(
    const Vector3D &incomingRayUnitDir,
    const IntersectionInfo &intersectionInfo) const;

private:
  // Private methods
  bool doesShadowRayReachTheCurrentLight_(
    const IntersectionInfo &shadowRayIntersectionInfo,
    const Point3D &currentIntersection,
    float intersectionPointDistanceToLight) const;

  // Private attributes
  const std::vector<std::unique_ptr<const Light>> &lights_;
  const ClosestIntersectedShapeFinder &closestIntersectedShapeFinder_;
};

