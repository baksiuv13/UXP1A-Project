// Copyright 2019 UXP1A Students Team

#include "src/tuple.h"

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

}  // namespace uxp
