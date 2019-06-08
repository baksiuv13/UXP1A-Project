// Copyright 2019 UXP1A Students Team

#ifndef SRC_MEMORY_CHUNK_H_
#define SRC_MEMORY_CHUNK_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <cstring>

namespace uxp {
class MemoryChunk {
 public:
  enum State {
    // Memory is not attached.
    BLANK = 0,
    // Memory is created by this object.
    CREATED = 1,
    // Memory is created by other object.
    GIVEN = 2,
  };

  MemoryChunk()
      : state_(State::BLANK),
        shm_id_(0),
        sem_id_(0),
        key_(0),
        address_(nullptr),
        size_(0) {}

  // Link existing memory.
  explicit MemoryChunk(const char *path) : MemoryChunk() {
    AttachNotNew_(path);
  }

  // Create new memory.
  explicit MemoryChunk(const char *path, size_t size) : MemoryChunk() {
    CreateNewMem_(path, size);
  }

  MemoryChunk(const MemoryChunk &) = delete;
  MemoryChunk &operator=(const MemoryChunk &) = delete;

  MemoryChunk(MemoryChunk &&);
  MemoryChunk &operator=(MemoryChunk &&);

  ~MemoryChunk() { Detach(); }

  bool IsOpen() const { return state_ != BLANK; }
  void *GetMem() { return IsOpen() ? address_ : nullptr; }
  size_t GetSize() const { return IsOpen() ? size_ : 0; }
  void ZeroMemory() {
    if (IsOpen()) memset(address_, '\0', size_);
  }

  int Attach(const char *path);
  int AttachNew(const char *path, size_t size);
  void Detach();
  char &operator[](size_t);

 private:
  static constexpr int SHM_PROJ_ID = 1;
  static constexpr int SEM_PROJ_ID = 2;

  static void ERROR_PRINT(const char *msg);

  int CreateNewMem_(const char *path, size_t size);
  int AttachNotNew_(const char *path);
  int UseSysSemaphores_(const char *path, int number, int flag);
  int UseSysShmMemory_(const char *path, size_t size, int flag);
  int ReadShmSize();
  void Close_();

  State state_;
  int shm_id_;
  int sem_id_;
  key_t key_;
  void *address_;
  size_t size_;
};
}  // namespace uxp

#endif  // SRC_MEMORY_CHUNK_H_
