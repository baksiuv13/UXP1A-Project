// Copyright 2019 UXP1A Students Team

#include <cstring>

#include "src/semaphore.h"

namespace uxp {

namespace {

static int sem_flag = IPC_CREAT | 0666; // Maybe 0660 ??

static std::string EXCEPTION_MSG(const char *msg) {
  int e = errno;
  errno = 0;
  return std::string(msg) + ":\n\t" + std::strerror(e) + '\n';
}
/*
static void ERROR_PRINT(const char *msg) {
  std::cerr << EXCEPTION_MSG(msg);
  errno = 0;
}
*/
template <typename T>
static void Throw(const char *msg) {
  throw T(EXCEPTION_MSG(msg));
}

}  // namespace

SemaphoreTable::SemaphoreTable(const char *path) {
  key_ = ftok(path, SEM_PROJ_ID);
  if (key_ < 0) Throw<std::runtime_error>("Could not get sem_key");
  semid_ = semget(key_, N_SEMS, sem_flag);
  if (semid_ < 0) Throw<std::runtime_error>("Could not open sem table");
  bool init_res = InitializeAll_(1);
  if (!init_res) Throw<std::runtime_error>("Could not initialize semaphores");
}

SemaphoreTable::~SemaphoreTable() {
  CloseSemTable_();
  // It has to be changed somehow.
  // We can't delete table in every process.
}

bool SemaphoreTable::InitializeAll_(int value) {
  ushort vals[N_SEMS];
  for (size_t i = 0; i < N_SEMS; ++i) vals[i] = value;
  int rc = semctl(semid_, 0, SETALL, &vals);
  return !(rc < 0);
}

bool SemaphoreTable::CloseSemTable_() {
  int rc = semctl(semid_, 0, IPC_RMID);  // 0 is ignored
  return !(rc < 0);
}

bool SemaphoreTable::P_(unsigned short semNum) {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, -1, SEM_UNDO};
    semop(semid_, &operation, 1);
    return true;
  } else {
    return false;
  }
}

bool SemaphoreTable::V_(unsigned short semNum) {
  if (semNum >= 0 && semNum < N_SEMS) {
    struct sembuf operation = {semNum, 1, SEM_UNDO};  //  1 - operation value
    semop(semid_, &operation, 1);                     // 1 - only one operation
    return true;
  } else {
    return false;
  }
}

bool SemaphoreTable::InitializeOne_(unsigned short semNum, int value) {
  int rc = semctl(semid_, semNum, SETVAL, value);
  return !(rc < 0);
}

int SemaphoreTable::GetValue_(unsigned short semNum) {
  return semctl(semid_, semNum, GETVAL);
}

}  // namespace uxp
