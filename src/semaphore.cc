// Copyright 2019 UXP1A Students Team

#include "src/semaphore.h"

bool uxp::semaphore::P(uint16_t semNum) {
  if (semNum >= 0 && semNum < 13) {
    struct sembuf operation = {semNum, -1, SEM_UNDO};
    semop(semId, &operation, 1);
    return true;
  } else {
    return false;
  }
}

bool uxp::semaphore::V(uint16_t semNum) {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, 1, SEM_UNDO};  //  1 - operatiopn value
    semop(semId, &operation, 1);  // 1 - only one operation
    return true;
  } else {
    return false;
  }
}

bool uxp::semaphore::initializeAll(int32_t value) {
  int rc = semctl(semId, 0, SETALL, value);
  if (rc < 0) {
    return false;
  } else {
    return true;
  }
}

bool uxp::semaphore::initialize(uint16_t semNum, int32_t value) {
  int rc = semctl(semId, semNum, SETVAL, value);
  if (rc < 0) {
    return false;
  } else {
    return true;
  }
}

uxp::semaphore::Semaphore::Semaphore(uint16_t semNum): semNum(semNum) {}

bool uxp::semaphore::Semaphore::P() {
  return uxp::semaphore::P(this->semNum);
}

bool uxp::semaphore::Semaphore::V() {
  return uxp::semaphore::V(this->semNum);
}

bool uxp::semaphore::Semaphore::initialize(int32_t value) {
  return uxp::semaphore::initialize(this->semNum, value);
}
