/*Represents triangle ABC is a primitive that can support intersection with a ray.*/

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
  // Private methods
  const Point3D &A_() { getVertices().at(cornerNodeIndices_.at(0)); }
  const Point3D &B_() { getVertices().at(cornerNodeIndices_.at(0)); }
  const Point3D &C_() { getVertices().at(cornerNodeIndices_.at(0)); }

  // Private attributes
  const std::array<int, 3> cornerNodeIndices_;
  Vector3D unitNormal_;
};

