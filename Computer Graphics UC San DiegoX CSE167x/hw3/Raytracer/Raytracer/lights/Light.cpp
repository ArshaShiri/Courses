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

Color Light::getContributionOnObject(const ShapeProperties &shapeProperties,
                                     const Vector3D &incomingRayUnitDir,
                                     const Vector3D &shadowRayUnitDir) const
{
  auto color = Color{};
 
  color += getDiffuseFactor_(shapeProperties, shadowRayUnitDir);
  color += getSpecularFactor_(shapeProperties,
                              incomingRayUnitDir,
                              shadowRayUnitDir);
  color /= getAttenuationFactor_(shapeProperties.distanceToLight);

  return color_ * color;
}

Color Light::getDiffuseFactor_(
  const ShapeProperties &shapeProperties,
  const Vector3D &shadowRayUnitDir) const
{
  // D * max(N . L, 0) where:
  // D is the diffuse term
  // N is the surface normal
  // L is the direction to the light
  const auto &diffuse = shapeProperties.matProp.diffuse;
  const auto rightTerm = std::max(shadowRayUnitDir.dot(shapeProperties.objectUnitNormal), 0.0f);

  return diffuse * rightTerm;
}

Color Light::getSpecularFactor_(
  const ShapeProperties &shapeProperties,
  const Vector3D &incomingRayDirection,
  const Vector3D &shadowRayUnitDir) const
{
  // S * max(N . H, 0) ^ s where:
  // S is the specular term
  // N is the surface normal
  // H is the half-angle
  // s is shininess
  const auto shininess = shapeProperties.matProp.shininess;
  const auto &specular = shapeProperties.matProp.specular;
  const auto &objectUnitNormal = shapeProperties.objectUnitNormal;

  const auto halfAngle = (shadowRayUnitDir - incomingRayDirection).normalize();
  const auto rightTerm = pow(std::max(halfAngle.dot(objectUnitNormal), 0.0f), shininess);

  return specular * rightTerm;
}