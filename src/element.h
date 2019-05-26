// Copyright 2019 UXP1A Students Team

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <cstdint>
#include <cstdlib>
#include <string>
#include <cstring>
#include "src/type.h"

namespace uxp {

using Type = ElemType;

constexpr size_t MAX_STRING_SIZE = 200;

struct Element {
  Type type;
  size_t str_size;
  union {
    int32_t int_;
    float float_;
    char string_[MAX_STRING_SIZE];
  } value;
};

struct ElementDesc {
  Type type;
  enum Condidtion : int16_t {
    ANY,         // *
    LESS,        // <
    GREATER,     // >
    LESS_EQ,     // <=
    GREATER_EQ,  // >=
    EQUAL        // ==
  } condition;
  size_t str_size;
  union {
    int32_t int_;
    float float_;
    char string_[MAX_STRING_SIZE];
  } value;
};

bool ChkElem(const Element *, const ElementDesc *);


}  // namespace uxp

#endif  // SRC_ELEMENT_H_
