#include "Color.h"

Color::Color() : rgb_{0.f, 0.f, 0.f}
{}

Color::Color(const std::array<float, 3> &rgb) : rgb_{rgb}
{}

Color::Color(float r, float g, float b) : rgb_{r, g, b}
{}

Color &Color::operator+=(const Color &other)
{
  const auto sum = *this + other;
  rgb_ = sum.rgb_;
  return *this;
}

Color &Color::operator/=(float factor)
{
  rgb_.at(0) /= factor;
  rgb_.at(1) /= factor;
  rgb_.at(2) /= factor;

  return *this;
}

const Color operator+(const Color &color1, const Color &color2)
{
  const auto red = color1.getRed() + color2.getRed();
  const auto green = color1.getGreen() + color2.getGreen();
  const auto blue = color1.getBlue() + color2.getBlue();

  return {red, green, blue};
}

const Color operator*(const Color &color1, const Color &color2)
{
  const auto red = color1.getRed() * color2.getRed();
  const auto green = color1.getGreen() * color2.getGreen();
  const auto blue = color1.getBlue() * color2.getBlue();

  return {red, green, blue};
}

const Color operator*(const Color &color, float factor)
{
  const auto red = color.getRed() * factor;
  const auto green = color.getGreen() * factor;
  const auto blue = color.getBlue() * factor;

  return {red, green, blue};
}

const Color operator*(float factor, const Color &color)
{
  return color * factor;
}