/*Class to query the closest intersected shape with a given ray. The shapes are used to 
  create RayShapeIntersector objects and cached. Each RayShapeIntersector can be used
  to query if a given ray intersect its underlying shape. */

#pragma once

#include <memory>
#include <unordered_map>

#include "shapes/RayShapeIntersector.h"
#include "shapes/Shape.h"

using ShapeIntersectorMap = std::unordered_map<const Shape*, 
                                               std::unique_ptr<const RayShapeIntersector>>;
class ClosestIntersectedShapeFinder
{
public:
  ClosestIntersectedShapeFinder() = default;

  void buildIntersectorMap(const std::vector<std::unique_ptr<const Shape>> &shapes);
  IntersectionInfo getIntersectionInfoAtCloesestIntersectionPoint(const Ray &ray) const;

private:
  ShapeIntersectorMap shapeIntersectorMap_;
};

