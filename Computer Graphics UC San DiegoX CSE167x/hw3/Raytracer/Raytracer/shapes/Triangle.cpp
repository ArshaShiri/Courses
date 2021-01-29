#include "Triangle.h"

#include "../Common.h"

Triangle::Triangle(const MateriaPropertiesAndAmbient &matProperties,
                   const std::vector<Point3D> &vertices,
                   const std::array<int, 3> &cornerNodeIndices) :
  Shape{matProperties, vertices},
  cornerNodeIndices_{cornerNodeIndices}
{
  const auto normal = (C_() - A_()).cross*(B_() - A_());
  unitNormal_ = normal.normalize();
}

std::optional<Point3D> Triangle::getIntersection(const Ray &ray)
{
  // The plane equation in which the triangle lies is:
  // P.n - A.n = 0, where n is unit normal, P is an arbitrary position point in the plane and A is 
  // the position vector of one of the corners of the triangle.
  // The equation of the ray is:
  // P = P0 + P1 * t
  // The intersection point of the plane and the given ray is located at:
  // t = (A.n - p0.n) / (P.n)

  const auto &p1 = ray.getUnitDirection();
  const auto PDotN = p1.dot(unitNormal_);
  const auto isRayIntersectingTrianglePlane = PDotN < GEOMETRY_TOLERANCE;

  if (isRayIntersectingTrianglePlane)
    return std::nullopt;

  const auto &p0 = ray.getViewPoint();
  const auto t = (A_() - p0).dot(unitNormal_) / p1.dot(unitNormal_);

  return std::nullopt;
}
