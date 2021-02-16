#include "TransformationMatrix.h"

// Initialize with identity
TransformationMatrix::TransformationMatrix() : data_{std::array<float, 4>{1.0f, 0.0f, 0.0f, 0.0f},
                                                     std::array<float, 4>{0.0f, 1.0f, 0.0f, 0.0f},
                                                     std::array<float, 4>{0.0f, 0.0f, 1.0f, 0.0f},
                                                     std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}
{}

TransformationMatrix::TransformationMatrix(const std::array<float, 4> &col1,
                                           const std::array<float, 4> &col2,
                                           const std::array<float, 4> &col3,
                                           const std::array<float, 4> &col4) : 
  data_{col1, col2, col3, col4}
{}

const std::array<float, 4> &TransformationMatrix::operator[](const size_t idx) const
{
  return data_.at(idx);
}