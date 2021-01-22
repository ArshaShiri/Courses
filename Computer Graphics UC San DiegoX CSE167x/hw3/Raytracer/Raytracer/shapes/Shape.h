/*Base class for supported shapes.*/

#pragma once

#include "../Common.h"

class Shape
{
public:
  Shape(const MateriaProperties &matProperties);

private:
  MateriaProperties matProperties_;
};