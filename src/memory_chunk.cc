// Copyright 2019 UXP1A Students Team

#include "src/memory_chunk.h"

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

template <typename T>
static void Throw(const char *msg) {
  throw T(EXCEPTION_MSG(msg));
}

}  // namespace

void MemoryChunk::AttachMem_(const char *path, size_t size) {
  if (size <= 0)
    throw std::invalid_argument("Size cannot be 0");
  int res;
  int shm_id;
  void *mem;
  bool newly_created = false;
  size_t allocated_size;
  key_t shm_key = ftok(path, SHM_PROJ_ID);
  if (shm_key < 0)
    Throw<std::runtime_error>("Could not get shm_key");
  shm_id = shmget(shm_key, 0, open_existing_mem_flag);
  if (shm_id < 0 && errno == ENOENT) {
    shm_id = shmget(shm_key, size, create_mem_flag);
    newly_created = true && shm_id >= 0;
  }
  if (shm_id < 0) {
    Throw<std::runtime_error>
      ("Could not create or attach shared memory block");
  }
  struct shmid_ds shmds;
  res = shmctl(shm_id, IPC_STAT, &shmds);
  if (res < 0) {
    Throw<std::runtime_error>("Could not read info about memory block");
  }
  allocated_size = shmds.shm_segsz;
  mem = shmat(shm_id, nullptr, 0);
  if (mem == reinterpret_cast<void *>(-1)) {
    if (newly_created)
      shmctl(shm_id, IPC_RMID, nullptr);
    Throw<std::runtime_error>("Could not attach memory block");
  }
  
  key_ = shm_key;
  shm_id_ = shm_id;
  address_ = mem;
  size_ = allocated_size;
  newly_created_ = newly_created;
}


void MemoryChunk::CloseMem_() {
  int res = shmdt(address_);
  if (res < 0) {
    Throw<std::runtime_error>("Could not detach memory block");
  }
  struct shmid_ds shmds;
  res = shmctl(shm_id_, IPC_STAT, &shmds);
  if (res < 0) {
    Throw<std::runtime_error>("Could not read info about memory block");
  }
  bool remove = shmds.shm_nattch == 0;
  if (remove) {
    shmctl(shm_id_, IPC_RMID, nullptr);
  }
}

char &MemoryChunk::at(size_t i) {
  if (i > size_) {
    throw std::out_of_range("Too large argument.");
  }
  return static_cast<char *>(GetMem())[i];
}

}  // namespace uxp
