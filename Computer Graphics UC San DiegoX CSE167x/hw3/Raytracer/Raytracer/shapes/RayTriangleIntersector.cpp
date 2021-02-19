#include "RayTriangleIntersector.h"

RayTriangleIntersector::RayTriangleIntersector(const Triangle &triangle) :
  RayShapeIntersector{},
  triangle_{triangle}
{
  const auto normal = (triangle_.B() - triangle_.A()).cross(triangle_.C() - triangle_.A());
  unitNormal_ = normal.normalize();
}

IntersectionInfo RayTriangleIntersector::getIntersectionInfo(const Ray &ray) const
{
  // The plane equation in which the triangle lies is:
  // P.n - A.n = 0, where n is unit normal, P is an arbitrary position point in the plane and A is 
  // the position vector of one of the corners of the triangle.
  // The equation of the ray is:
  // P = P0 + P1 * t
  // The intersection point of the plane and the given ray is located at:
  // t = (A.n - p0.n) / (P.n)

  const auto &p1 = ray.getUnitDirection();
  const auto P1DotN = p1.dot(unitNormal_);
  const auto isRayParallelToTrianglePlane = abs(P1DotN - 0.0f) < GEOMETRY_TOLERANCE;

  if (isRayParallelToTrianglePlane)
    return {};

  const auto &p0 = ray.getViewPoint();
  const auto t = (triangle_.A() - p0).dot(unitNormal_) / p1.dot(unitNormal_);

  const auto isIntersectionInNegativeDirectionOfRay = t < 0.0;
  if (isIntersectionInNegativeDirectionOfRay)
    return {};

  const auto intersectionPoint = p0 + p1 * t;

  auto intersectionInfo = IntersectionInfo{};
  if (triangle_.isPointInTriangle(intersectionPoint))
  {
    intersectionInfo.setIntersectionPoint_(intersectionPoint);
    intersectionInfo.setUnitNormalOfShape_(unitNormal_);
    intersectionInfo.setIntersectionPointDistanceToLookFrom_(intersectionPoint.distance(p0));
  }

  return intersectionInfo;
}