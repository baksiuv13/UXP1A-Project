// Copyright 2019 UXP1A Students Team

#ifndef SRC_LOCK_GUARD_H_
#define SRC_LOCK_GUARD_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "src/type.h"

namespace uxp {

template<typename T>
class LockGuard {
 public:
  LockGuard() = delete;
  LockGuard(const LockGuard&) = delete;
  explicit LockGuard(T &t) : semaphore_(&t) {
    t.P();
  }
  ~LockGuard() {
    semaphore_->V();
  }

 private:
  T *semaphore_;
};

}  // namespace uxp

#endif  // SRC_LOCK_GUARD_H_
