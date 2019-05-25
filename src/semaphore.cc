// Copyright 2019 UXP1A Students Team

#include "src/semaphore.h"

bool uxp::P(uint16_t semNum) {
  if (semNum >= 0 && semNum < 13) {
    struct sembuf operation = {semNum, -1, SEM_UNDO};
    semop(uxp::semId, &operation, 1);
    return true;
  } else {
    return false;
  }
}

bool uxp::V(uint16_t semNum) {
  if (semNum >= 0 && semNum < 13) {
    struct sembuf operation = {semNum, 1, SEM_UNDO};  //  1 - operatiopn value
    semop(uxp::semId, &operation, 1);  // 1 - only one operation
    return true;
  } else {
    return false;
  }
}
