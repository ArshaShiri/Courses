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
  Shape(const MateriaPropertiesAndAmbient &matProperties, 
        ShapeType type = ShapeType::Unknown);

  ShapeType getType() const;

  const Color &getDiffuse() const;
  const Color &getSpecular() const;
  const Color &getEmission() const;
  const Color &getAmbient() const;

  float  getShininess() const;
  
  virtual ~Shape() = default;
  
private:
  const MateriaPropertiesAndAmbient matPropertiesAndAmbient_;  
  ShapeType type_;
};