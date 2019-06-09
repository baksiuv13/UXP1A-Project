// Copyright 2019 UXP1A Students Team

#include "src/tuple.h"

namespace uxp {

std::string Tuple::ToString() {
  std::string out = "(";
  for (size_t i = 0; i < size; ++i) {
    out += elements[i].ToString();
    if (i != size - 1) out += ", ";
  }
  out += ")";
  return out;
}

bool CheckTuple(const Tuple *t, const TupleDesc *td) {
  size_t elem_n;
  if (t->size != td->size) {
    return false;
  }
  for (elem_n = 0; elem_n < t->size; ++elem_n) {
    if (!CheckElement(&t->elements[elem_n], &td->elements[elem_n])) {
      return false;
    }
  }
  return true;
}

}  // namespace uxp
