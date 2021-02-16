#include "TransformationStack.h"

TransformationStack::TransformationStack()
{
  // The stack is always initialized with an identity matrix.
  transformationStack_.push(TransformationMatrix{});
}

void TransformationStack::push(const TransformationMatrix &matrix)
{
  transformationStack_.push(matrix);
}

void TransformationStack::pop()
{
  transformationStack_.pop();
}

const TransformationMatrix &TransformationStack::top() const
{
  return transformationStack_.top();
}

TransformationMatrix &TransformationStack::top()
{
  return transformationStack_.top();
}