#include "Parser.h"
#include "FreeImageWrapper.h"

int main()
{
  auto scene = Scene();
  scene.createSceneFromInputFile("scene1.test");

  FreeImage::FreeImageWrapper::saveImage(100, 100);
  return 0;
}