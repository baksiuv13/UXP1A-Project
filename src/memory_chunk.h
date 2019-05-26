// Copyright 2019 UXP1A Students Team

#ifndef SRC_MEMORY_CHUNK_H_
#define SRC_MEMORY_CHUNK_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

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
  // static constexpr struct CREATE_t {} CREATE {};

  // static constexpr size_t MEMORY_SIZE = 64 * 1024;

  MemoryChunk();

  // Link existing memory.
  explicit MemoryChunk(const char *path);

  // Create new memory.
  explicit MemoryChunk(const char *path, size_t size);

  MemoryChunk(const MemoryChunk &) = delete;
  MemoryChunk &operator=(const MemoryChunk &) = delete;

  MemoryChunk(MemoryChunk &&);
  MemoryChunk &operator=(MemoryChunk &&);

  ~MemoryChunk();

  constexpr bool IsOpen() const { return state_ != BLANK; }
  void *GetMem() { return IsOpen() ? address_ : nullptr; }
  // int GetMemId() {return IsOpen() ? shm_id_ : 0;}
  constexpr size_t GetSize() const { return IsOpen() ? size_ : 0; }
  int Attach(const char *path);
  int AttachNew(const char *path, size_t size);
  void Detach();
  char &operator[](size_t);

 private:
  static constexpr int SHM_PROJ_ID = 1;
  static constexpr int SEM_PROJ_ID = 2;

  int CreateNewMem_(const char *path, size_t size);
  int AttachNotNew_(const char *path);
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
