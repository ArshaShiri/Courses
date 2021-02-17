#pragma once

#include <string>
#include <vector>

#include "Color.h"

namespace FreeImage
{
class FreeImageWrapper
{
public:
  static void saveImage(const std::string &fileName, int width, int height, const std::vector<Color> &colors);
};
}