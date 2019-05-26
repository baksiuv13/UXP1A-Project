// Copyright 2019 UXP1A Students Team

#include "Tuple.h"

namespace uxp {

Tuple TupleFromString(const std::string &s) {
  Tuple result;
  TupleGen_(s, false, &result);
  return result;
}

TupleDesc TupleDescFromString(const std::string &s) {
  TupleDesc result;
  TupleGen_(s, true, &result);
  return result;
}

void TupleGen_(const std::string &s, bool desc, void *dest) {
  for (auto it = s.cbegin(); it != s.cend(); ++it) {
  }
}

bool ChkTuple(const Tuple *t, const TupleDesc *td) {
  size_t elem_n;
  if (t->size != td->size) {
    return false;
  }
  for (elem_n = 0; elem_n < t->size; ++elem_n) {
    if (!ChkElem(&t->elements[elem_n], &td->elements[elem_n])) {
      return false;
    }
  }
  return true;
}

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
