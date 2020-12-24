// Transform.cpp: implementation of the Transform class.

#include "Transform.h"
#include <cstdio>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  const auto rad = glm::radians(degrees);
  const auto cosAngle = cos(rad);
  const auto sinAngle = sin(rad);

  const auto x = axis[0];
  const auto y = axis[1];
  const auto z = axis[2];

  const auto identityMatrix = mat3(1.0);

  const auto firstComponent = cosAngle * identityMatrix;
  const auto secondComponent = (1 - cosAngle) * mat3(x * x, x * y, x * z,
                                                     x * y, y * y, y * z,
                                                     x * z, y * z, z * z);
  const auto thirdComponent = sinAngle * mat3(0.f, -z, y,
                                              z, 0.f, -x,
                                              -y, x, 0.f);

  return firstComponent + secondComponent + thirdComponent;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  const auto rotationMatrix = rotate(degrees, -up);
  eye = rotationMatrix * eye;
  up = rotationMatrix * up;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  const auto rotationAxis = normalize(cross(up, eye));
  const auto rotationMatrix = rotate(degrees, rotationAxis);
  eye = rotationMatrix * eye;
  up = rotationMatrix * up;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  const auto w = normalize(eye);
  const auto u = normalize(cross(up, w));
  const auto v = cross(w, u);

  return mat4(u[0], v[0], w[0], 0.f,
              u[1], v[1], w[1], 0.f,
              u[2], v[2], w[2], 0.f,
              -dot(u, eye), -dot(v, eye), -dot(w, eye), 1.f);
}

Transform::Transform()
{

}

Transform::~Transform()
{

}