// Copyright 2019 UXP1A Students Team

#ifndef SRC_LINDA_H_
#define SRC_LINDA_H_

#include <cstdint>
#include <cstdlib>

#include "src/memory_chunk.h"

namespace uxp {

class Linda {
 public:
  static constexpr size_t MAX_TUPLE_SIZE = 100;
  static constexpr size_t TUPLES_NUMBER = 100;
  static constexpr size_t MAX_STRING_SIZE = 200;

  static constexpr struct CREATE_t {} CREATE {};


  enum Type : int16_t { INT, FLOAT, STRING };

  struct Element {
    Type type;
    size_t str_size;
    union {
      int32_t int_;
      float float_;
      char string_start_[MAX_STRING_SIZE];
    } value;
  };

  struct ElementDesc {
    Type type;
    enum Condidtion : int16_t {
      LESS,        // <
      GREATER,     // >
      LESS_EQ,     // <=
      GREATER_EQ,  // >=
      EQUAL        // ==
    } condition;
    size_t str_size;
    union {
      int32_t int_;
      float float_;
      char string_start_[MAX_STRING_SIZE];
    } value;
  };

  struct Tuple {
    size_t size;
    struct Element elements[MAX_TUPLE_SIZE];
  };

  struct TupleDesc {
    size_t size;
    struct ElementDesc elements[MAX_TUPLE_SIZE];
  };

  static constexpr size_t MEM_SIZE = TUPLES_NUMBER * sizeof(Tuple);

  // Create empty object.
  Linda();

  // Link existing memory.
  explicit Linda(const char *path) : mc_(path) {}

  // Create new memory.
  explicit Linda(const char *path, CREATE_t) : mc_(path, MEM_SIZE) {}


  int Attach(const char *path) {mc_.Attach(path);}
  int AttachNew(const char *path) {mc_.AttachNew(path, MEM_SIZE);}
  void Detach() {mc_.Detach();}

  bool IsOpen() {return mc_.IsOpen();}

  void Output(Tuple tuple);
  Tuple Input(TupleDesc describe, unsigned int timeout_ms);
  Tuple Read(TupleDesc describe, unsigned int timeout_ms);

 private:
  MemoryChunk mc_;
};

}  // namespace uxp


#endif  // SRC_LINDA_H_
