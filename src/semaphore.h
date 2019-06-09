// Copyright 2019 UXP1A Students Team

#ifndef SRC_SEMAPHORE_H_
#define SRC_SEMAPHORE_H_

#include <pthread.h>
#include <sys/sem.h>
#include <iostream>

namespace uxp {

class Semaphore {
 private:
  const uint16_t semNum;

 public:
  // constants for ftok()
  static const char SEM_KEY_PATH[];
  static const int32_t SEM_KEY_PROJ_ID;

  static const int32_t N_SEMS;

  static const key_t SEM_KEY;
  // global semaphore identifier
  static const int32_t SEM_ID;

  static bool initializeAll(const int32_t value);
  static bool closeSemTable();

  explicit Semaphore(const uint16_t semNum) : semNum(semNum) {}

  bool P();
  bool V();
  bool initialize(const int32_t value);
  int32_t getValue();
  bool isZero();
};

}  // namespace uxp

#endif  // SRC_SEMAPHORE_H_
