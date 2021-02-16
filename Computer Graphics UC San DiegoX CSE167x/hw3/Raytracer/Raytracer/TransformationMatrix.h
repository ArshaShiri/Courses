/*Represents a 4x4 transformation matrix in column major order.*/

#include <array>

#pragma once
class TransformationMatrix
{
public:
  TransformationMatrix();
  TransformationMatrix(const std::array<float, 4> &col1, 
                       const std::array<float, 4> &col2,
                       const std::array<float, 4> &col3,
                       const std::array<float, 4> &col4);

  const std::array<float, 4> &operator[](const size_t idx) const;

private:
  std::array<std::array<float, 4>, 4> data_;
};

