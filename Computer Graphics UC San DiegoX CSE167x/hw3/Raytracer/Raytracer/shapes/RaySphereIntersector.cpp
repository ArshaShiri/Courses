
#include "../Common.h"
#include "../GLMWrapper.h"
#include "RaySphereIntersector.h"

RaySphereIntersector::RaySphereIntersector(const Sphere &sphere) : 
  RayShapeIntersector{}, 
  sphere_ {sphere}
{}

void RaySphereIntersector::calculateIntersectionPointWithRay(const Ray &ray)
{
  const auto &transformationMat = sphere_.getTransformationMatrix();
  const auto inverseTransform = GLMWrapper::GLMWrapper::getInversed(transformationMat);
  const auto transformedRay = GLMWrapper::GLMWrapper::getTransformedRay(ray, inverseTransform);

  // The equation of a sphere with radius r is given by:
  // (p - C) . (p - C) - r * r = 0, P is an arbitrary position point on the sphere and c is the 
  // center of sphere.
  // The equation of the ray is:
  // P = P0 + P1 * t
  // In order to find the intersection point we have to find t.
  // t can be found by solving: 
  // t ^ 2 * (p1 . p1) + 2 * t * p1 . (p0 - C) + (p0 - C) . (p0 - C) - r ^ 2 = 0.
  // This quadratic equation can be simplified to: a * t ^ 2 + b * t + c. 

  const auto &p0 = transformedRay.getViewPoint();
  const auto &p1 = transformedRay.getUnitDirection();
  const auto &center = sphere_.getCenter();
  const auto radius = sphere_.getRadius();

  const auto origin = Point3D{0.0f, 0.0f, 0.0f};
  const auto centerPositionVector = center - origin;
  const auto p0PositionVector = p0 - origin;

  const auto a = p1.dot(p1);
  const auto b = 2.0f * (p1.dot(p0PositionVector) - p1.dot(centerPositionVector));
  const auto c = (p0 - center).dot(p0 - center) - radius * radius;

  const auto discriminant = b * b - (4.0f * a * c);

  // Complex roots
  if (discriminant < 0.0f)
    return;

  // Roots are calculated as:
  // sqrtDiscriminant = sqrt(discriminant)
  // r1 = (-b + sqrtDiscriminant) / 2 * a
  // r2 = (-b - sqrtDiscriminant) / 2 * a
  const auto aDoubled = 2.0f * a;
  const auto sqrtDiscriminant = sqrt(discriminant);
  const auto r1 = (-b + sqrtDiscriminant) / aDoubled;
  const auto r2 = (-b - sqrtDiscriminant) / aDoubled;

  // Equal roots 
  if (abs(r1 - r2) < GEOMETRY_TOLERANCE)
    return;

  const auto minRoot = std::min(r1, r2);
  const auto closestRoot = minRoot > 0.f ? minRoot : std::max(r1, r2);

  const auto actualIntersectionPoint = 
    GLMWrapper::GLMWrapper::TransformPoint(transformationMat, p0 + p1 * closestRoot);
  setIntersectionPoint_(actualIntersectionPoint);
  setIntersectionPointDistanceToOrigin_(actualIntersectionPoint.distance(ray.getViewPoint()));
}
 