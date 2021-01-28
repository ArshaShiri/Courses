#include "Parser.h"

int main()
{
  const auto parser = Parser("scene1.test");
  const auto &scene = parser.getCreatedScene();

  return 0;
}