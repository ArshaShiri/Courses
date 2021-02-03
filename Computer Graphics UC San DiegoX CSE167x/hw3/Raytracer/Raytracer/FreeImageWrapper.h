#pragma once

#include <vector>

#include "Color.h"

namespace FreeImage
{
class FreeImageWrapper
{
public:
  static void saveImage(int width, int height, const std::vector<Color> &colors);
};
}