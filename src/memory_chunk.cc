// Copyright 2019 UXP1A Students Team

#include "src/memory_chunk.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cstring>

namespace uxp {

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
  int res;
  key_t shm_key = ftok(path, SHM_PROJ_ID);
  if (shm_key < -1) {
    std::cerr << "Could not get shm_key:\n" << std::strerror(errno) << '\n';
    return -1;
  }
  key_t sem_key = ftok(path, SEM_PROJ_ID);
  if (sem_key < -1) {
    std::cerr << "Could not get sem_key:\n" << std::strerror(errno) << '\n';
    return -2;
  }
  int shm_id = shmget(shm_key, size, IPC_CREAT | IPC_EXCL | 0660);
  if (shm_id < 0) {
    std::cerr << "Could not create shared memory block:\n" <<
      std::strerror(errno) << '\n';
    return -3;
  }
  int sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0660);
  if (sem_id < 0) {
    std::cerr << "Could not create memory guarding semaphore:\n"
      << std::strerror(errno) << '\n';
    return -4;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "Could not attach newly created memory block:\n"
      << std::strerror(errno) << '\n';
    return -5;
  }
  res = semctl(sem_id, 0, SETVAL, 0);
  if (res < 0) {
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "Could not initialize memory guarding semaphore:\n"
      << std::strerror(errno) << '\n';
    return -6;
  }
  // We also need some hack here, because if we lower semaophore when we exit
  // and someone other if on it, we upper it when we quit, because of SEM_UNDO.
  // We need to mark this semaphore as if it was increased by us. xd
  struct sembuf smbf[2];
  smbf[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
  smbf[0].sem_num = 0;
  smbf[0].sem_op = 1;
  smbf[1].sem_flg = IPC_NOWAIT;
  smbf[1].sem_num = 0;
  smbf[1].sem_op = -1;
  res = semop(sem_id, &smbf[0], 2);
  // End of hack.
  if (res < 0) {
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "Could not set memory guarding semaphore:\n"
      << std::strerror(errno) << '\n';
    return -7;
  }
  state_ = CREATED;
  shm_id_ = shm_id;
  sem_id_ = sem_id;
  address_ = mem;
  key_ = shm_key;
  struct shmid_ds shmds;
  res = shmctl(shm_id, IPC_STAT, &shmds);
    if (res < 0) {
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "Probably the memory block was created badly and the size "
      "cannot be read:\n" << std::strerror(errno) << '\n';
    return -8;
  }
  size_ = shmds.shm_segsz;
  return 0;
}

int MemoryChunk::AttachNotNew_(const char *path) {
  assert(state_ == BLANK);

  key_t shm_key = ftok(path, SHM_PROJ_ID);
  if (shm_key < -1) {
    std::cerr << "Could not get shm_key:\n" << std::strerror(errno) << '\n';
    return -1;
  }
  key_t sem_key = ftok(path, SEM_PROJ_ID);
  if (sem_key < -1) {
    std::cerr << "Could not get sem_key:\n" << std::strerror(errno) << '\n';
    return -2;
  }
  int shm_id = shmget(shm_key, 0, 0);
  if (shm_id < 0) {
    std::cerr << "Could not open shared memory block:\n" <<
      std::strerror(errno) << '\n';
    return -3;
  }
  int sem_id = semget(sem_key, 0, 0);
  if (sem_id < 0) {
    std::cerr << "Could not open memory guarding semaphore:\n"
      << std::strerror(errno) << '\n';
    return -4;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "Could not attach memory block:\n"
      << std::strerror(errno) << '\n';
    return -5;
  }
  struct sembuf smbf;
  smbf.sem_flg = SEM_UNDO | IPC_NOWAIT;
  smbf.sem_num = 0;
  smbf.sem_op = 1;
  int semop_ret = semop(sem_id, &smbf, 1);
  if (semop_ret < 0) {
    std::cerr << "Could not change value of memory guarding semaphore:\n"
      << std::strerror(errno) << '\n';
    return -7;
  }
  state_ = GIVEN;
  shm_id_ = shm_id;
  sem_id_ = sem_id;
  address_ = mem;
  key_ = shm_key;
  struct shmid_ds shmds;
  int res = shmctl(shm_id, IPC_STAT, &shmds);
    if (res < 0) {
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, nullptr);
    std::cerr << "The size of memory block cannot be read:\n"
      << std::strerror(errno) << '\n';
    return -8;
  }
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
  int res = shmdt(address_);
  if (res < 0) {
    std::cerr << "Could not detach memory block:\n" <<
      std::strerror(errno) << '\n';
  }
  int semop_ret = semop(sem_id_, &smbf, 1);
  if (semop_ret < 0) {
    if (errno == EAGAIN) {
      // If we are here, it is not bad. It means that we are last detaching
      // from this memory, and we shall close it.
      res = 0;
      res = semctl(sem_id_, 0, IPC_RMID);
      if (res < 0) {
        std::cerr << "Could not set guarding semaphore to remove:\n" <<
          std::strerror(errno) << '\n';
      }
      res = shmctl(shm_id_, IPC_RMID, nullptr);
      if (res < 0) {
        std::cerr << "Could not set memory block to remove:\n" <<
          std::strerror(errno);
      }
    } else {
      std::cerr << "Error in decreasing memory guarding semaphore:\n" <<
        std::strerror(errno) << '\n';
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
                  // It is intentionally caused segfault.
}

}  // namespace uxp
