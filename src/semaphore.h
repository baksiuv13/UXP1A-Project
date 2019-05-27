// Copyright 2019 UXP1A Students Team

#ifndef SRC_SEMAPHORE_H_
#define SRC_SEMAPHORE_H_

#include <sys/sem.h>
#include <pthread.h>
#include <iostream>

namespace uxp {
namespace semaphore {
  // constants for ftok()
static const char SEM_KEY_PATH[] = "./semkeypath";
static const int32_t SEM_KEY_PROJ_ID = 13;
static const int32_t N_SEMS = 13;

// global semaphore identifier
static key_t semKey = ftok(SEM_KEY_PATH, SEM_KEY_PROJ_ID);
static int32_t semId = semget(semKey, N_SEMS, 0666 | IPC_CREAT);

enum {
  semaphore,
  semaphore1};  // to semNum

bool P(uint16_t semNum);
bool V(uint16_t semNum);

class Semaphore {
 private:
  uint16_t semNum;

 public:
  explicit Semaphore(uint16_t semNum);
  bool P();
  bool V();
};

}  // namespace semaphore
}  // namespace uxp

#endif  // SRC_SEMAPHORE_H_
