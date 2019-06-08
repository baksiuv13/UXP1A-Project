// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <cstring>
#include <iostream>

#include "src/memory_chunk.h"

static int fn(int a) {
  return a + 6;
}

int main(int argc, char **argv, char **env) {
  int x = 10;
  // Tutaj jakiś komentarz xd
  for (int i = 0; i < 100; ++i) {
    x -= fn(x);
  }
  std::cout << "x = " << x << '\n';

  std::cerr << getpid() << '\n';

  // uxp::MemoryChunk mc;


  return 0;
}
