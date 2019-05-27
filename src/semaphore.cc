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

uxp::semaphore::Semaphore::Semaphore(uint16_t semNum): semNum(semNum) {
  std::cout << semId;
}

bool uxp::semaphore::Semaphore::P() {
  return uxp::semaphore::P(this->semNum);
}

bool uxp::semaphore::Semaphore::V() {
  return uxp::semaphore::V(this->semNum);
}
