// Copyright 2019 UXP1A Students Team

#include "src/element.h"

namespace uxp {

bool ChkElem(const Element *e, const ElementDesc *ed) {
  if (e->type != ed->type) {
    return false;
  }
  auto t = e->type;
  switch (ed->condition) {
    case ElementDesc::LESS:
      return t == INT
             ? e->value.int_ < ed->value.int_
             : t == FLOAT ? e->value.float_ < ed->value.float_
                          : strncmp(e->value.string_, ed->value.string_,
                                    MAX_STRING_SIZE) < 0;
    case ElementDesc::GREATER:
      return t == INT
             ? e->value.int_ > ed->value.int_
             : t == FLOAT ? e->value.float_ > ed->value.float_
                          : strncmp(e->value.string_, ed->value.string_,
                                    MAX_STRING_SIZE) > 0;
    case ElementDesc::LESS_EQ:
      return t == INT
             ? e->value.int_ <= ed->value.int_
             : t == FLOAT ? e->value.float_ <= ed->value.float_
                          : strncmp(e->value.string_, ed->value.string_,
                                    MAX_STRING_SIZE) <= 0;
    case ElementDesc::GREATER_EQ:
      return t == INT
             ? e->value.int_ >= ed->value.int_
             : t == FLOAT ? e->value.float_ >= ed->value.float_
                          : strncmp(e->value.string_, ed->value.string_,
                                    MAX_STRING_SIZE) >= 0;
    case ElementDesc::EQUAL:
      return t == INT
             ? e->value.int_ == ed->value.int_
             : t == FLOAT ? false
                          : strncmp(e->value.string_, ed->value.string_,
                                    MAX_STRING_SIZE) == 0;
    case ElementDesc::ANY:
    default:return true;
  }
}

}  // namespace uxp
