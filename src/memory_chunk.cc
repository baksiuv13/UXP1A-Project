// Copyright 2019 UXP1A Students Team

#include "src/memory_chunk.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace uxp {

void MemoryChunk::ERROR_PRINT(const char *msg) {
  std::cerr << msg << ":\n\t" << std::strerror(errno) << '\n';
}

int MemoryChunk::UseSysSemaphores_(const char *path, int number, int flag) {
  key_t sem_key = ftok(path, SEM_PROJ_ID);
  if (sem_key < -1) {
    ERROR_PRINT("Could not get sem_key");
    return -2;
  }
  int sem_id = semget(sem_key, number, flag);
  if (sem_id < 0) {
    ERROR_PRINT("Could not create or attach memory guarding semaphore");
    return -4;
  }
  sem_id_ = sem_id;
  return 0;
}

int MemoryChunk::UseSysShmMemory_(const char *path, size_t size, int flag) {
  key_t shm_key = ftok(path, SHM_PROJ_ID);
  if (shm_key < -1) {
    ERROR_PRINT("Could not get shm_key");
    return -1;
  }
  int shm_id = shmget(shm_key, size, flag);
  if (shm_id < 0) {
    ERROR_PRINT("Could not create or attach shared memory block");
    return -3;
  }
  void *mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    shmctl(shm_id, IPC_RMID, nullptr);
    ERROR_PRINT("Could not attach memory block");
    return -5;
  }
  key_ = shm_key;
  shm_id_ = shm_id;
  address_ = mem;
  return 0;
}

int MemoryChunk::ReadShmSize() {
  struct shmid_ds shmds;
  int res = shmctl(shm_id_, IPC_STAT, &shmds);
  if (res < 0) {
    ERROR_PRINT("The size of memory block cannot be read");
    return -8;
  }
  size_ = shmds.shm_segsz;
  return 0;
}

int MemoryChunk::CreateNewMem_(const char *path, size_t size) {
  assert(state_ == BLANK);
  int res;

  int new_sem_flag = IPC_CREAT | IPC_EXCL | 0660;
  if ((res = UseSysSemaphores_(path, 1, new_sem_flag)) < 0) return res;
  int new_mem_flag = IPC_CREAT | IPC_EXCL | 0660;
  if ((res = UseSysShmMemory_(path, size, new_mem_flag)) < 0) return res;

  res = semctl(sem_id_, 0, SETVAL, 0);
  if (res < 0) {
    semctl(sem_id_, 0, IPC_RMID);
    shmctl(shm_id_, IPC_RMID, nullptr);
    ERROR_PRINT("Could not initialize memory guarding semaphore");
    return -6;
  }

  // We also need some hack here, because if we lower semaphore when we exit
  // and someone other if on it, we upper it when we quit, because of SEM_UNDO.
  // We need to mark this semaphore as if it was increased by us. xd
  struct sembuf smbf[2];
  smbf[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
  smbf[0].sem_num = 0;
  smbf[0].sem_op = 1;
  smbf[1].sem_flg = IPC_NOWAIT;
  smbf[1].sem_num = 0;
  smbf[1].sem_op = -1;
  res = semop(sem_id_, &smbf[0], 2);
  // End of hack.
  if (res < 0) {
    semctl(sem_id_, 0, IPC_RMID);
    shmctl(shm_id_, IPC_RMID, nullptr);
    ERROR_PRINT("Could not set memory guarding semaphore");
    return -7;
  }

  state_ = CREATED;
  res = ReadShmSize();

  if (res < 0) {
    semctl(sem_id_, 0, IPC_RMID);
    shmctl(shm_id_, IPC_RMID, nullptr);
    return res;
  }
  return 0;
}

int MemoryChunk::AttachNotNew_(const char *path) {
  assert(state_ == BLANK);
  int res;

  if ((res = UseSysSemaphores_(path, 0, 0)) < 0) return res;
  if ((res = UseSysShmMemory_(path, 0, 0)) < 0) return res;

  struct sembuf smbf;
  smbf.sem_flg = SEM_UNDO | IPC_NOWAIT;
  smbf.sem_num = 0;
  smbf.sem_op = 1;
  int semop_ret = semop(sem_id_, &smbf, 1);
  if (semop_ret < 0) {
    ERROR_PRINT("Could not change value of memory guarding semaphore");
    return -7;
  }

  state_ = GIVEN;
  res = ReadShmSize();

  if (res < 0) {
    semctl(sem_id_, 0, IPC_RMID);
    shmctl(shm_id_, IPC_RMID, nullptr);
    return res;
  }
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
  if (res < 0) ERROR_PRINT("Could not detach memory block");

  int semop_ret = semop(sem_id_, &smbf, 1);
  if (semop_ret < 0) {
    if (errno == EAGAIN) {
      // If we are here, it is not bad. It means that we are last detaching
      // from this memory, and we shall close it.
      res = semctl(sem_id_, 0, IPC_RMID);
      if (res < 0) ERROR_PRINT("Could not set guarding semaphore to remove");
      res = shmctl(shm_id_, IPC_RMID, nullptr);
      if (res < 0) ERROR_PRINT("Could not set memory block to remove");
    } else {
      ERROR_PRINT("Error in decreasing memory guarding semaphore");
    }
  }
  state_ = BLANK;
}

void MemoryChunk::Detach() {
  if (IsOpen()) Close_();
}

char &MemoryChunk::operator[](size_t i) {
  return IsOpen() ? static_cast<char *>(GetMem())[i]
                  : *static_cast<char *>(nullptr);
  // It is intentionally caused segfault.
}

}  // namespace uxp
