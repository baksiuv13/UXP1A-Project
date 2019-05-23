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
    BLANK = 0,
    CREATED = 1,
    GIVEN = 2,
  };
  // static constexpr struct CREATE_t {} CREATE {};

  // static constexpr size_t MEMORY_SIZE = 64 * 1024;

  MemoryChunk();
  explicit MemoryChunk(const char *path);
  explicit MemoryChunk(const char *path, size_t size);

  MemoryChunk(const MemoryChunk&);
  MemoryChunk &operator=(const MemoryChunk&);

  MemoryChunk(MemoryChunk &&);
  MemoryChunk &operator=(MemoryChunk &&);

  ~MemoryChunk();

  bool IsOpen() {return state_ != BLANK;}
  void *GetMem() {return IsOpen() ? address_ : nullptr;}
  // int GetMemId() {return IsOpen() ? shm_id_ : 0;}
  size_t GetSize() {return IsOpen() ? size_ : 0;}
  int Attach(const char *path);
  int AttachNew(const char *path, size_t size);
  void Detach();

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
