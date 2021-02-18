/*Base class for supported primitives.*/

#pragma once

#include <array>
#include <vector>

#include "../Common.h"
#include "../Ray.h"
#include "../TransformationMatrix.h"

enum class ShapeType
{
  Triangle,
  Sphere,
  Unknown
};

class Shape
{
public:
  // We need the transformationMatrix for defining the shape. For a simple shape like 
  // triangle, the triangle can be directly initialized by its vertices transformed.
  // For other shapes that are defined by their equation (e.g. sphere), the transformationMatrix
  // cannot be used on the shape because it will change its equation.
  // Instead, we use this transformation to transform the ray and intersect and transform back the 
  // intersection point.
  Shape(const MateriaPropertiesAndAmbient &matProperties,
        const TransformationMatrix &transformationMatrix,
        ShapeType type = ShapeType::Unknown);

  const TransformationMatrix &getTransformationMatrix() const;

  ShapeType getType() const;

  const Color &getDiffuse() const;
  const Color &getSpecular() const;
  const Color &getEmission() const;
  const Color &getAmbient() const;
  const MateriaPropertiesAndAmbient &getMateriaPropertiesAndAmbient() const;

  float  getShininess() const;
  
  virtual ~Shape() = default;
  
private:
  const MateriaPropertiesAndAmbient matPropertiesAndAmbient_;
  const TransformationMatrix transformationMatrix_;
  ShapeType type_;
};