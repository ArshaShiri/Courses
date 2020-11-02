#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CompilationEngine.h"
#include "JackTokenizer.h"

int main()
{
  try
  {
    auto tokenizer = JackTokenizer{"SquareGame.jack"};
    CompilationEngine{"SquareGame", tokenizer};
  }
  catch (std::runtime_error &e)
  {
    std::cout << "Caught a runtime_error exception: "
      << e.what() << '\n';
  }
  return 0;
}