#pragma once

enum class CommandType {
  C_ARITHMETIC, C_PUSH, C_POP, C_LABLE, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, C_UNKNOWN};