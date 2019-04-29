// Copyright 2019 Ktoś

#include <unistd.h>
#include <iostream>

static int fn(int a) {
  return a + 6;
}

int main(int argc, char **argv, char **env) {
  int x = 10;
  // Tutaj jakiś komentarz xd
  for (int i = 0; i < 100; ++i) {
    x -= fn(x);
  }
  std::cout << "x = " << x << '\n';  // albo std::endl ale nie wiem
  return 0;
}