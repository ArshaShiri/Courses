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

struct Point3D
{
  Point3D(const std::array<float, 3> &point) :  x{point.at(0)},
                                                y{point.at(1)},
                                                z{point.at(2)}
  {}

  float x;
  float y;
  float z;
};