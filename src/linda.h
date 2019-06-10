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
      : sem_tab_(path),
        serviceQueue(&sem_tab_, 1),
        resourceAccess(&sem_tab_, 2),
        readCountAccess(&sem_tab_, 3),
        readCount(&sem_tab_, 4),
        memorySem(&sem_tab_, 5),
        mc_(path, MEM_SIZE, &memorySem) {}

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

  SemaphoreTable sem_tab_;
  Semaphore serviceQueue, resourceAccess, readCountAccess, readCount, memorySem;
  MemoryChunk mc_;
  std::chrono::system_clock::time_point start_time_point;
};

}  // namespace uxp

#endif  // SRC_LINDA_H_
