#include "Parser.h"

int main()
{
  auto scene = Scene{};
  scene.createSceneFromInputFile("scene3.test");
  scene.renderAndSave();

  return 0;
}