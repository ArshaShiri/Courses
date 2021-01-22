#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

#include "Vector3D.h"

Vector3D::Vector3D(const std::array<float, 3> &coords) : coords_{coords}
{}

Vector3D::Vector3D(float x, float y, float z) : coords_{x, y, z}
{}

float Vector3D::dot(const Vector3D &otherVec) const
{
  return x() * otherVec.x() +
         y() * otherVec.y() +
         z() * otherVec.z();
}

Vector3D Vector3D::cross(const Vector3D &otherVec) const
{
  return {y() * otherVec.z() - z() * otherVec.y(),
          z() * otherVec.x() - x() * otherVec.z(),
          x() * otherVec.y() - y() * otherVec.x()};
}

float Vector3D::length() const
{
  return sqrt(x() * x() + y() * y() + z() * z());
}

Vector3D Vector3D::normalize() const
{
  const auto currentLegnth = length();

  return {x() / currentLegnth,
          y() / currentLegnth,
          z() / currentLegnth};
}

Vector3D& Vector3D::operator=(const Vector3D &otherVec)
{
  coords_.at(0) = otherVec.x();
  coords_.at(1) = otherVec.y();
  coords_.at(2) = otherVec.z();

  return *this;
}

const Vector3D operator+(const Vector3D &v1, const Vector3D &v2)
{
  return {v1.x() + v2.x(),
          v1.y() + v2.y(),
          v1.z() + v2.z()};
}

const Vector3D operator-(const Vector3D &v1, const Vector3D &v2)
{
  return {v1.x() - v2.x(),
          v1.y() - v2.y(),
          v1.z() - v2.z()};
}