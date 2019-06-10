// Copyright 2019 UXP1A Students Team

#ifndef SRC_LINDA_H_
#define SRC_LINDA_H_

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>

#include "src/memory_chunk.h"
#include "src/semaphore.h"
#include "src/tuple.h"
#include "src/type.h"

namespace uxp {

class Linda {
 public:
  static constexpr size_t TUPLES_NUMBER = 100;

  using Type = ElemType;

 public:
  // Link existing memory.
  Linda() = delete;
  explicit Linda(const char *path)
      : mc_(path, MEM_SIZE),
        serviceQueue(0),
        resourceAccess(1),
        readCountAccess(2),
        readCount(3) {
    if (mc_.NewlyCreated()) {
      mc_.ZeroMemory();
      for (auto sem :
           {serviceQueue, resourceAccess, readCountAccess, readCount})
        sem.initialize(1);
    }
  }

  bool Output(Tuple tuple);
  Tuple Input(TupleDesc describe, unsigned int timeout_ms);
  Tuple Read(TupleDesc describe, unsigned int timeout_ms);

 private:
  static constexpr size_t TUPLES_START = 0;
  static constexpr size_t MEM_SIZE =
      TUPLES_START + TUPLES_NUMBER * sizeof(Tuple);
  static void ZeroTuple_(Tuple *tuple) { tuple->size = 0; }

  Tuple &TupleAt_(size_t n) {
    return reinterpret_cast<Tuple *>(&mc_.at(TUPLES_START))[n];
  }
  Tuple *Find_(const TupleDesc &describe);

  void StartTimer();
  bool IsTimeout(unsigned int timeout_ms);

  MemoryChunk mc_;
  Semaphore serviceQueue, resourceAccess, readCountAccess, readCount;
  std::chrono::system_clock::time_point start_time_point;
};

}  // namespace uxp

#endif  // SRC_LINDA_H_
