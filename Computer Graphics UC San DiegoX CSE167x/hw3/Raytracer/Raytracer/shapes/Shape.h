/*Base class for supported primitives.*/

#pragma once

#include <array>
#include <optional>
#include <vector>

#include "../Common.h"
#include "../Ray.h"

class Shape
{
public:
  Shape(const MateriaPropertiesAndAmbient &matProperties, 
        const std::vector<Point3D> &Vertices);
  
  virtual std::optional<Point3D> getIntersection(const Ray &ray) = 0;

protected:
  const std::vector<Point3D> &getVertices() const;
  
private:
  const MateriaPropertiesAndAmbient matProperties_;
  const std::vector<Point3D> &Vertices_;
};