// Copyright 2019 UXP1A Students Team

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
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

  Element() = default;
  ~Element() = default;
  explicit Element(int32_t i) : type(Type::INT), value{.int_ = i} {}
  explicit Element(float f) : type(Type::FLOAT), value{.float_ = f} {}
  explicit Element(const char *s) : type(Type::STRING) {
    size_t i;
    char c;
    for (i = 0; i < MAX_STRING_SIZE - 1 && (c = s[i]) != '\0'; ++i) {
      value.string_[i] = c;
    }
    value.string_[i] = '\0';
    str_size = i;
  }

  std::string ToString() {
    switch (type) {
      case Type::INT:
        return std::to_string(value.int_);
      case Type::FLOAT:
        return std::to_string(value.float_);
      case Type::STRING:
        return std::string(value.string_);
      default:
        return "ERROR";
    }
  }
};

struct ElementDesc {
  Type type;
  enum Condition : int16_t {
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

  ElementDesc() = default;
  ~ElementDesc() = default;
  explicit ElementDesc(int32_t i, Condition cnd)
      : type(Type::INT), condition(cnd), value{.int_ = i} {}
  explicit ElementDesc(float f, Condition cnd)
      : type(Type::FLOAT), condition(cnd), value{.float_ = f} {}
  explicit ElementDesc(const char *s, Condition cnd) : type(Type::STRING) {
    condition = cnd;
    if (cnd == ANY) return;
    size_t i;
    char c;
    for (i = 0; i < MAX_STRING_SIZE - 1 && (c = s[i]) != '\0'; ++i) {
      value.string_[i] = c;
    }
    value.string_[i] = '\0';
    str_size = i;
  }
};

bool ChkElem(const Element *, const ElementDesc *);

}  // namespace uxp

#endif  // SRC_ELEMENT_H_
