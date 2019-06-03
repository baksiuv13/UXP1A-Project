// Copyright 2019 UXP1A Students Team

#include <csignal>
#include <cstring>
#include <iostream>
#include <string>

#include "src/linda.h"

bool run = true;
uxp::Linda linda;

void handler(int s) { run = false; }
void attach_signal_handler() {
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
}

uxp::Element parse_element() {
  std::string type;
  std::cin >> type;
  if (type == "i") {
    int val;
    std::cin >> val;
    return uxp::Element(val);
  } else if (type == "f") {
    float val;
    std::cin >> val;
    return uxp::Element(val);
  } else if (type == "s") {
    std::string val;
    std::cin >> val;
    return uxp::Element(val.c_str());
  } else {
    std::cout << "Element type '" + type + " is invalid" << std::endl;
    throw std::runtime_error("Invalid type");
  }
}

uxp::Tuple parse_tuple(size_t size) {
  uxp::Tuple tuple;
  tuple.size = size;
  for (size_t i = 0; i < size; ++i) {
    tuple.elements[i] = parse_element();
  }
  return tuple;
}

uxp::ElementDesc::Condition get_condition(std::string cmp) {
  if (cmp == "*")
    return uxp::ElementDesc::Condition::ANY;
  else if (cmp == "<")
    return uxp::ElementDesc::Condition::LESS;
  else if (cmp == ">")
    return uxp::ElementDesc::Condition::GREATER;
  else if (cmp == "<=")
    return uxp::ElementDesc::Condition::LESS_EQ;
  else if (cmp == ">=")
    return uxp::ElementDesc::Condition::GREATER_EQ;
  else if (cmp == "==")
    return uxp::ElementDesc::Condition::EQUAL;
  throw std::runtime_error("Invalid condition");
}

template <typename T>
T get_when_need(uxp::ElementDesc::Condition cmp) {
  T val;
  if (cmp == uxp::ElementDesc::ANY) return val;
  std::cin >> val;
  return val;
}

uxp::ElementDesc parse_element_desc() {
  std::string type, cond;
  std::cin >> type >> cond;
  auto condition = get_condition(cond);
  if (type == "i") {
    return uxp::ElementDesc(get_when_need<int32_t>(condition), condition);
  } else if (type == "f") {
    return uxp::ElementDesc(get_when_need<float>(condition), condition);
  } else if (type == "s") {
    return uxp::ElementDesc(get_when_need<std::string>(condition).c_str(),
                            condition);
  } else {
    std::cout << "Element type '" + type + " is invalid" << std::endl;
    throw std::runtime_error("Invalid type");
  }
}

uxp::TupleDesc parse_tuple_desc(size_t size) {
  uxp::TupleDesc tuple;
  tuple.size = size;
  for (size_t i = 0; i < size; ++i) {
    tuple.elements[i] = parse_element_desc();
  }
  return tuple;
}

void help() {
  std::cout << "Possible commands: " << std::endl;
  std::cout << "* help - see this" << std::endl;
  std::cout << "* output <size:int> {<type:i|f|s> <value:int|float|string>}"
            << std::endl;
  std::cout << "* input <size:int> <timeout_sec:int> {<type:i|f|s> "
               "<cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}"
            << std::endl;
  std::cout << "* read <size:int> <timeout_sec:int> {<type:i|f|s> "
               "<cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}"
            << std::endl;
  std::cout
      << std::endl
      << "When you declare tuple pattern, don't set value for *. Examples:"
      << std::endl;
  std::cout << " output 4 i 1 s abc f 3.1415 s d \n ->  output for tuple (1, "
               "\"abc\", 3.1415, \"d\")"
            << std::endl;
  std::cout
      << " input 4 10 i == 1 s * f >= 3 s == d \n -> input for tuple "
         "(integer:1, string:*, float:>=3, string:\"d\"), timeout 10 seconds"
      << std::endl;
  std::cout << "Ctrl^C to close program." << std::endl;
}

void output() {
  size_t size;
  std::cin >> size;
  auto tuple = parse_tuple(size);
  linda.Output(tuple);
  std::cout << "> Output for " << tuple.ToString() << std::endl;
}

void input() {
  size_t size;
  int timeout;
  std::cin >> size >> timeout;
  auto tuple = parse_tuple_desc(size);
  std::cout << "> Input: wait for find tuple..." << std::endl;
  auto result = linda.Input(tuple, timeout * 1000);
  std::cout << "> Found: " << result.ToString() << std::endl;
}

void read() {
  size_t size;
  int timeout;
  std::cin >> size >> timeout;
  auto tuple = parse_tuple_desc(size);
  std::cout << "> Read: wait for find tuple..." << std::endl;
  auto result = linda.Input(tuple, timeout * 1000);
  std::cout << "> Found: " << result.ToString() << std::endl;
}

int main(int argc, char **argv, char **env) {
  if (argc < 2) {
    std::cout << "Need a sharedmemory file as arg";
    return -1;
  }

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
  std::string command = "";
  help();
  while (run) {
    std::cin >> command;
    try {
      if (command == "help")
        help();
      else if (command == "output")
        output();
      else if (command == "input")
        input();
      else if (command == "read")
        read();
      else if (command != "")
        std::cout << "# Unknown command. See 'help'" << std::endl;
    } catch (std::exception e) {
      std::cout << "# Invalid execute." << std::endl;
    }
    command = "";
  }
  return 0;
}
