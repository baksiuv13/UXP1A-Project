// Copyright 2019 UXP1A Students Team

#include <csignal>
#include <cstring>
#include <iostream>
#include <string>

#include "client/lindacli.h"
#include "src/linda.h"

std::atomic_bool stop {false};

void handler(int s) { stop = true; }
void attach_signal_handler() {
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
}

int main(int argc, char **argv, char **env) {
  if (argc < 2) {
    std::cerr << "Need a shared memory file as arg\n";
    return -1;
  }

  uxp::Linda linda;
  if (argc > 2 && strcmp(argv[2], "-c") == 0)
    linda.AttachNew(argv[1]);
  else
    linda.Attach(argv[1]);

  if (linda.IsOpen()) {
    std::cout << "Linda is ready with shm: " << argv[1] << std::endl;
  } else {
    std::cerr << "Unable to open Linda for shm " << argv[1] << std::endl;
    return -1;
  }

  attach_signal_handler();
  client::LindaCli cli(&linda);

  cli.Help();
  cli.Run(&stop);
  return 0;
}
