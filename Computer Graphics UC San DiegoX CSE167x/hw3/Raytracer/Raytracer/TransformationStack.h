/*Represents the transformation stack which is used to transfer the objects.*/

#pragma once

#include <stack>

#include "TransformationMatrix.h"

class TransformationStack
{
public:
  TransformationStack();

  void push(const TransformationMatrix &matrix);
  void pop();

  const TransformationMatrix &top() const;
  TransformationMatrix &top();

private:
  std::stack<TransformationMatrix> transformationStack_;
};