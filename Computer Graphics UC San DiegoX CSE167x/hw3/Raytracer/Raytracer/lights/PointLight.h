#pragma once

#include "Light.h"

#include "../Common.h"
#include "../Point3D.h"

class PointLight : public Light
{
public:
  PointLight(const Color &rgb, const Point3D &point, const Attenuation &attenuation);

  Ray getRayTowardsLightFromPoint(const Point3D &point) const override final;
  float getDistanceToPoint(const Point3D &point) const override final;

protected:
  float getAttenuationFactor_(float distanceToLight) const override final;

private:
  Point3D point_;
  Attenuation attenuation_;
};