// Copyright 2019 UXP1A Students Team

#ifndef SRC_SEMAPHORE_H_
#define SRC_SEMAPHORE_H_

#include <pthread.h>
#include <sys/sem.h>
#include <iostream>

namespace uxp {

class Semaphore;

class SemaphoreTable {
  friend class Semaphore;

 public:
  static constexpr int N_SEMS = 13;
  SemaphoreTable() = delete;
  SemaphoreTable(const SemaphoreTable &) = delete;
  SemaphoreTable(const char *path);
  ~SemaphoreTable();

 private:
  static constexpr int SEM_PROJ_ID = 13;

  bool P_(unsigned short n);
  bool V_(unsigned short n);
  int GetValue_(unsigned short n);
  bool IsZero_(unsigned short n) { return GetValue_(n) == 0; }
  bool InitializeOne_(unsigned short sem, int value);
  bool InitializeAll_();
  bool CloseSemTable_();

  key_t key_;
  int semid_;
};

class Semaphore {
 public:
  explicit Semaphore(SemaphoreTable *tab, unsigned short num);

  bool P() { return sem_tab_->P_(sem_num_); }
  bool V() { return sem_tab_->V_(sem_num_); }
  int GetValue() { return sem_tab_->GetValue_(sem_num_); }
  bool IsZero() { return sem_tab_->IsZero_(sem_num_); }
  bool Initialize(int val) { return sem_tab_->InitializeOne_(sem_num_, val); }

 private:
  bool ValidNum_(unsigned short n) {
    return n >= 1 && n < SemaphoreTable::N_SEMS;
  }

  SemaphoreTable *sem_tab_;
  unsigned short sem_num_;
};

}  // namespace uxp

#endif  // SRC_SEMAPHORE_H_
