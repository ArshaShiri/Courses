/*Represents triangle ABC.*/

#pragma once

#include "Shape.h"

class Triangle : public Shape
{
public:
  Triangle(const MateriaPropertiesAndAmbient &matProperties,
           const std::vector<Point3D> &vertices,
           const std::array<int, 3> &cornerNodeIndices);

  const Point3D &A() const;
  const Point3D &B() const;
  const Point3D &C() const;
  bool isPointInTriangle(const Point3D &point) const;
  const Vector3D &getUnitNormal() const;

private:
  const std::array<int, 3> cornerNodeIndices_;
  Vector3D unitNormal_;
};

