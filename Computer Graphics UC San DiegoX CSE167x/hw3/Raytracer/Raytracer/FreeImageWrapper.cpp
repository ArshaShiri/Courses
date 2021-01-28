#include <memory>

#include "FreeImage.h"
#include "FreeImageWrapper.h"

namespace FreeImage
{
void FreeImageWrapper::saveImage(int width, int height)
{
  // For now, just a dummy screen shot to see if things work.
  const auto totalNumberOfPixels = width * height;
  const auto pPixels = std::make_unique<BYTE[]>(3 * totalNumberOfPixels);

  const auto pImage = FreeImage_ConvertFromRawBits(
    pPixels.get(), width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  FreeImage_Save(FIF_PNG, pImage, "dummy.png", 0);
}
}
