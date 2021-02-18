#include "LightFactory.h"

std::unique_ptr<DirectionalLight> LightFactory::createDirectionalLight(
  const Vector3D &direction,
  const Color &rgb)
{
  return std::make_unique<DirectionalLight>(rgb, direction);
}

std::unique_ptr<PointLight> LightFactory::createPointLight(
  const Point3D &point,
  const Color &rgb,
  const Attenuation &attenuation)
{
  return std::make_unique<PointLight>(rgb, point, attenuation);
}