/*Base class for supported primitives.*/

#pragma once

#include <optional>

#include "../Common.h"
#include "../Ray.h"

class Shape
{
public:
  Shape(const MateriaPropertiesAndAmbient &matProperties);
  
  virtual std::optional<Point3D> getIntersection(const Ray &ray) = 0;
  
private:
  MateriaPropertiesAndAmbient matProperties_;
};