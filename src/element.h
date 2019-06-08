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
    str_size = WriteString(s);
  }

  std::string ToString();

 private:
  size_t WriteString(const char *s);
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
  explicit ElementDesc(const char *s, Condition cnd)
      : type(Type::STRING), condition(cnd) {
    if (cnd != ANY) str_size = WriteString(s);
  }

 private:
  size_t WriteString(const char *s);
};

bool CheckElement(const Element *, const ElementDesc *);

}  // namespace uxp

#endif  // SRC_ELEMENT_H_
