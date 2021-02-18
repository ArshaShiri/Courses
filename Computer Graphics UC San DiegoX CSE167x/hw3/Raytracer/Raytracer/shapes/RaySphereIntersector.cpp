
#include "../Common.h"
#include "../GLMWrapper.h"
#include "RaySphereIntersector.h"

namespace
{
class TransformedRayAccessor
{
public:
  TransformedRayAccessor(const Ray &ray, const TransformationMatrix &matrix);
  const Point3D &getViewPoint() const;
  const Vector3D &getUnitDirection() const;

private:
  Ray transformedRay_;
};
} // End of namespace declaration

RaySphereIntersector::RaySphereIntersector(const Sphere &sphere) : 
  RayShapeIntersector{},
  sphere_ {sphere},
  transformationMat_{sphere_.getTransformationMatrix()},
  inverseTransform_{GLMWrapper::GLM::getInversed(transformationMat_)}
{}

void RaySphereIntersector::doCalculateIntersectionPointWithRay(const Ray &ray)
{
  // We first transfer the ray via the inverse transformation assigned to the sphere.
  // We continue the calculation with the transformed ray.
  const auto transformedRayAccessor = TransformedRayAccessor(ray, inverseTransform_);

  // The equation of a sphere with radius r is given by:
  // (p - C) . (p - C) - r * r = 0, P is an arbitrary position point on the sphere and c is the 
  // center of sphere.
  // The equation of the ray is:
  // P = P0 + P1 * t
  // In order to find the intersection point we have to find t.
  // t can be found by solving: 
  // t ^ 2 * (p1 . p1) + 2 * t * p1 . (p0 - C) + (p0 - C) . (p0 - C) - r ^ 2 = 0.
  // This quadratic equation can be simplified to: a * t ^ 2 + b * t + c. 
  const auto &p0 = transformedRayAccessor.getViewPoint();
  const auto &p1 = transformedRayAccessor.getUnitDirection();
  const auto roots = calculateRoots_(p0, p1);

  if (roots == std::nullopt)
    return;

  const auto r1 = roots.value().first;
  const auto r2 = roots.value().second;

  const auto rootsAreNegative = (r1 < 0.0f) && (r2 < 0.0f);
  const auto rootsAreEqual = abs(r1 - r2) < GEOMETRY_TOLERANCE;

  if (rootsAreNegative || rootsAreEqual)
    return;

  const auto minRoot = std::min(r1, r2);
  const auto smallestPositiveRoot = minRoot > 0.f ? minRoot : std::max(r1, r2);
  const auto actualIntersectionPointAndNormal = 
    getActualIntersectionPointAndNormal_(p0, p1, smallestPositiveRoot);

  const auto &actualNormal = actualIntersectionPointAndNormal.second;
  const auto &actualIntersectionPoint = actualIntersectionPointAndNormal.first;

  setUnitNormalOfShape_(actualNormal.normalize());
  setIntersectionPoint_(actualIntersectionPoint);
  setIntersectionPointDistanceToOrigin_(actualIntersectionPoint.distance(ray.getViewPoint()));
}

std::optional<std::pair<float, float>> RaySphereIntersector::calculateRoots_(
  const Point3D &p0, const Vector3D &p1) const
{
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
    return std::nullopt;

  // Roots are calculated as:
  // sqrtDiscriminant = sqrt(discriminant)
  // r1 = (-b + sqrtDiscriminant) / 2 * a
  // r2 = (-b - sqrtDiscriminant) / 2 * a
  const auto aDoubled = 2.0f * a;
  const auto sqrtDiscriminant = sqrt(discriminant);
  const auto r1 = (-b + sqrtDiscriminant) / aDoubled;
  const auto r2 = (-b - sqrtDiscriminant) / aDoubled;

  return std::make_pair(r1, r2);
}

std::pair<Point3D, Vector3D> RaySphereIntersector::getActualIntersectionPointAndNormal_(
  const Point3D &p0, const Vector3D &p1, float smallestPositiveRoot) const
{
  using namespace GLMWrapper;

  // The actual intersection point is calculated by transforming the intersection point of the 
  // transformed ray.
  const auto transformedIntersectionPoint = p0 + p1 * smallestPositiveRoot;
  const auto actualIntersectionPoint = GLM::TransformPoint(transformationMat_,
                                                           transformedIntersectionPoint);

  const auto transformedNormal = transformedIntersectionPoint - sphere_.getCenter();
  const auto actualNormal = GLM::TransformVector(inverseTransform_.getTransposed(),
                                                 transformedNormal);

  return std::make_pair(actualIntersectionPoint, actualNormal);
}

namespace
{
TransformedRayAccessor::TransformedRayAccessor(const Ray &ray, const TransformationMatrix &matrix)
{
  const auto transformedRay = GLMWrapper::GLM::getTransformedRay(ray, matrix);
}

const Point3D &TransformedRayAccessor::getViewPoint() const
{
  return transformedRay_.getViewPoint();
}

const Vector3D &TransformedRayAccessor::getUnitDirection() const
{
  return transformedRay_.getUnitDirection();
}
} // End of namespace definition