// Copyright 2019 UXP1A Students Team

#include "src/element.h"

namespace uxp {

bool CheckElement(const Element *e, const ElementDesc *ed) {
  if (e->type != ed->type) {
    return false;
  }
  auto type = e->type;
  switch (ed->condition) {
    case ElementDesc::LESS:
      return type == INT
                 ? e->value.int_ < ed->value.int_
                 : type == FLOAT ? e->value.float_ < ed->value.float_
                                 : strncmp(e->value.string_, ed->value.string_,
                                           MAX_STRING_SIZE) < 0;
    case ElementDesc::GREATER:
      return type == INT
                 ? e->value.int_ > ed->value.int_
                 : type == FLOAT ? e->value.float_ > ed->value.float_
                                 : strncmp(e->value.string_, ed->value.string_,
                                           MAX_STRING_SIZE) > 0;
    case ElementDesc::LESS_EQ:
      return type == INT
                 ? e->value.int_ <= ed->value.int_
                 : type == FLOAT ? e->value.float_ <= ed->value.float_
                                 : strncmp(e->value.string_, ed->value.string_,
                                           MAX_STRING_SIZE) <= 0;
    case ElementDesc::GREATER_EQ:
      return type == INT
                 ? e->value.int_ >= ed->value.int_
                 : type == FLOAT ? e->value.float_ >= ed->value.float_
                                 : strncmp(e->value.string_, ed->value.string_,
                                           MAX_STRING_SIZE) >= 0;
    case ElementDesc::EQUAL:
      return type == INT
                 ? e->value.int_ == ed->value.int_
                 : type == FLOAT ? false
                                 : strncmp(e->value.string_, ed->value.string_,
                                           MAX_STRING_SIZE) == 0;
    case ElementDesc::ANY:
    default:
      return true;
  }
}

size_t Element::WriteString(const char *s) {
  size_t i;
  char c;
  for (i = 0; i < MAX_STRING_SIZE - 1 && (c = s[i]) != '\0'; ++i) {
    value.string_[i] = c;
  }
  value.string_[i] = '\0';
  return i;
}

size_t ElementDesc::WriteString(const char *s) {
  size_t i;
  char c;
  for (i = 0; i < MAX_STRING_SIZE - 1 && (c = s[i]) != '\0'; ++i) {
    value.string_[i] = c;
  }
  value.string_[i] = '\0';
  return i;
}

std::string Element::ToString() {
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

}  // namespace uxp
