#include "Triangle.h"

#include "../Common.h"
#include "../GLMWrapper.h"

Triangle::Triangle(const MateriaPropertiesAndAmbient &matProperties,
                   const TransformationMatrix &transformationMatrix,
                   const std::array<Point3D, 3> &vertices) :
  Shape{matProperties, transformationMatrix, ShapeType::Triangle}
{
  initializeVertices_(vertices);
}

bool Triangle::isPointInTriangle(const Point3D &point) const
{
  // In order to check if the found intersection of ray and the plane is actually inside the 
  // triangle, we need to calculate the barycentric coordinates.
  // Based on https://gamedev.stackexchange.com/ and 
  // Christer Ericson's Real-Time Collision Detection
  const auto v0 = B() - A();
  const auto v1 = C() - A();
  const auto v2 = point - A();

  const auto d00 = v0.dot(v0);
  const auto d01 = v0.dot(v1);
  const auto d11 = v1.dot(v1);
  const auto d20 = v2.dot(v0);
  const auto d21 = v2.dot(v1);
  const auto denom = d00 * d11 - d01 * d01;

  const auto alpha = (d11 * d20 - d01 * d21) / denom;
  const auto beta = (d00 * d21 - d01 * d20) / denom;
  const auto gamma = 1.0f - alpha - beta;

  const auto isAlphaBetweenZeroAndOne = (alpha >= 0.0) && (alpha <= 1.0);
  const auto isBetaBetweenZeroAndOne = (beta >= 0.0) && (beta <= 1.0);
  const auto isGammaaBetweenZeroAndOne = (gamma >= 0.0) && (gamma <= 1.0);

  return isAlphaBetweenZeroAndOne &&
         isBetaBetweenZeroAndOne  &&
         isGammaaBetweenZeroAndOne;
}

const Point3D &Triangle::A() const
{
  return vertices_.at(0);
}

const Point3D &Triangle::B() const
{
  return vertices_.at(1);
}

const Point3D &Triangle::C() const
{
  return vertices_.at(2);
}

void Triangle::initializeVertices_(
  const std::array<Point3D, 3> &vertices)
{
  const auto &transformationMatrix = getTransformationMatrix();

  vertices_.at(0) = GLMWrapper::GLMWrapper::TransformPoint(transformationMatrix,
                                                           vertices.at(0));
  vertices_.at(1) = GLMWrapper::GLMWrapper::TransformPoint(transformationMatrix,
                                                           vertices.at(1));
  vertices_.at(2) = GLMWrapper::GLMWrapper::TransformPoint(transformationMatrix,
                                                           vertices.at(2));
}