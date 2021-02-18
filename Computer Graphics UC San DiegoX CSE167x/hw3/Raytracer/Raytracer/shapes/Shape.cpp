#include "Shape.h"

Shape::Shape(const MateriaPropertiesAndAmbient &matProperties,
             const TransformationMatrix &transformationMatrix,
             ShapeType type) : matPropertiesAndAmbient_{matProperties},
                               transformationMatrix_{transformationMatrix},
                               type_{type}
{}

const TransformationMatrix &Shape::getTransformationMatrix() const
{
  return transformationMatrix_;
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

const MateriaPropertiesAndAmbient &Shape::getMateriaPropertiesAndAmbient() const
{
  return matPropertiesAndAmbient_;
}