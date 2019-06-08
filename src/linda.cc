// Copyright 2019 UXP1A Students Team

#include "src/linda.h"

namespace uxp {

int Linda::AttachNew(const char *path) {
  for (auto sem : {serviceQueue, resourceAccess, readCountAccess, readCount})
    sem.initialize(1);
  int result = mc_.AttachNew(path, MEM_SIZE);
  mc_.ZeroMemory();
  return result;
}

bool Linda::Output(Tuple tuple) {
  serviceQueue.P();
  resourceAccess.P();
  serviceQueue.V();

  for (size_t i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (t->size != 0) continue;

    *t = tuple;
    resourceAccess.V();
    return true;
  }
  resourceAccess.V();
  return false;
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
  } while (!IsTimeout(timeout_ms));
  return Tuple{0};
}

Tuple Linda::Read(TupleDesc describe, unsigned int timeout_ms) {
  Tuple *t;
  Tuple ret;
  bool found = false;

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
      found = true;
    }

    readCountAccess.P();
    readCount.P();
    if (readCount.isZero()) resourceAccess.V();
    readCountAccess.V();

    if (found) return ret;
  } while (!IsTimeout(timeout_ms));
  return Tuple{0};
}

Tuple *Linda::Find_(const TupleDesc &td) {
  for (size_t i = 0; i < TUPLES_NUMBER; ++i) {
    Tuple *t = &TupleAt_(i);
    if (CheckTuple(t, &td)) return t;
  }
  return nullptr;
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
