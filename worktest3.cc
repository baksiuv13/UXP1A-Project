// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "src/linda.h"

static constexpr const char *filename = "./shm-test-file";

using uxp::ElemType;
using uxp::Linda;
using uxp::Element;
using uxp::ElementDesc;
using uxp::Tuple;
using uxp::TupleDesc;
using Condition = uxp::ElementDesc::Condition;

int main(int argc, char **argv, char **env) {
  Tuple t;
  t.size = 3;
  t.elements[0] = Element(5);
  t.elements[1] = Element("haha");
  t.elements[2] = Element(9.6f);

  TupleDesc td;
  td.size = 3;
  td.elements[0] = ElementDesc(5, Condition::EQUAL);
  td.elements[1] = ElementDesc("chehehe", Condition::GREATER_EQ);
  td.elements[2] = ElementDesc(10.0f, Condition::LESS);

  std::cerr << uxp::ChkTuple(&t, &td) << '\n';

  return 0;
}
