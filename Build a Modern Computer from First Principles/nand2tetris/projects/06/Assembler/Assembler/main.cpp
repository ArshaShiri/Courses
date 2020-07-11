#include "Assembler.h"

int main()
{
  auto assembler = Assembler{"Rect"};
  // assembler.writeProgramLinesToAfile();
  assembler.translate();
  assembler.saveFile();
  return 0;
}