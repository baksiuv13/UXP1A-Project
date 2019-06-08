// Copyright 2019 UXP1A Students Team

#include "src/linda.h"
#include <zconf.h>
#include <cassert>

namespace uxp {

void Linda::Output(Tuple tuple) {
  // [TODO] empty tuple

  // Only for debug;
  serviceQueue.P();
  resourceAccess.P();
  serviceQueue.V();
  // Tuple *tuples = &TupleAt_(0);

  size_t i;
  for (i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (t->size != 0) {
      continue;
    }
    *t = tuple;
    resourceAccess.V();
    return;
  }
  resourceAccess.V();
  // We did not find empty place. We should wait??
  assert(((void)&Linda::Output, 0));  // Temporary solution.
}

Tuple Linda::Input(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;

  StartTimer();
  do {
    serviceQueue.P();
    resourceAccess.P();
    serviceQueue.V();

    t = Find_(describe);

    if (t != nullptr) {
      Tuple ret = *t;
      ZeroTuple_(t);
      resourceAccess.V();
      return ret;
    }
    resourceAccess.V();
  } while (!IsTimeout(timeout_ms));  // or other option for waiting/sleeping
  return Tuple{0};
}

void Linda::ZeroTuple_(Tuple *t) { t->size = 0; }

Tuple Linda::Read(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  Tuple ret;
  bool find = false;

  StartTimer();
  do {
    serviceQueue.P();
    readCountAccess.P();
    if (readCount.isZero()) resourceAccess.P();
    readCount.V();
    serviceQueue.V();
    readCountAccess.V();

    t = Find_(describe);

    if (t != nullptr) {
      ret = *t;
      find = true;
    }

    readCountAccess.P();
    readCount.P();
    if (readCount.isZero()) resourceAccess.V();
    readCountAccess.V();

    if (find) return ret;
  } while (!IsTimeout(timeout_ms));  // or other option for waiting/sleeping

  return Tuple{0};
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

void Linda::StartTimer() {
  start_time_point = std::chrono::system_clock::now();
}

bool Linda::IsTimeout(unsigned int timeout_ms) {
  auto timeout = std::chrono::milliseconds(timeout_ms);
  auto now = std::chrono::system_clock::now();
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      now - start_time_point);
  return duration_ms > timeout;
}

}  // namespace uxp
