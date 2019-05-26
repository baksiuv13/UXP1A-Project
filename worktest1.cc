// Copyright 2019 UXP1A Students Team

#include <unistd.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "src/memory_chunk.h"

static constexpr const char *filename = "./shm-test-file";

int jeden() {
  uxp::MemoryChunk mc;
  mc.AttachNew(filename, 100);
  if (!mc.IsOpen()) {
    std::cerr << "jeden: Nie otwarło nam pamięci :<, coś jest  źle :<<\n";
    std::cerr << strerror(errno) << '\n';
    return -666;
  }
  memset(mc.GetMem(), '6', 99);
  mc[99] = '\0';
  std::cerr << "jeden: wpisane już\n";
  int xd;
  wait(&xd);
  return 0;
}

int dwa() {
  sleep(5);
  std::cerr << "dwa: ok, spróbuję to przeczytać\n";
  uxp::MemoryChunk mc;
  mc.Attach(filename);
  if (!mc.IsOpen()) {
    std::cerr << "dwa: Nie otwarło nam pamięci :<, coś jest  źle :<<\n";
    std::cerr << strerror(errno) << '\n';
    return -667;
  }
  std::cerr << &mc[0] << '\n';
  return 0;
}

int inne(int argc, char **argv, char **env) {
  int fret = fork();
  if (fret < 0) {
    return -19382;
  } else if (fret > 0) {
    int jabko = jeden();
    return jabko;
  } else {
    exit(dwa());
  }
}

int main(int argc, char **argv, char **env) {
  std::cerr << getpid() << '\n';

  if (1 || argc > 1) {
    return inne(argc, argv, env);
  }

  uxp::MemoryChunk mc;

  mc.AttachNew(filename, 666);

  if (!mc.IsOpen()) {
    std::cerr << "Nie otwarło nam pamięci :<, coś jest  źle :<<\n";
    std::cerr << strerror(errno) << '\n';
    return -666;
  }

  memset(mc.GetMem(), 'x', 665);
  reinterpret_cast<char *>(mc.GetMem())[665] = '\0';

  std::cerr << reinterpret_cast<char *>(mc.GetMem());

  return 0;
}
