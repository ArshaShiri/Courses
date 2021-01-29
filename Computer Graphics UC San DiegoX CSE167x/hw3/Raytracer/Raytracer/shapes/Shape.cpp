#include "Shape.h"

Shape::Shape(const MateriaPropertiesAndAmbient &matProperties,
             const std::vector<Point3D> &Vertices) : matProperties_{matProperties},
                                                     Vertices_{Vertices}
{}

const std::vector<Point3D> &Shape::getVertices() const
{
  return Vertices_;
}