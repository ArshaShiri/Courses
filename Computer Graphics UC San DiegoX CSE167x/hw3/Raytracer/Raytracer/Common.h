/* Common helper functions and other useful helper utilities are added here.*/

#pragma once

#include <array>

struct MateriaProperties
{
  // rgb 
  std::array<float, 3> diffuse;
  std::array<float, 3> specular;
  std::array<float, 3> emission;
  std::array<float, 3> ambient;

  float shininess;
};

struct Point
{
  float x;
  float y;
  float z;
};