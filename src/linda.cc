// Copyright 2019 UXP1A Students Team

#include <cassert>
#include <zconf.h>
#include "src/linda.h"

namespace uxp {

void Linda::Output(Tuple tuple) {
  // [TODO] empty tuple

  // Only for debug;
  serviceQueue.p();
  resourceAccess.p();
  serviceQueue.v();
  Tuple *tuples = &TupleAt_(0);

  size_t i;
  for (i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (t->size != 0) {
      continue;
    }
    *t = tuple;
    resourceAccess.v();
    return;
  }
  resourceAccess.v();
  // We did not find empty place. We should wait??
  assert(((void) &Linda::Output, 0));  // Temporary solution.
}

Tuple Linda::Input(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;

  do{
    serviceQueue.p();
    resourceAccess.p();
    serviceQueue.v();

    t = Find_(describe);

    if(t != nullptr){
      Tuple ret = *t;
      ZeroTuple_(t);
      resourceAccess.v();
      return ret;
    }
    resourceAccess.v();
  } while (sleep(1));  // or other option for waiting/sleeping
}

void Linda::ZeroTuple_(Tuple *t) { t->size = 0; }

Tuple Linda::Read(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  Tuple ret;
  bool find = false;

  do{
    serviceQueue.p();
    readCountAccess.p();
    if(readCount.equalZero())
      resourceAccess.p();
    readCount.v();
    serviceQueue.v();
    readCountAccess.v();

    t = Find_(describe);

    if(t != nullptr){
      ret = *t;
      find = true;
    }

    readCountAccess.p();
    readCount.p();
    if(readCount.equalZero())
      resourceAccess.v();
    readCountAccess.v();

    if(find)
      return ret;
  } while (sleep(1));  // or other option for waiting/sleeping
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
