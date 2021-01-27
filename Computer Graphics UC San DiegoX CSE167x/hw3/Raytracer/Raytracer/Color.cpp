#include "Color.h"

Color::Color(const std::array<float, 3> &rgb) : rgb_{rgb}
{}

Color::Color(float r, float g, float b) : rgb_{r, g, b}
{}