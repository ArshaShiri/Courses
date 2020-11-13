/* This program compiles a file written in jack language to virtual machine code which is then
   used to create machine level code. It is assumed that the given file to this compiler does not
   have any syntax errors. */

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

    // For each input jack file, create a tokenizer and then use that to compile the file.
    for (const auto &inputFile : jackFilesList)
    {
      auto tokenizer = JackTokenizer{inputFile + ".jack"};
      CompilationEngine{inputFile + ".xml", tokenizer};
    }
  }
  catch (std::runtime_error &e)
  {
    std::cout << "Caught a runtime_error exception: "
      << e.what() << '\n';
  }
  return 0;
}