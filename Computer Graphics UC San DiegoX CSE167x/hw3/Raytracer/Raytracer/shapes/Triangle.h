/*Represents triangle ABC.*/

#pragma once

#include "Shape.h"

class Triangle : public Shape
{
public:
  Triangle(const MateriaPropertiesAndAmbient &matProperties,
           const TransformationMatrix &transformationMatrix,
           const std::array<Point3D, 3> &vertices);

  const Point3D &A() const;
  const Point3D &B() const;
  const Point3D &C() const;
  bool isPointInTriangle(const Point3D &point) const;
  const Vector3D &getUnitNormal() const;

private:
  void initializeVertices_(
    const std::array<Point3D, 3> &vertices);

  std::array<Point3D, 3> vertices_;
  Vector3D unitNormal_;
};

