/*A very simple vector class in 3D.*/

#pragma once

#include <array>

class Vector3D
{
public:
  Vector3D() = default;
  Vector3D(const std::array<float, 3> &coords);
  Vector3D(float x, float y, float z);

  float x() const { return coords_.at(0); }
  float y() const { return coords_.at(1); }
  float z() const { return coords_.at(2); }

  float dot(const Vector3D &vec) const;
  Vector3D cross(const Vector3D &otherVec) const;
  float length() const;
  Vector3D normalize() const;

  Vector3D operator-() const;

  const Vector3D friend operator+(const Vector3D &v1, const Vector3D &v2);
  const Vector3D friend operator-(const Vector3D &v1, const Vector3D &v2);
  const Vector3D friend operator*(const Vector3D &v, const float &s);

private:
  std::array<float, 3> coords_;
};