// Copyright 2019 UXP1A Students Team
#include <iostream>
#include "src/semaphore.h"

int main(int argc, char **argv, char **env) {
  std::cout << "Hello world!" << std::endl;

  uxp::semaphore::Semaphore sem = uxp::semaphore::Semaphore(0);

  //  play
  sem.initialize(2);

  sem.P();
  sem.P();
  sem.V();
  sem.P();

  // ~play

  return 0;
}
