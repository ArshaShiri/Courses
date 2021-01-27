#pragma once


#include "Shape.h"

class Triangle : public Shape
{
public:
  Triangle(const MateriaPropertiesAndAmbient &matProperties,
           const std::vector<Point3D> &vertices,
           const std::array<int, 3> &cornerNodeIndices);

  std::optional<Point3D> getIntersection(const Ray &ray) override final;

private:
  const std::array<int, 3> cornerNodeIndices_;
};

