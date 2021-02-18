/*A wrapper class to encapsulate necessary functionalities from GLM library*/

#pragma once

#include "Point3D.h"
#include "Ray.h"
#include "TransformationMatrix.h"
#include "Vector3D.h"

namespace GLMWrapper
{
class GLMWrapper
{
public:
  static TransformationMatrix getScaled(
    const TransformationMatrix &matrix, float sx, float sy, float sz);

  static TransformationMatrix getTranslated(
    const TransformationMatrix &matrix, float tx, float ty, float tz);

  static TransformationMatrix getRotated(
    const TransformationMatrix &matrix, const Vector3D &axis, float degrees);

  static TransformationMatrix getInversed(const TransformationMatrix &matrix);

  static Point3D TransformPoint(const TransformationMatrix &matrix, const Point3D &point);
  static Vector3D TransformVector(const TransformationMatrix &matrix, const Vector3D &vector);
  static Vector3D TransformNormalVector(const TransformationMatrix &matrix, const Vector3D &vector);
  static Ray getTransformedRay(const Ray &ray, const TransformationMatrix &matrix);
};
} // End of namespace GLMWrapper
