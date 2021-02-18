#pragma once

#include <memory>

#include "../Common.h"
#include "DirectionalLight.h"
#include "PointLight.h"

class LightFactory
{
public:
  static std::unique_ptr<DirectionalLight> createDirectionalLight(const Vector3D &direction,
                                                                  const Color &rgb);

  static std::unique_ptr<PointLight> createPointLight(const Point3D &point,
                                                      const Color &rgb,
                                                      const Attenuation &attenuation);
};

