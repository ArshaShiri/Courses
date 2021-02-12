#include "Color.h"

Color::Color() : rgb_{0.f, 0.f, 0.f}
{}

Color::Color(const std::array<float, 3> &rgb) : rgb_{rgb}
{}

Color::Color(float r, float g, float b) : rgb_{r, g, b}
{}