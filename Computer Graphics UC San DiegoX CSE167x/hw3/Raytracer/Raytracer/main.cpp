#include "Parser.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
    throw std::runtime_error("Wrong number of command line arguments: " + argc);

  const auto inputFilename = std::string{argv[1]};
  auto scene = Scene{};
  scene.createSceneFromInputFile(inputFilename);
  scene.render();
  scene.save();

  return 0;
}