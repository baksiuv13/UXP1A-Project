// Copyright 2019 UXP1A Students Team

#include "src/semaphore.h"

const char uxp::Semaphore::SEM_KEY_PATH[] = "./semkeypath";
const int32_t uxp::Semaphore::SEM_KEY_PROJ_ID = 13;
const int32_t uxp::Semaphore::N_SEMS = 13;
const key_t uxp::Semaphore::SEM_KEY = ftok(SEM_KEY_PATH, SEM_KEY_PROJ_ID);
const int32_t uxp::Semaphore::SEM_ID = semget(SEM_KEY,
                                              N_SEMS,
                                              0666 | IPC_CREAT);

bool uxp::Semaphore::P(const uint16_t semNum) {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, -1, SEM_UNDO};
    semop(SEM_ID, &operation, 1);
    return true;
  } else {
    return false;
  }
}

bool uxp::Semaphore::V(const uint16_t semNum) {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, 1, SEM_UNDO};  //  1 - operatiopn value
    semop(SEM_ID, &operation, 1);  // 1 - only one operation
    return true;
  } else {
    return false;
  }
}

bool uxp::Semaphore::initializeAll(const int32_t value) {
  int rc = semctl(SEM_ID, 0, SETALL, value);
  if (rc < 0) {
    return false;
  } else {
    return true;
  }
}

bool uxp::Semaphore::initialize(const uint16_t semNum, const int32_t value) {
  int rc = semctl(SEM_ID, semNum, SETVAL, value);
  if (rc < 0) {
    return false;
  } else {
    return true;
  }
}

int32_t uxp::Semaphore::getValue(const uint16_t semNum) {
  return semctl(SEM_ID, semNum, GETVAL);
}

bool uxp::Semaphore::isZero(const uint16_t semNum) {
  return !getValue(semNum);
}

uxp::Semaphore::Semaphore(const uint16_t semNum): semNum(semNum) {}

bool uxp::Semaphore::P() {
  return uxp::Semaphore::P(this->semNum);
}

bool uxp::Semaphore::V() {
  return uxp::Semaphore::V(this->semNum);
}

bool uxp::Semaphore::initialize(const int32_t value) {
  return uxp::Semaphore::initialize(this->semNum, value);
}

int32_t uxp::Semaphore::getValue() {
  return semctl(SEM_ID, this->semNum, GETVAL);
}

bool uxp::Semaphore::isZero() {
  return !getValue(this->semNum);
}
