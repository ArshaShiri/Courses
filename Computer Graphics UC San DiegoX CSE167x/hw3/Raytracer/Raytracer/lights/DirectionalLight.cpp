#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Color &rgb, const Vector3D &direction) :
  Light{rgb}, 
  direction_{direction}
{}

Ray DirectionalLight::getRayTowardsLightFromPoint(const Point3D &point) const
{
  return{slighltyMovePointTowardsDirection_(point, direction_), direction_};
}

float DirectionalLight::getDistanceToPoint(const Point3D &point) const
{
  return std::numeric_limits<float>::max();
}

float DirectionalLight::getAttenuationFactor_(float distanceToLight) const
{
  return 1.0f;
}