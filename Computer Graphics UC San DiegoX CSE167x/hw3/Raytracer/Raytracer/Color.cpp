#include "Color.h"

Color::Color() : rgb_{0.f, 0.f, 0.f}
{}

Color::Color(const std::array<float, 3> &rgb) : rgb_{rgb}
{}

Color::Color(float r, float g, float b) : rgb_{r, g, b}
{}

const Color operator+(const Color &color1, const Color &color2)
{
  const auto red = color1.getRed() + color2.getRed();
  const auto green = color1.getGreen() + color2.getGreen();
  const auto blue = color1.getBlue() + color2.getBlue();

  return {red, green, blue};
}