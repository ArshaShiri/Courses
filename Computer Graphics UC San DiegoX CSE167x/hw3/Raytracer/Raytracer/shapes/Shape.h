/*Base class for supported primitives.*/

#pragma once

#include <array>
#include <vector>

#include "../Common.h"
#include "../Ray.h"

enum class ShapeType
{
  Triangle,
  Unknown
};

class Shape
{
public:
  Shape(const MateriaPropertiesAndAmbient &matProperties, 
        const std::vector<Point3D> &Vertices,
        ShapeType type = ShapeType::Unknown);

  ShapeType getType() const;

  const Color &getDiffuse() const;
  const Color &getSpecular() const;
  const Color &getEmission() const;
  const Color &getAmbient() const;

  float  getShininess() const;
  
  virtual ~Shape() = default;

protected:
  const std::vector<Point3D> &getVertices_() const;
  
private:
  const MateriaPropertiesAndAmbient matPropertiesAndAmbient_;
  const std::vector<Point3D> &Vertices_;
  ShapeType type_;
};