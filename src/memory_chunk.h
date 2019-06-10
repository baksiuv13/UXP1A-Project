// Copyright 2019 UXP1A Students Team

#ifndef SRC_MEMORY_CHUNK_H_
#define SRC_MEMORY_CHUNK_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <cstring>
#include <exception>

namespace uxp {
class MemoryChunk {
 public:
  MemoryChunk() = delete;

  // Attach to memory pointed by file.
  explicit MemoryChunk(const char *path, size_t size) :
      shm_id_(0),
      key_(0),
      address_(nullptr),
      size_(0) {
    AttachMem_(path, size);
  }

  MemoryChunk(const MemoryChunk &) = delete;
  MemoryChunk &operator=(const MemoryChunk &) = delete;

  // This class is not movable because it has to be initialized all time.
  MemoryChunk(MemoryChunk &&) = delete;
  MemoryChunk &operator=(MemoryChunk &&) = delete;

  ~MemoryChunk() { CloseMem_(); }

  void *GetMem() { return address_; }
  size_t GetSize() const { return size_; }
  void ZeroMemory() { memset(address_, '\0', size_); }
  bool NewlyCreated() { return newly_created_; }

  // Get i byte of shared memory.
  char &at(size_t i);

 private:
  static constexpr int SHM_PROJ_ID = 1;

  void AttachMem_(const char *path, size_t size);
  void CloseMem_();

  int shm_id_;
  key_t key_;
  void *address_;
  size_t size_;
  bool newly_created_;
};
}  // namespace uxp

#endif  // SRC_MEMORY_CHUNK_H_
