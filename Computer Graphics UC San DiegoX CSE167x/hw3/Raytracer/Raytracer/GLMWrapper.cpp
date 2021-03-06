#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLMWrapper.h"

namespace
{
TransformationMatrix convertToTransformationMatrix(const glm::mat4 &glmMatrix);
glm::mat4 convertToMat4(const TransformationMatrix &transformationMatrix);

TransformationMatrix rightMultiply(const glm::mat4 &matrix, const glm::mat4 &multiplier);
} // End of namespace declaration

namespace GLMWrapper
{
TransformationMatrix GLM::getScaled(
  const TransformationMatrix &matrix, float sx, float sy, float sz)
{
  auto scaledMat = glm::mat4(1.0);
  scaledMat[0][0] = sx;
  scaledMat[1][1] = sy;
  scaledMat[2][2] = sz;

  return rightMultiply(convertToMat4(matrix), scaledMat);
}

TransformationMatrix GLM::getTranslated(
  const TransformationMatrix &matrix, float tx, float ty, float tz)
{
  auto translatedMat = glm::mat4(1.0);
  translatedMat[3][0] = tx;
  translatedMat[3][1] = ty;
  translatedMat[3][2] = tz;

  return rightMultiply(convertToMat4(matrix), translatedMat);
}

TransformationMatrix GLM::getRotated(
  const TransformationMatrix &matrix, const Vector3D &axis, float degrees)
{
  const auto rad = glm::radians(degrees);
  const auto cosAngle = cos(rad);
  const auto sinAngle = sin(rad);
  const auto normalizedAxis = axis.normalize();

  const auto x = normalizedAxis.x();
  const auto y = normalizedAxis.y();
  const auto z = normalizedAxis.z();

  const auto identityMatrix = glm::mat3(1.f);

  const auto firstComponent = cosAngle * identityMatrix;
  const auto secondComponent = (1 - cosAngle) * glm::mat3(x * x, x * y, x * z,
                                                          x * y, y * y, y * z,
                                                          x * z, y * z, z * z);
  const auto thirdComponent = sinAngle * glm::mat3(0.f, z, -y,
                                                   -z, 0.f, x,
                                                   y, -x, 0.f);

  const auto rotationMatrix = glm::mat4{firstComponent + secondComponent + thirdComponent};

  return rightMultiply(convertToMat4(matrix), rotationMatrix);
}

Point3D GLM::TransformPoint(const TransformationMatrix &matrix, const Point3D &point)
{
  const auto convertedMat = convertToMat4(matrix);
  const auto convertedVec4 = glm::vec4{point.x(), point.y(), point.z(), 1.0f};
  const auto transformedPoint = convertedMat * convertedVec4;

  const auto newX = transformedPoint.x / transformedPoint.w;
  const auto newY = transformedPoint.y / transformedPoint.w;
  const auto newZ = transformedPoint.z / transformedPoint.w;

  return {newX, newY, newZ};
}

Vector3D GLM::TransformNormalVector(const TransformationMatrix &matrix, const Vector3D &vector)
{
  // The translation parts of the transformation matrix do not affect the vector.
  const auto convertedMat = convertToMat4(matrix);
  const auto convertedVec4 = glm::vec4{vector.x(), vector.y(), vector.z(), 1.0f};
  const auto transformedPoint = convertedMat * convertedVec4;

  return {transformedPoint.x, transformedPoint.y, transformedPoint.z};
}

Vector3D GLM::TransformVector(const TransformationMatrix &matrix, const Vector3D &vector)
{
  // The translation parts of the transformation matrix do not affect the vector.
  const auto convertedMat = convertToMat4(matrix);
  const auto convertedVec4 = glm::vec4{vector.x(), vector.y(), vector.z(), 0.0f};
  const auto transformedPoint = convertedMat * convertedVec4;

  return {transformedPoint.x, transformedPoint.y, transformedPoint.z};
}

TransformationMatrix GLM::getInversed(const TransformationMatrix &matrix)
{
  const auto convertedMat = convertToMat4(matrix);
  return convertToTransformationMatrix(glm::inverse(convertedMat));
}

Ray GLM::getTransformedRay(const Ray &ray, const TransformationMatrix &matrix)
{
  const auto transformedViewPoint = TransformPoint(matrix, ray.getViewPoint());
  const auto transformedDirection = TransformVector(matrix, ray.getUnitDirection());

  return {transformedViewPoint, transformedDirection};
}
} // End of namespace GLMWrapper

namespace
{
TransformationMatrix convertToTransformationMatrix(const glm::mat4 &glmMatrix)
{
  using colArray = std::array<float, 4>;
  const auto col1 = colArray{glmMatrix[0][0], glmMatrix[0][1], glmMatrix[0][2], glmMatrix[0][3]};
  const auto col2 = colArray{glmMatrix[1][0], glmMatrix[1][1], glmMatrix[1][2], glmMatrix[1][3]};
  const auto col3 = colArray{glmMatrix[2][0], glmMatrix[2][1], glmMatrix[2][2], glmMatrix[2][3]};
  const auto col4 = colArray{glmMatrix[3][0], glmMatrix[3][1], glmMatrix[3][2], glmMatrix[3][3]};

  return {col1, col2, col3, col4};
}

glm::mat4 convertToMat4(const TransformationMatrix &transformationMatrix)
{
  auto converted = glm::mat4{};

  converted[0][0] = transformationMatrix[0][0];
  converted[0][1] = transformationMatrix[0][1];
  converted[0][2] = transformationMatrix[0][2];
  converted[0][3] = transformationMatrix[0][3];

  converted[1][0] = transformationMatrix[1][0];
  converted[1][1] = transformationMatrix[1][1];
  converted[1][2] = transformationMatrix[1][2];
  converted[1][3] = transformationMatrix[1][3];

  converted[2][0] = transformationMatrix[2][0];
  converted[2][1] = transformationMatrix[2][1];
  converted[2][2] = transformationMatrix[2][2];
  converted[2][3] = transformationMatrix[2][3];

  converted[3][0] = transformationMatrix[3][0];
  converted[3][1] = transformationMatrix[3][1];
  converted[3][2] = transformationMatrix[3][2];
  converted[3][3] = transformationMatrix[3][3];

  return converted;
}

TransformationMatrix rightMultiply(const glm::mat4 &matrix, const glm::mat4 &multiplier)
{
  return convertToTransformationMatrix(matrix * multiplier);
}
} // End of namespace definition