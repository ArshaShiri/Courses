#include "Light.h"

Light::Light(const Color &rgb) : color_{rgb}
{}

// The moving of the point towards the given direction, can be used in order to avoid self
// intersecting shadow rays.
Point3D Light::slighltyMovePointTowardsDirection_(const Point3D &point, const Vector3D &dir) const
{
  const auto slightMovementFactor = 0.001f;
  return point + dir.normalize() * slightMovementFactor;
}

Color Light::getContributionOnObject(const MateriaPropertiesAndAmbient &matProp,
                                     const Vector3D &incomingRayDir,
                                     const Vector3D &objectUnitNormal,
                                     const Ray &shadowRay,
                                     float distanceToLight) const
{
  auto color = Color{};
  const auto &lightDir = shadowRay.getUnitDirection();

  color += getDiffuseFactor_(matProp.diffuse, objectUnitNormal, lightDir);
  color += getSpecularFactor_(matProp.specular,
                              matProp.shininess,
                              incomingRayDir,
                              objectUnitNormal,
                              lightDir);
  color /= getAttenuationFactor_(distanceToLight);

  return color_ * color;
}

Color Light::getDiffuseFactor_(
  const Color &diffuse,
  const Vector3D &objectUnitNormal,
  const Vector3D &lightDirection) const
{
  // D * max(N . L, 0) where:
  // D is the diffuse term
  // N is the surface normal
  // L is the direction to the light
  const auto green = diffuse.getGreen();
  const auto rightTerm = std::max(lightDirection.dot(objectUnitNormal), 0.0f);

  return diffuse * rightTerm;
}

Color Light::getSpecularFactor_(
  const Color &specular,
  float shininess,
  const Vector3D &incomingRayDirection,
  const Vector3D &objectUnitNormal,
  const Vector3D &lightDirection) const
{
  // S * max(N . H, 0) ^ s where:
  // S is the specular term
  // N is the surface normal
  // H is the half-angle
  // s is shininess

  const auto halfAngle = (lightDirection - incomingRayDirection).normalize();
  const auto rightTerm = pow(std::max(halfAngle.dot(objectUnitNormal), 0.0f), shininess);

  return specular * rightTerm;
}