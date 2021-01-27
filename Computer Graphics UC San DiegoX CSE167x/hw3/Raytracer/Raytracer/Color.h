/* Used for colored types of values. Note that the value is not necessarily between 0 to 255 and 
it depends on the convention.*/

#pragma once

#include <array>

class Color
{
public:
  Color() = default;
  Color(float r, float g, float b);
  Color(const std::array<float, 3> &rgb);

private:
  std::array<float, 3> rgb_;
};

