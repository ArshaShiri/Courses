#include <memory>

#include "FreeImage.h"
#include "FreeImageWrapper.h"

namespace FreeImage
{

std::vector<BYTE> getConvertedColors(int width, int height, const std::vector<Color> &colors)
{
  const auto totalNumberOfPixels = width * height;
  const auto numberOfColors = 3;

  auto convertedColors = std::vector<BYTE>{};
  convertedColors.reserve(numberOfColors * totalNumberOfPixels);

  auto convertColor = [&convertedColors](float color) 
  {
    const auto convertedColor = floor(color >= 1.0f ? 255 : color * 255);
    convertedColors.emplace_back(convertedColor);
  };

  for (const auto &color : colors)
  {
    convertColor(color.getBlue());
    convertColor(color.getGreen());
    convertColor(color.getRed());
  }
  
  return convertedColors;
}

void FreeImageWrapper::saveImage(int width, int height, const std::vector<Color> &colors)
{
  auto convertedColors = getConvertedColors(width, height, colors);
  const auto pImage = FreeImage_ConvertFromRawBits(
    convertedColors.data(), width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  FreeImage_Save(FIF_PNG, pImage, "dummy.png", 0);
}
}
