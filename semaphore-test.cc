// Copyright 2019 UXP1A Students Team
#include <iostream>
#include "src/semaphore.h"

int main(int argc, char **argv, char **env) {
  std::cout << "Hello world!" << std::endl;

  uxp::Semaphore sem = uxp::Semaphore(0);

  //  play
  sem.initialize(2);

  std::cout << "2?: " << sem.getValue() << std::endl;

  sem.P();
  std::cout << "1?: " << sem.getValue() << std::endl;

  sem.P();
  std::cout << "0?: " << sem.getValue() << std::endl;

  sem.V();
  std::cout << "1?: " << sem.getValue() << std::endl;

  std::cout << "False?: " << sem.isZero() << std::endl;

  sem.P();
  std::cout << "0?: " << sem.getValue() << std::endl;

  std::cout << "True?: " << sem.isZero() << std::endl;

  std::cout << "Close: True?: " << uxp::Semaphore::closeSemTable() << std::endl;

  // ~play

  return 0;
}
