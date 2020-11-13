#pragma once

#include <string>
#include <vector>

/* Get a list of jack files. A jack file path or a directory path is passed to it. In case a 
   directory is passed, a list of all jack files inside the directory is returned.*/
std::vector<std::string> getListOfJackFiles(const std::string &fileOrDirPath);

/* Helper functions to specify if a character is considered a symbol, an operator or an unary
   operator based on Jack documentation. */
bool isSymbol(const char character);
bool isOp(const char character);
bool isUnaryOp(const char character);