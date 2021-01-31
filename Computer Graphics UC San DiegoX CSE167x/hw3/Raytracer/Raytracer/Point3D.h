#pragma once

#include <array>

#include "Vector3D.h"

class Point3D
{
public:
  Point3D(float x, float y, float z);
  Point3D(const std::array<float, 3> &coords);

  float x() const { return coords_.at(0); }
  float y() const { return coords_.at(1); }
  float z() const { return coords_.at(2); }

  const Vector3D friend operator+(const Point3D &p1, const Point3D &p2);
  const Vector3D friend operator-(const Point3D &p1, const Point3D &p2);

  const Point3D friend operator+(const Point3D &p1, const Vector3D &p2);

private:
  std::array<float, 3> coords_;
};

