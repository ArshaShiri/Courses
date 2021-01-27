#pragma once

#include <array>

#include "../Color.h"

class Light
{
public:
  Light(const Color &rgb);

private:
  Color color_;
};