#include "Sphere.h"

Sphere::Sphere(const MateriaPropertiesAndAmbient &matProperties,
               const Point3D &center,
               const float radius) : 
  Shape{matProperties, ShapeType::Sphere},
  center_{center},
  radius_{radius}
{}

const Point3D &Sphere::getCenter() const
{
  return center_;
}

float Sphere::getRadius() const
{
  return radius_;
}