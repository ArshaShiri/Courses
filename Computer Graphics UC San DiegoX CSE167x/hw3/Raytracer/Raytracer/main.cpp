#include "Parser.h"

int main()
{
  auto scene = Scene{};
  scene.createSceneFromInputFile("scene2.test");
  scene.renderAndSave();

  return 0;
}