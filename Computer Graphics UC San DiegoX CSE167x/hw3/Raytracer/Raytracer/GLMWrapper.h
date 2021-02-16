/*A wrapper class to encapsulate necessary functionalities from GLM library*/

#pragma once

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
};
} // End of namespace GLMWrapper
