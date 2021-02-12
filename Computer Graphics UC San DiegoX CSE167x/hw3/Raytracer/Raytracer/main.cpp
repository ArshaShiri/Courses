#include "Parser.h"

int main()
{
  auto scene = Scene{};
  scene.createSceneFromInputFile("scene1.test");
  scene.render();

  return 0;
}