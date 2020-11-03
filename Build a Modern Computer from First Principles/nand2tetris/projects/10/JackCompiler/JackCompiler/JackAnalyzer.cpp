#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CompilationEngine.h"
#include "JackTokenizer.h"


#if defined(WIN32) || defined(_WIN32) 
  #define PATH_SEPARATOR "\\"
#else 
  #define PATH_SEPARATOR "/"
#endif

using FileNameAndAddress = std::vector<std::pair<std::string, std::string>>;

std::vector<std::string> getListOfJackFiles(const std::string &fileOrDirPath)
{

  auto listOfJackFiles = std::vector<std::string>{};
  const auto extensionIndicator = '.';
  const auto extensionIndicatorLocation = fileOrDirPath.find_last_of(extensionIndicator);

  // If we cannot find an extension, it means a directory given for compilation.
  // We then have to compile all the jack files in that directory.
  const auto isFilePathADirectory = extensionIndicatorLocation == fileOrDirPath.npos;

  if (isFilePathADirectory)
  {
    const auto inputFileExtension = ".jack";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(fileOrDirPath.c_str())) != NULL)
    {
      while ((ent = readdir(dir)) != NULL)
      {
        const auto fileName = std::string{ent->d_name};
        const auto extensionLocation = fileName.find(inputFileExtension);
        if (extensionLocation != fileName.npos)
        {
          const auto fileNameWithNoExtension = fileName.substr(0, extensionLocation);
          const auto inputFilePath = fileOrDirPath + PATH_SEPARATOR + fileNameWithNoExtension;
          listOfJackFiles.emplace_back(inputFilePath);
        }
      }
    }
    else
      throw std::runtime_error("Cannot open the given directory!");
  }
  else
    listOfJackFiles.emplace_back(fileOrDirPath);

  return listOfJackFiles;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    std::cout << "Only two command line arguments are expected!" << std::endl;

  try
  {
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