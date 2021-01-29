/* Common helper functions and other useful helper utilities are added here.*/

#pragma once

#include <array>

#include "Color.h"

struct MateriaPropertiesAndAmbient
{
  // rgb 
  Color diffuse;
  Color specular;
  Color emission;
  Color ambient;

  float shininess;
};

const auto GEOMETRY_TOLERANCE = 1e-7;