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

using uxp::Linda;
using uxp::ElemType;
using Elem = Linda::Element;
using Tuple = Linda::Tuple;
using TupleDesc = Linda::TupleDesc;
using Condition = Linda::ElementDesc::Condidtion;

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
  t.elements[0].type = ElemType::INT;
  t.elements[0].value.int_ = 5;
  t.elements[1].type = ElemType::STRING;
  t.elements[1].str_size = 4;
  memcpy(t.elements[1].value.string_, "haha", 5);
  t.elements[2].type = ElemType::FLOAT;
  t.elements[2].value.float_ = 9.6;
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
  t.elements[0].type = ElemType::INT;
  t.elements[0].value.int_ = 5;
  t.elements[0].condition = Condition::EQUAL;
  t.elements[1].type = ElemType::STRING;
  t.elements[1].str_size = 6;
  t.elements[1].condition = Condition::LESS_EQ;
  memcpy(t.elements[1].value.string_, "hehehe", 7);
  t.elements[2].type = ElemType::FLOAT;
  t.elements[2].value.float_ = 10.0;
  t.elements[2].condition = Condition::LESS;
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
