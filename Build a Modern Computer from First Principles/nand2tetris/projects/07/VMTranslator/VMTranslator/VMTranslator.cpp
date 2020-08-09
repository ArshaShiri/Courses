#include <utility>

#include "translator.h"

int main()
{
  auto translator = Translator{"SimpleAdd.vm"};
  translator.translate();

  return 0;
}