// Copyright 2019 UXP1A Students Team

#include "src/memory_chunk.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace uxp {

namespace {

static constexpr int create_mem_flag = IPC_CREAT | IPC_EXCL | 0660;
static constexpr int open_existing_mem_flag = 0660;

static std::string EXCEPTION_MSG(const char *msg) {
  int e = errno;
  errno = 0;
  return std::string(msg) + ":\n\t" + std::strerror(e) + '\n';
}

static void ERROR_PRINT(const char *msg) {
  std::cerr << EXCEPTION_MSG(msg);
  errno = 0;
}

template <typename T>
static void Throw(const char *msg) {
  throw T(EXCEPTION_MSG(msg));
}

}  // namespace

void MemoryChunk::AttachMem_(const char *path, size_t size) {
  if (size <= 0) throw std::invalid_argument("Size cannot be 0");
  key_ = ftok(path, SHM_PROJ_ID);
  if (key_ < 0) Throw<std::runtime_error>("Could not get shm_key");
  shm_id_ = shmget(key_, 0, open_existing_mem_flag);
  if (shm_id_ < 0 && errno == ENOENT) {
    shm_id_ = shmget(key_, size, create_mem_flag);
    newly_created_ = true && shm_id_ >= 0;
  }
  if (shm_id_ < 0) {
    Throw<std::runtime_error>("Could not create or attach shared memory block");
  }
  struct shmid_ds shmds;
  int res = shmctl(shm_id_, IPC_STAT, &shmds);
  if (res < 0) {
    Throw<std::runtime_error>("Could not read info about memory block");
  }
  size_ = shmds.shm_segsz;
  address_ = shmat(shm_id_, nullptr, 0);
  if (address_ == reinterpret_cast<void *>(-1)) {
    if (newly_created_) shmctl(shm_id_, IPC_RMID, nullptr);
    Throw<std::runtime_error>("Could not attach memory block");
  }
}

void MemoryChunk::CloseMem_() {
  int dt_res = shmdt(address_);
  if (dt_res < 0) {
    ERROR_PRINT("Could not detach memory block");
  }
  struct shmid_ds shmds;
  int stat_res = shmctl(shm_id_, IPC_STAT, &shmds);
  if (stat_res < 0) {
    ERROR_PRINT("Could not read info about memory block");
  }
  int rem_res = 0;
  bool remove = shmds.shm_nattch == 0;
  if (remove) {
    rem_res = shmctl(shm_id_, IPC_RMID, nullptr);
    if (rem_res < 0) ERROR_PRINT("Could not remove memory block");
  }
}

char &MemoryChunk::at(size_t i) {
  if (i > size_) {
    throw std::out_of_range("Too large argument.");
  }
  return static_cast<char *>(GetMem())[i];
}

}  // namespace uxp
