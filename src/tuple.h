// Copyright 2019 UXP1A Students Team

#ifndef SRC_TUPLE_H_
#define SRC_TUPLE_H_

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "src/element.h"

namespace uxp {

using Type = ElemType;

constexpr size_t MAX_TUPLE_SIZE = 100;

struct Tuple {
  size_t size;
  Element elements[MAX_TUPLE_SIZE];
  std::string ToString();
};

struct TupleDesc {
  size_t size;
  ElementDesc elements[MAX_TUPLE_SIZE];
};

bool CheckTuple(const Tuple *, const TupleDesc *);

}  // namespace uxp

#endif  // SRC_TUPLE_H_
