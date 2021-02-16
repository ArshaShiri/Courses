#pragma once

#include "Shape.h"

class Sphere : public Shape
{
public:
  Sphere(const MateriaPropertiesAndAmbient &matProperties,
         const TransformationMatrix &transformationMatrix,
         const Point3D &center,
         const float radius);

  const Point3D &getCenter() const;
  float getRadius() const;

private:
  const Point3D center_;
  const float radius_;
};

