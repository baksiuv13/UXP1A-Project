// Copyright 2019 UXP1A Students Team

#include "src/memory_chunk.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

namespace uxp {
/*
void *MemoryChunk::CreateNewMem_(const char *path, int key_id) {
  int shm_key = ftok(path, key_id);
  if (shm_key < 0)
    return nullptr;
  int shm_id = shmget(shm_key, MEMORY_SIZE, IPC_CREAT | IPC_EXCL | 0660);
  if (shm_id < 0) {
    return nullptr;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    shmctl(shm_id, IPC_RMID, nullptr);
    return nullptr;
  }
  return mem;
}
*/

MemoryChunk::MemoryChunk()
    : state_(State::BLANK),
      shm_id_(0),
      sem_id_(0),
      key_(0),
      address_(nullptr),
      size_(0) {}

MemoryChunk::MemoryChunk(const char *path, size_t size) : MemoryChunk() {
  CreateNewMem_(path, size);
}

MemoryChunk::MemoryChunk(const char *path) : MemoryChunk() {
  AttachNotNew_(path);
}

MemoryChunk::~MemoryChunk() {
  if (IsOpen()) {
    Close_();
  }
}

int MemoryChunk::CreateNewMem_(const char *path, size_t size) {
  assert(state_ == BLANK);

  key_t shm_key = ftok(path, SHM_PROJ_ID);
  key_t sem_key = ftok(path, SEM_PROJ_ID);
  if (static_cast<int>(shm_key) < 0 || static_cast<int>(sem_key) < 0) return -1;
  int shm_id = shmget(shm_key, size, IPC_CREAT | IPC_EXCL | 0660);
  if (shm_id < 0) {
    return -2;
  }
  int sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0660);
  if (sem_id < 0) {
    return -3;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    shmctl(shm_id, IPC_RMID, nullptr);
    return -4;
  }
  // struct sembuf smbf;
  // smbf.sem_flg = SEM_UNDO | IPC_NOWAIT;
  // smbf.sem_num = 0;
  // smbf.sem_op = 1;
  int semctl_ret = semctl(sem_id, 0, SETVAL, 0);
  // int semop_ret = semop(sem_id, &smbf, 1);
  if (semctl_ret < 0 /* || semctl_ret < 0 */) {
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, nullptr);
    return -5;
    // [TODO] Do clean up correctly.
  }
  state_ = CREATED;
  shm_id_ = shm_id;
  sem_id_ = sem_id;
  address_ = mem;
  key_ = shm_key;
  struct shmid_ds shmds;
  int e = shmctl(shm_id, IPC_STAT, &shmds);
  assert(e >= 0);
  size_ = shmds.shm_segsz;
  return 0;
}

int MemoryChunk::AttachNotNew_(const char *path) {
  assert(state_ == BLANK);

  key_t shm_key = ftok(path, SHM_PROJ_ID);
  key_t sem_key = ftok(path, SEM_PROJ_ID);
  if (static_cast<int>(shm_key) < 0 || static_cast<int>(sem_key) < 0) {
    return -1;
  }
  int shm_id = shmget(shm_key, 0, 0);
  if (shm_id < 0) {
    return -2;
  }
  int sem_id = semget(sem_key, 0, 0);
  if (sem_id < 0) {
    return -3;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    return -3;
  }
  struct sembuf smbf;
  smbf.sem_flg = SEM_UNDO | IPC_NOWAIT;
  smbf.sem_num = 0;
  smbf.sem_op = 1;
  int semop_ret = semop(sem_id, &smbf, 1);
  if (semop_ret < 0) {
    return -5;
  }
  state_ = GIVEN;
  shm_id_ = shm_id;
  sem_id_ = sem_id;
  address_ = mem;
  key_ = shm_key;
  struct shmid_ds shmds;
  int e = shmctl(shm_id, IPC_STAT, &shmds);
  assert(e >= 0);
  size_ = shmds.shm_segsz;
  return 0;
}

int MemoryChunk::AttachNew(const char *path, size_t size) {
  int e = CreateNewMem_(path, size);
  if (e < 0)
    return e;
  else
    return shm_id_;
}

int MemoryChunk::Attach(const char *path) {
  int e = AttachNotNew_(path);
  if (e < 0)
    return e;
  else
    return shm_id_;
}

void MemoryChunk::Close_() {
  struct sembuf smbf;
  smbf.sem_op = -1;
  smbf.sem_num = 0;
  smbf.sem_flg = IPC_NOWAIT | SEM_UNDO;
  shmdt(address_);
  int semop_ret = semop(sem_id_, &smbf, 1);
  if (semop_ret < 0) {
    if (errno == EAGAIN) {
      // Jest dobrze, jeśli tu jesteśmy, to znaczy, że jako ostatni zamykamy
      // naszą pamięć.

      semctl(sem_id_, 0, IPC_RMID);
      shmctl(shm_id_, IPC_RMID, nullptr);
    } else {
      std::cerr << "blad xd\n";
    }
  }
  state_ = BLANK;
}

void MemoryChunk::Detach() {
  if (!IsOpen()) {
    return;  // Now we do nothing.
  }
  return Close_();
}

char &MemoryChunk::operator[](size_t i) {
  return IsOpen() ? static_cast<char *>(GetMem())[i]
                  : *static_cast<char *>(nullptr);
}

}  // namespace uxp
