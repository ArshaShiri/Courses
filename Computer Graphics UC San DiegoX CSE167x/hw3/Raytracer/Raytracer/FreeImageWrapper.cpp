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
    const auto convertedColor = (BYTE)floor(color >= 1.0f ? 255 : color * 255);
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

std::vector<BYTE> getTestColors(int width, int height)
{
  auto convertedColors = std::vector<BYTE>{};

  for (auto w = 0; w < width; ++w)
  {
    for (auto h = 0; h < height; ++h)
    {
      if (h == 0)
      {
        convertedColors.push_back(255);
        convertedColors.push_back(0);
        convertedColors.push_back(0);
      }
      else
      {
        convertedColors.push_back(0);
        convertedColors.push_back(0);
        convertedColors.push_back(0);
      }
    }
  }

  return convertedColors;
}

void FreeImageWrapper::saveImage(int width, int height, const std::vector<Color> &colors)
{
  // auto convertedColors = getConvertedColors(width, height, colors);

  auto convertedColors = getTestColors(width, height);
  const auto pImage = FreeImage_ConvertFromRawBits(
    convertedColors.data(), width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  FreeImage_Save(FIF_PNG, pImage, "dummy.png", 0);
}
}
