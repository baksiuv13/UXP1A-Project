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

using uxp::Linda;
using uxp::ElemType;
using uxp::Element;
using uxp::ElementDesc;
using uxp::Tuple;
using uxp::TupleDesc;
using Condition = ElementDesc::Condition;

int jeden() {
  Linda linda;
  linda.AttachNew(filename);
  if (!linda.IsOpen()) {
    std::cerr << "jeden: Nie otwarło nam pamięci :<, coś jest  źle :<<\n";
    std::cerr << strerror(errno) << '\n';
    return -10;
  }
  Tuple t;
  t.size = 3;
  t.elements[0] = Element(5);
  t.elements[1] = Element("haha");
  t.elements[2] = Element(9.6f);
  linda.Output(t);
  std::cerr << "jeden: wpisane już\n";
  int xd;
  wait(&xd);
  return 0;
}

int dwa() {
  sleep(5);
  std::cerr << "dwa: ok, spróbuję to przeczytać\n";
  Linda linda;
  linda.Attach(filename);
  if (!linda.IsOpen()) {
    std::cerr << "dwa: Nie otwarło nam pamięci :<, coś jest  źle :<<\n";
    std::cerr << strerror(errno) << '\n';
    return -667;
  }
  TupleDesc t;
  t.size = 3;
  t.elements[0] = ElementDesc(5, Condition::EQUAL);
  t.elements[1] = ElementDesc("hehehe", Condition::LESS_EQ);
  t.elements[2] = ElementDesc(10.0f, Condition::LESS);
  std::cerr << "wpisane, szukanko\n";
  Tuple tuple = linda.Input(t, 0);
  std::cerr << "Ok, coś mam " << tuple.size << "\n";
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

  return inne(argc, argv, env);

  return 0;
}
