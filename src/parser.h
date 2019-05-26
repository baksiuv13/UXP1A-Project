// Copyright 2019 UXP1A Students Team

#ifndef SRC_PARSER_H_
#define SRC_PARSER_H_

/*

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cinttypes>

#include "src/type.h"

namespace uxp {

namespace Parser {

constexpr const char *IntName = "integer";
constexpr const char *FloatName = "float";
constexpr const char *StringName = "string";

ElemType Type(const char *s) {
  if (strncmp(s, IntName, strlen(IntName)) == 0) return ElemType::INT;
  if (strncmp(s, FloatName, strlen(FloatName)) == 0) return ElemType::FLOAT;
  if (strncmp(s, StringName, strlen(StringName)) == 0) return ElemType::STRING;
  return ElemType::ERR;
}

int32_t Int(const char *s) {
  int32_t result;
  sscanf(s, "%" PRId32, &result);
  return result;
}

float Float(const char *s) {
  float result;
  sscanf(s, "%f", &result);
  return result;
}

Element GetElement(const char *)

}  // namespace Parser

}  // namespace uxp

*/

#endif  // SRC_PARSER_H_
