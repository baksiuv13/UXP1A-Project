// Copyright 2019 UXP1A Students Team

#include "src/Linda.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace uxp {

bool Linda::ChkTuple(const Linda::Tuple *t, const Linda::TupleDesc *td) {
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

bool Linda::ChkElem(const Linda::Element *e, const Linda::ElementDesc *ed) {
  if (e->type != ed->type) {
    return false;
  }
  Type t = e->type;
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
    default:
      return true;
  }
}

void Linda::Output(Linda::Tuple tuple) {
  // [TODO] empty tuple

  // Only for debug;
  Tuple *tuples = &TupleAt_(0);
  (void)tuples;

  size_t i;
  for (i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (t->size != 0) {
      continue;
    }
    *t = tuple;
    return;
  }
  // We did not find empty place. We should wait??
  assert(((void)&Linda::Output, 0));  // Temporary solution.
}

Linda::Tuple Linda::Input(Linda::TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  while ((t = Find_(describe)) == nullptr) {
    // czekanie??
  }
  Tuple ret;
  ret = *t;
  ZeroTuple_(t);
  return ret;
}

void Linda::ZeroTuple_(Tuple *t) { t->size = 0; }

Linda::Tuple Linda::Read(Linda::TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  while ((t = Find_(describe)) == nullptr) {
    // czekanie??
  }
  return *t;
}

Linda::Tuple *Linda::Find_(const Linda::TupleDesc &td) {
  // [TODO] empty tuple desc
  size_t i;
  for (i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (ChkTuple(t, &td)) {
      return t;
    }
  }
  return nullptr;  // did not find
}

Linda::Tuple Linda::TupleFromString(const std::string &s) {
  Tuple result;
  TupleGen_(s, false, &result);
  return result;
}
Linda::TupleDesc Linda::TupleDescFromString(const std::string &s) {
  TupleDesc result;
  TupleGen_(s, true, &result);
  return result;
}

void Linda::TupleGen_(const std::string &s, bool desc, void *dest) {
  for (auto it = s.cbegin(); it != s.cend(); ++it) {
  }
}

}  // namespace uxp
