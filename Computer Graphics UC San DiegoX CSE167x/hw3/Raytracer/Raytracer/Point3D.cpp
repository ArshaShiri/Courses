#include "Point3D.h"

Point3D::Point3D(float x, float y, float z) : coords_{x, y, z}
{}

Point3D::Point3D(const std::array<float, 3> &coords) : coords_{coords}
{}

float Point3D::distance(const Point3D &other) const
{
  const auto deltaX = x() - other.x();
  const auto deltaY = y() - other.y();
  const auto deltaZ = z() - other.z();

  return sqrt(deltaX * deltaX + 
              deltaY * deltaY +
              deltaZ * deltaZ);
}

const Vector3D operator+(const Point3D &p1, const Point3D &p2)
{
  return {p1.x() + p2.x(),
          p1.y() + p2.y(),
          p1.z() + p2.z()};
}

const Vector3D operator-(const Point3D &p1, const Point3D &p2)
{
  return {p1.x() - p2.x(),
          p1.y() - p2.y(),
          p1.z() - p2.z()};
}

const Point3D operator+(const Point3D &p, const Vector3D &v)
{
  return {p.x() + v.x(),
          p.y() + v.y(),
          p.z() + v.z()};
}