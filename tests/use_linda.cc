// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "src/linda.h"

static constexpr const char *filename = "./tests-shmem";

int first() {
  uxp::Linda linda(filename, uxp::Linda::CREATE);
  uxp::Tuple tuple{3,
                   {uxp::Element(3.0f), uxp::Element(1), uxp::Element("aaa")}};
  assert(linda.Output(tuple) == true);

  int res;
  wait(&res);
  return 0;
}

int second() {
  sleep(5);  // time for first() to create memory
  uxp::Linda linda(filename);
  uxp::TupleDesc pattern{
      3,
      {uxp::ElementDesc(1.0f, uxp::ElementDesc::Condition::GREATER_EQ),
       uxp::ElementDesc(1, uxp::ElementDesc::Condition::ANY),
       uxp::ElementDesc("aaa", uxp::ElementDesc::EQUAL)}};

  int second_10 = 10 * 1000;  // 10 sec in ms
  auto read = linda.Read(pattern, second_10);
  assert(read.size == 3);
  auto input = linda.Input(pattern, second_10);
  assert(input.size == 3);
  auto notfound = linda.Input(pattern, 1000);
  assert(notfound.size == 0);
  return 0;
}

int main(int argc, char **argv, char **env) {
  int forked = fork();
  if (forked < 0) {
    return -1;
  } else if (forked > 0) {
    return first();
  } else {
    return second();
  }
}
