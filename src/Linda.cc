// Copyright 2019 UXP1A Students Team

#include <cassert>
#include "src/Linda.h"

namespace uxp {

void Linda::Output(Tuple tuple) {
  // [TODO] empty tuple

  // Only for debug;
  Tuple *tuples = &TupleAt_(0);

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
  assert(((void) &Linda::Output, 0));  // Temporary solution.
}

Tuple Linda::Input(TupleDesc describe, unsigned int timeout_ms) {
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

Tuple Linda::Read(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  while ((t = Find_(describe)) == nullptr) {
    // czekanie??
  }
  return *t;
}

Tuple *Linda::Find_(const TupleDesc &td) {
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

}  // namespace uxp