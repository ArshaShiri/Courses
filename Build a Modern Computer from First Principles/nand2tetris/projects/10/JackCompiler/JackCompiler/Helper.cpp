#include <dirent.h>
#include <stdexcept>

#include "Helper.h"

#if defined(WIN32) || defined(_WIN32) 
  #define PATH_SEPARATOR "\\"
#else 
  #define PATH_SEPARATOR "/"
#endif

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
      throw std::runtime_error("Cannot open the given directory: " + fileOrDirPath);
  }
  else
    listOfJackFiles.emplace_back(fileOrDirPath);

  return listOfJackFiles;
}

bool isOp(const char character)
{
  if (character == '+') return true;
  if (character == '-') return true;
  if (character == '*') return true;

  if (character == '/') return true;
  if (character == '&') return true;
  if (character == '|') return true;

  if (character == '<') return true;
  if (character == '>') return true;
  if (character == '=') return true;

  return false;
}

bool isUnaryOp(const char character)
{
  if (character == '-') return true;
  if (character == '~') return true;

  return false;
}

bool isSymbol(const char character)
{
  if (isOp(character))       return true;
  if (isUnaryOp(character))  return true;

  if (character == '}')      return true;
  if (character == '(')      return true;
  if (character == '{')      return true;

  if (character == ')')      return true;
  if (character == '[')      return true;
  if (character == ']')      return true;

  if (character == '.')      return true;
  if (character == ',')      return true;
  if (character == ';')      return true;

  return false;
}