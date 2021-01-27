#include "LightFactory.h"

std::unique_ptr<DirectionalLight> LightFactory::createDirectionalLight(
  const Vector3D &direction,
  const Color &rgb)
{
  return std::make_unique<DirectionalLight>(direction, rgb);
}

std::unique_ptr<PointLight> LightFactory::createPointLight(
  const Point3D &point,
  const Color &rgb)
{
  return std::make_unique<PointLight>(point, rgb);
}