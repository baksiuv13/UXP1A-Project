// Copyright 2019 UXP1A Students Team

#ifndef SRC_LINDA_H_
#define SRC_LINDA_H_

#include <cstdint>
#include <cstdlib>

namespace uxp {

class Linda {
 public:
  static const size_t MAX_TUPLE_SIZE = 100;

  enum Type : int16_t { INT, FLOAT, STRING };

  struct Element {
    Type type;
    size_t str_size;
    union {
      int32_t int_;
      float float_;
      char string_start;
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
    };
    size_t str_size;
    union {
      int32_t int_;
      float float_;
      char string_start;
    } value;
  };

  struct Tuple {
    size_t size;
    struct Element* elements[MAX_TUPLE_SIZE];
  };

  struct TupleDesc {
    size_t size;
    struct ElementDesc* elements[MAX_TUPLE_SIZE];
  };

  Linda();
  explicit Linda(int memory_identifier);

  void Output(Tuple tuple);
  Tuple Input(TupleDesc describe, unsigned int timeout_ms);
  Tuple Read(TupleDesc describe, unsigned int timeout_ms);
};

}  // namespace uxp

#endif  // SRC_LINDA_H_
