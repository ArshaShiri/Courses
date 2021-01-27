#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Vector3D &direction, const Color &rgb) :
  Light{rgb}, 
  direction_{direction}
{}