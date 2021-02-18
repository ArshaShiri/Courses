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

TransformationMatrix TransformationMatrix::getTransposed() const
{
  return TransformationMatrix{{data_[0][0], data_[1][0], data_[2][0], data_[3][0]},
                              {data_[0][1], data_[1][1], data_[2][1], data_[3][1]},
                              {data_[0][2], data_[1][2], data_[2][2], data_[3][2]},
                              {data_[0][3], data_[1][3], data_[2][3], data_[3][3]}};
}