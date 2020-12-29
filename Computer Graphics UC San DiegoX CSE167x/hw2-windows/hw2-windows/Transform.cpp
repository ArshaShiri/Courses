// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  const auto rad = glm::radians(degrees);
  const auto cosAngle = cos(rad);
  const auto sinAngle = sin(rad);
  const auto normalizedAxis = normalize(axis);

  const auto x = normalizedAxis[0];
  const auto y = normalizedAxis[1];
  const auto z = normalizedAxis[2];

  const auto identityMatrix = mat3(1.f);

  const auto firstComponent = cosAngle * identityMatrix;
  const auto secondComponent = (1 - cosAngle) * mat3(x * x, x * y, x * z,
                                                     x * y, y * y, y * z,
                                                     x * z, y * z, z * z);
  const auto thirdComponent = sinAngle * mat3(0.f, z, -y,
                                              -z, 0.f, x,
                                              y, -x, 0.f);

  return firstComponent + secondComponent + thirdComponent;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  const auto rotationMatrix = rotate(degrees, up);
  eye = rotationMatrix * eye;
  up = normalize(rotationMatrix * up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  const auto rotationAxis = normalize(cross(eye, up));
  const auto rotationMatrix = rotate(degrees, rotationAxis);
  eye = rotationMatrix * eye;
  up = normalize(rotationMatrix * up);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  const auto w = normalize(eye - center);
  const auto u = normalize(cross(up, w));
  const auto v = cross(w, u);

  return mat4(u[0], v[0], w[0], 0.f,
              u[1], v[1], w[1], 0.f,
              u[2], v[2], w[2], 0.f,
              -dot(u, eye), -dot(v, eye), -dot(w, eye), 1.f);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  const auto theta = fovy / 2.f;
  const auto d = 1.f / tan(glm::radians(theta));
  const auto A = -(zFar + zNear) / (zFar - zNear);
  const auto B = (-2.f * zFar * zNear) / (zFar - zNear);

  return mat4(d / aspect, 0.f, 0.f, 0.f,
              0.f, d, 0.f, 0.f,
              0.f, 0.f, A, -1.f,
              0.f, 0.f, B, 0.f);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  auto ret = mat4(1.0);
  ret[0][0] = sx;
  ret[1][1] = sy;
  ret[2][2] = sz;

  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  auto ret = mat4(1.0);
  ret[3][0] = tx;
  ret[3][1] = ty;
  ret[3][2] = tz;

  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
