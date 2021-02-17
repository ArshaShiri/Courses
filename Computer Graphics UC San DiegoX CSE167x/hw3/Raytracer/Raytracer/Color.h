/* Used for colored types of values. Note that the value is not necessarily between 0 to 255 and 
it depends on the convention.*/

#pragma once

#include <array>

class Color
{
public:
  Color();
  Color(float r, float g, float b);
  Color(const std::array<float, 3> &rgb);

  float getRed() const { return rgb_.at(0); }
  float getGreen() const { return rgb_.at(1); }
  float getBlue() const { return rgb_.at(2); }

  friend const Color operator+(const Color &color1, const Color &color2);

private:
  std::array<float, 3> rgb_;
};

