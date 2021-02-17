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
  convertedColors.resize(numberOfColors * totalNumberOfPixels * numberOfColors);

  auto getConvertedColor = [](float color)
  {
    return (BYTE)floor(color >= 1.0f ? 255 : color * 255);
  };

  for (auto pixelIdx = size_t{0}; pixelIdx < totalNumberOfPixels; ++pixelIdx)
  {
    const auto color = colors.at(pixelIdx);
    const auto pixelHeight = static_cast<int>(pixelIdx / width);
    const auto pixelWidth = pixelIdx - pixelHeight * width;
    const auto colorIdx = (pixelHeight * width * 3) + (pixelWidth * 3);

    convertedColors.at(colorIdx) = getConvertedColor(color.getBlue());
    convertedColors.at(colorIdx + 1) = getConvertedColor(color.getGreen());
    convertedColors.at(colorIdx + 2) = getConvertedColor(color.getRed());
  }
  
  return convertedColors;
}

std::vector<BYTE> getTestColors(int width, int height)
{
  auto convertedColors = std::vector<BYTE>{};
  convertedColors.resize(width * height * 3);

  for (auto w = 0; w < width; ++w)
  {
    for (auto h = 0; h < height; ++h)
    {
      const auto colorIdx = (h * width * 3) + (w * 3);

      if (h == 0)
      {
        convertedColors.at(colorIdx) = 255;
        convertedColors.at(colorIdx + 1) = 0;
        convertedColors.at(colorIdx + 2) = 0;
      }
      else
      {
        convertedColors.at(colorIdx) = 0;
        convertedColors.at(colorIdx + 1) = 0;
        convertedColors.at(colorIdx + 2) = 0;
      }
    }
  }

  return convertedColors;
}

void FreeImageWrapper::saveImage(const std::string &fileName, int width, int height, const std::vector<Color> &colors)
{
  auto convertedColors = getConvertedColors(width, height, colors);

  // auto convertedColors = getTestColors(width, height);
  const auto pImage = FreeImage_ConvertFromRawBits(
    convertedColors.data(), width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

  const auto outputFile = fileName + ".png";
  FreeImage_Save(FIF_PNG, pImage, outputFile.c_str(), 0);
}
}
