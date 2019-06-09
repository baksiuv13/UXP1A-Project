// Copyright 2019 UXP1A Students Team

#include "src/semaphore.h"

namespace uxp {

const char Semaphore::SEM_KEY_PATH[] = "./semkeypath";
const int32_t Semaphore::SEM_KEY_PROJ_ID = 13;
const int32_t Semaphore::N_SEMS = 13;
const key_t Semaphore::SEM_KEY = ftok(SEM_KEY_PATH, SEM_KEY_PROJ_ID);
const int32_t Semaphore::SEM_ID = semget(SEM_KEY, N_SEMS, 0666 | IPC_CREAT);

bool Semaphore::initializeAll(const int32_t value) {
  int rc = semctl(SEM_ID, 0, SETALL, value);
  return !(rc < 0);
}

bool Semaphore::closeSemTable() {
  int rc = semctl(SEM_ID, 0, IPC_RMID);  // 0 is ignored
  return !(rc < 0);
}

bool Semaphore::P() {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, -1, SEM_UNDO};
    semop(SEM_ID, &operation, 1);
    return true;
  } else {
    return false;
  }
}

bool Semaphore::V() {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, 1, SEM_UNDO};  //  1 - operation value
    semop(SEM_ID, &operation, 1);                     // 1 - only one operation
    return true;
  } else {
    return false;
  }
}

bool Semaphore::initialize(const int32_t value) {
  int rc = semctl(SEM_ID, semNum, SETVAL, value);
  return !(rc < 0);
}

int32_t Semaphore::getValue() { return semctl(SEM_ID, semNum, GETVAL); }

bool Semaphore::isZero() { return !getValue(); }

}  // namespace uxp
