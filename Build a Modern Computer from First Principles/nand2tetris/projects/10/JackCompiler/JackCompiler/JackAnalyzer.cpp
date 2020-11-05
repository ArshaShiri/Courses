#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CompilationEngine.h"
#include "Helper.h"
#include "JackTokenizer.h"

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 2)
      throw std::runtime_error("Wrong number of command line arguments: " + argc);

    const auto jackFilesList = getListOfJackFiles(argv[1]);

    for (const auto &inputFile : jackFilesList)
    {
      auto tokenizer = JackTokenizer{inputFile + ".jack"};
      CompilationEngine{inputFile + "Ans_.xml", tokenizer};
    }
  }
  catch (std::runtime_error &e)
  {
    std::cout << "Caught a runtime_error exception: "
      << e.what() << '\n';
  }
  return 0;
}