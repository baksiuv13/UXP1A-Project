// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "src/Linda.h"

static constexpr const char *filename = "./shm-test-file";

using uxp::ElemType;
using uxp::Linda;
using Elem = Linda::Element;
using Tuple = Linda::Tuple;
using TupleDesc = Linda::TupleDesc;
using Condition = Linda::ElementDesc::Condidtion;

int main(int argc, char **argv, char **env) {
  Tuple t;
  t.size = 3;
  t.elements[0].type = ElemType::INT;
  t.elements[0].value.int_ = 5;
  t.elements[1].type = ElemType::STRING;
  t.elements[1].str_size = 4;
  memcpy(t.elements[1].value.string_, "haha", 5);
  t.elements[2].type = ElemType::FLOAT;
  t.elements[2].value.float_ = 9.6;

  TupleDesc td;
  td.size = 3;
  td.elements[0].type = ElemType::INT;
  td.elements[0].value.int_ = 5;
  td.elements[0].condition = Condition::EQUAL;
  td.elements[1].type = ElemType::STRING;
  td.elements[1].str_size = 7;
  td.elements[1].condition = Condition::GREATER_EQ;
  memcpy(td.elements[1].value.string_, "chehehe", 8);
  td.elements[2].type = ElemType::FLOAT;
  td.elements[2].value.float_ = 10.0;
  td.elements[2].condition = Condition::LESS;

  std::cerr << Linda::ChkTuple(&t, &td) << '\n';

  return 0;
}
