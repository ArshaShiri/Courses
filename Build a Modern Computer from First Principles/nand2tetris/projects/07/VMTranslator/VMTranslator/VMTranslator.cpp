#include <utility>

#include "translator.h"

int main(int argc, char *argv[])
{
  auto translator = Translator{argv[1]};
  translator.translate();

  return 0;
}