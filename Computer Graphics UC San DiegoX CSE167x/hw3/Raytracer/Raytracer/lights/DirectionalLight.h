#pragma once

#include "../Vector3D.h"
#include "Light.h"

class DirectionalLight : public Light
{
public:
  DirectionalLight(const Color &rgb, const Vector3D &direction);

  Ray getRayTowardsLightFromPoint(const Point3D &point) const override final;
  float getDistanceToPoint(const Point3D &point) const override final;

protected:
  float getAttenuationFactor_(float distanceToLight) const override final;

private:
  const Vector3D direction_;
};