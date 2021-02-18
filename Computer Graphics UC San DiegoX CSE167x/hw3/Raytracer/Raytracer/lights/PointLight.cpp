#include "PointLight.h"

PointLight::PointLight(const Color &rgb, const Point3D &point, const Attenuation &attenuation) :
  Light{rgb},
  point_{point},
  attenuation_{attenuation}
{}

Ray PointLight::getRayTowardsLightFromPoint(const Point3D &point) const
{
  const auto dir = point_ - point;
  return{slighltyMovePointTowardsDirection_(point, dir), dir};
}

float PointLight::getDistanceToPoint(const Point3D &point) const
{
  return point.distance(point_);
}

float PointLight::getAttenuationFactor_(float distanceToLight) const
{
  const auto constTerm = attenuation_.constant;
  const auto linearTerm = attenuation_.linear;
  const auto quadraticTerm = attenuation_.quadratic;

  return constTerm + 
         linearTerm * distanceToLight + 
         quadraticTerm * distanceToLight * distanceToLight;
}