// Copyright 2019 UXP1A Students Team

#ifndef SRC_TUPLE_H_
#define SRC_TUPLE_H_

#include <cstdint>
#include <cstdlib>
#include <string>
#include <cstring>
#include "src/element.h"

namespace uxp {

using Type = ElemType;

constexpr size_t MAX_TUPLE_SIZE = 100;

struct Tuple {
  size_t size;
  Element elements[MAX_TUPLE_SIZE];
};

struct TupleDesc {
  size_t size;
  ElementDesc elements[MAX_TUPLE_SIZE];
};

Tuple TupleFromString(const std::string &);
TupleDesc TupleDescFromString(const std::string &);
void TupleGen_(const std::string &, bool desc, void *dest);
bool ChkTuple(const Tuple *, const TupleDesc *);

}  // namespace uxp

#endif  // SRC_TUPLE_H_