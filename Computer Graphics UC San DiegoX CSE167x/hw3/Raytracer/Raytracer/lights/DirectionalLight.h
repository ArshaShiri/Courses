#pragma once

#include "../Vector3D.h"
#include "Light.h"

class DirectionalLight : public Light
{
public:
  DirectionalLight(const Vector3D &direction, const Color &rgb);

private:
  const Vector3D direction_;
};