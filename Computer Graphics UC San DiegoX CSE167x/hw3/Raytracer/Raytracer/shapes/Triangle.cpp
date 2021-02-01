#include "Triangle.h"

#include "../Common.h"

Triangle::Triangle(const MateriaPropertiesAndAmbient &matProperties,
                   const std::vector<Point3D> &vertices,
                   const std::array<int, 3> &cornerNodeIndices) :
  Shape{matProperties, vertices},
  cornerNodeIndices_{cornerNodeIndices}
{
  const auto normal = (C_() - A_()).cross(B_() - A_());
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
  const auto P1DotN = p1.dot(unitNormal_);
  const auto isRayParallelToTrianglePlane = P1DotN < GEOMETRY_TOLERANCE;

  if (isRayParallelToTrianglePlane)
    return std::nullopt;

  const auto &p0 = ray.getViewPoint();
  const auto t = (A_() - p0).dot(unitNormal_) / p1.dot(unitNormal_);

  const auto isIntersectionInNegativeDirectionOfRay = t < 0.0;
  if (isIntersectionInNegativeDirectionOfRay)
    return std::nullopt;

  const auto intersectionPoint = p0 + p1 * t;

  if (isIntersectionPointInTriangle_(intersectionPoint))
    return intersectionPoint;

  return std::nullopt;
}

bool Triangle::isIntersectionPointInTriangle_(const Point3D &intersectionPoint)
{
  // In order to check if the found intersection of ray and the plane is actually inside the 
  // triangle, we need to calculate the barycentric coordinates.
  // Based on https://gamedev.stackexchange.com/ and 
  // Christer Ericson's Real-Time Collision Detection
  const auto v0 = B_() - A_();
  const auto v1 = C_() - A_();
  const auto v2 = intersectionPoint - A_();

  const auto d00 = v0.dot(v0);
  const auto d01 = v0.dot(v1);
  const auto d11 = v1.dot(v1);
  const auto d20 = v2.dot(v0);
  const auto d21 = v2.dot(v1);
  const auto denom = d00 * d11 - d01 * d01;

  const auto alpha = (d11 * d20 - d01 * d21) / denom;
  const auto beta = (d00 * d21 - d01 * d20) / denom;
  const auto gamma = 1.0f - alpha - beta;

  const auto isAlphaBetweenZeroAndOne = (alpha >= 0.0) && (alpha <= 1.0);
  const auto isBetaBetweenZeroAndOne = (beta >= 0.0) && (beta <= 1.0);
  const auto isGammaaBetweenZeroAndOne = (gamma >= 0.0) && (gamma <= 1.0);

  return isAlphaBetweenZeroAndOne &&
         isBetaBetweenZeroAndOne  &&
         isGammaaBetweenZeroAndOne;
}