#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CompilationEngine.h"
#include "JackTokenizer.h"

int main()
{
  auto tokenizer = JackTokenizer{"Main.jack"};
  CompilationEngine{"Main", tokenizer};
  return 0;
}