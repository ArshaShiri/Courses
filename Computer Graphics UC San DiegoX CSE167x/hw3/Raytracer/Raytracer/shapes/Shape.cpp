#include "Shape.h"

Shape::Shape(const MateriaPropertiesAndAmbient &matProperties,
             const std::vector<Point3D> &Vertices,
             ShapeType type) : matPropertiesAndAmbient_{matProperties},
                               Vertices_{Vertices},
                               type_{type}
{}

const std::vector<Point3D> &Shape::getVertices_() const
{
  return Vertices_;
}

ShapeType Shape::getType() const
{
  return type_;
}

const Color &Shape::getDiffuse() const
{
  return matPropertiesAndAmbient_.diffuse;
}

const Color &Shape::getSpecular() const
{
  return matPropertiesAndAmbient_.specular;
}

const Color &Shape::getEmission() const
{
  return matPropertiesAndAmbient_.emission;
}

const Color &Shape::getAmbient() const
{
  return matPropertiesAndAmbient_.ambient;
}

float  Shape::getShininess() const
{
  return matPropertiesAndAmbient_.shininess;
}