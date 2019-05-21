// Copyright 2019 UXP1A Students Team

#ifndef SRC_LINDA_H_
#define SRC_LINDA_H_

#include <cstdint>

class Linda {
 public:
  enum Type : int16_t { INT, FLOAT, STRING };

  struct Element {
    Type type;
    int16_t empty;
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
    union {
      int32_t int_;
      float float_;
      char string_start;
    } value;
  };

  struct Tuple {
    alignas(int32_t) char elements[];  // We expected an array of pointers, yes?
    // Should we have size given here or as a first element od array?
  };

  Linda();
  explicit Linda(int memory_identifier);

  void outpute(Tuple tuple);
  Tuple input(Tuple describe, unsigned int timeout_ms);
  Tuple read(Tuple describe, unsigned int timeout_ms);
};

#endif  // SRC_LINDA_H_
