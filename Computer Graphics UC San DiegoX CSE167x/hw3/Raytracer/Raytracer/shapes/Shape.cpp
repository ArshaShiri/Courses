#include "Shape.h"

Shape::Shape(const MateriaPropertiesAndAmbient &matProperties,
             const std::vector<Point3D> &Vertices) : matProperties_{matProperties},
                                                     Vertices_{Vertices}
{}
