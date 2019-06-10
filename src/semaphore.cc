// Copyright 2019 UXP1A Students Team

#include "src/semaphore.h"

#include <cstring>

#include "src/error.h"

namespace uxp {

namespace {

static int sem_flag = IPC_CREAT | 0666;  // Maybe 0660 ??

}  // namespace

SemaphoreTable::SemaphoreTable(const char *path) {
  key_ = ftok(path, SEM_PROJ_ID);
  if (key_ < 0) Throw<std::runtime_error>("Could not get sem_key");
  semid_ = semget(key_, N_SEMS, sem_flag);
  if (semid_ < 0) Throw<std::runtime_error>("Could not open sem table");
  struct semid_ds sem;
  int res = semctl(semid_, 0, IPC_STAT, &sem);
  if (res < 0) Throw<std::runtime_error>("Could not get stat of sem table");
  if (sem.sem_otime == 0) {
    bool init_res = InitializeAll_();
    if (!init_res) Throw<std::runtime_error>("Could not initialize semaphores");
  }
  V_(0);  // Add ONE to counting semaphore.
}

SemaphoreTable::~SemaphoreTable() {
  P_(0);  // Subtract ONE from counting semaphore.
  if (IsZero_(0))
    CloseSemTable_();
}

bool SemaphoreTable::InitializeAll_() {
  ushort vals[N_SEMS];
  vals[0] = 0;
  for (size_t i = 1; i < N_SEMS; ++i) vals[i] = 1;
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

Semaphore::Semaphore(SemaphoreTable *tab, unsigned short num)
    : sem_tab_(tab), sem_num_(num) {
  if (!ValidNum_(num)) Throw<std::out_of_range>("Bad semaphore number");
}

}  // namespace uxp
