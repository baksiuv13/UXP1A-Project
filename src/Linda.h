// Copyright 2019 UXP1A Students Team

#ifndef SRC_LINDA_H_
#define SRC_LINDA_H_

#include <cstdint>
#include <cstdlib>
#include <string>
#include <cstring>

#include "src/memory_chunk.h"
#include "src/type.h"

namespace uxp {

class Linda {
 public:
  static constexpr size_t MAX_TUPLE_SIZE = 100;
  static constexpr size_t TUPLES_NUMBER = 100;
  static constexpr size_t MAX_STRING_SIZE = 200;

  using Type = ElemType;

  static constexpr struct CREATE_t {
  } CREATE{};

  struct Element {
    Type type;
    size_t str_size;
    union {
      int32_t int_;
      float float_;
      char string_[MAX_STRING_SIZE];
    } value;
  };

  struct ElementDesc {
    Type type;
    enum Condidtion : int16_t {
      ANY,         // *
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
      char string_[MAX_STRING_SIZE];
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

  // Create empty object.
  Linda() : mc_() {}

  // Link existing memory.
  explicit Linda(const char *path) : mc_(path) {}

  // Create new memory.
  explicit Linda(const char *path, CREATE_t) : mc_(path, MEM_SIZE) {
    if (IsOpen()) {
      memset(mc_.GetMem(), '\0', mc_.GetSize());
    }
  }

  int Attach(const char *path) { return mc_.Attach(path); }
  int AttachNew(const char *path) {
    int result = mc_.AttachNew(path, MEM_SIZE);
    if (result >= 0) {
      memset(mc_.GetMem(), '\0', mc_.GetSize());
      return 0;
    }
    return result;
  }
  void Detach() { return mc_.Detach(); }

  bool IsOpen() { return mc_.IsOpen(); }

  void Output(Tuple tuple);
  Tuple Input(TupleDesc describe, unsigned int timeout_ms);
  Tuple Read(TupleDesc describe, unsigned int timeout_ms);

  static Tuple TupleFromString(const std::string &);
  static TupleDesc TupleDescFromString(const std::string &);

  static bool ChkTuple(const Tuple *, const TupleDesc *);
  static bool ChkElem(const Element *, const ElementDesc *);

 private:
  static constexpr size_t TUPLES_START = 0;
  static constexpr size_t MEM_SIZE =
      TUPLES_START + TUPLES_NUMBER * sizeof(Tuple);
  static void ZeroTuple_(Tuple *);
  static void TupleGen_(const std::string &, bool desc, void *dest);

  Tuple &TupleAt_(size_t n) {
    return reinterpret_cast<Tuple *>(&mc_[TUPLES_START])[n];
  }
  Tuple *Find_(const TupleDesc &describe);

  MemoryChunk mc_;
};

}  // namespace uxp

#endif  // SRC_LINDA_H_
