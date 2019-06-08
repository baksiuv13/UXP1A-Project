// Copyright 2019 UXP1A Students Team
#include <iostream>
#include "src/semaphore.h"

int main(int argc, char **argv, char **env) {
  std::cout << "Hello world!\n";

  uxp::Semaphore sem = uxp::Semaphore(0);

  //  play
  sem.initialize(2);

  std::cout << "2?: " << sem.getValue() << '\n';

  sem.P();
  std::cout << "1?: " << sem.getValue() << '\n';

  sem.P();
  std::cout << "0?: " << sem.getValue() << '\n';

  sem.V();
  std::cout << "1?: " << sem.getValue() << '\n';

  std::cout << "False?: " << sem.isZero() << '\n';

  sem.P();
  std::cout << "0?: " << sem.getValue() << '\n';

  std::cout << "True?: " << sem.isZero() << '\n';

  std::cout << "Close: True?: " << uxp::Semaphore::closeSemTable() << '\n';

  // ~play

  return 0;
}
