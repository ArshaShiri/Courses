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
  ClosestIntersectedShapeFinder();

  void buildIntersectorMap(const std::vector<std::unique_ptr<const Shape>> &shapes);
  void intersectRay(const Ray &ray);

  bool isAnyOfTheShapesIntersectingWithRay() const;
  const Shape *getClosestShape() const;
  const Point3D &getClosestIntersectionPoint() const;
  const Vector3D &getUnitNormalAtIntersectionPoint() const;

private:
  // Private methods;
  void setIntersectionParametersToDefault_();
  void updateClosestShapeIfPossible_(const IntersectionInfo &intersectionInfo,
                                     const Shape *pShape);

  // Private attributes
  bool isAnyOfTheShapesIntersectingWithRay_;
  Point3D closestIntersectionPoint_;
  Vector3D unitNormalAtIntersectionPoint_;
  float closestIntersectionPointDistance_;
  const Shape *pClosestShape_;

  ShapeIntersectorMap shapeIntersectorMap_;
};

