// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "src/linda.h"

static constexpr const char *filename = "./tests-shmem";

void first() {
  uxp::Linda linda(filename);
  uxp::Tuple tuple{3,
                   {uxp::Element(3.0f), uxp::Element(1), uxp::Element("aaa")}};
  assert(linda.Output(tuple) == true);

  int res;
  wait(&res);
}

void second() {
  uxp::Linda linda(filename);
  uxp::TupleDesc pattern{
      3,
      {uxp::ElementDesc(1.0f, uxp::ElementDesc::Condition::GREATER_EQ),
       uxp::ElementDesc(1, uxp::ElementDesc::Condition::ANY),
       uxp::ElementDesc("aaa", uxp::ElementDesc::EQUAL)}};

  int second_10 = 10 * 1000;  // 10 sec in ms
  auto read = linda.Read(pattern, second_10);
  assert(read.size == 3);
  std::cout << read.ToString() << "\n";

  auto input = linda.Input(pattern, second_10);
  assert(input.size == 3);
  std::cout << input.ToString() << "\n";

  auto notfound = linda.Input(pattern, 1000);
  assert(notfound.size == 0);
  std::cout << notfound.ToString() << "\n";
}

int main(int argc, char **argv, char **env) {
  int forked = fork();
  if (forked < 0) {
    return -1;
  } else if (forked > 0) {
    first();
  } else {
    second();
  }
  return 0;
}
