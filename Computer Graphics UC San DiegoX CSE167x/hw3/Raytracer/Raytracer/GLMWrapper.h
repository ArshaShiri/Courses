/*A wrapper class to encapsulate necessary functionalities from GLM library*/

#pragma once

#include "Point3D.h"
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

  static Point3D TransformPoint(const TransformationMatrix &matrix, const Point3D &point);
};
} // End of namespace GLMWrapper
