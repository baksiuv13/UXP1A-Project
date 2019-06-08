// Copyright 2019 UXP1A Students Team

#include "client/lindacli.h"

namespace client {

const char LindaCli::help_text[] =
    "Possible commands: \n"
    "* help - see this\n"
    "* output <size:int> {<type:i|f|s> <value:int|float|string>}\n"
    "* input <size:int> <timeout_sec:int> {<type:i|f|s> "
    "<cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}\n"
    "* read <size:int> <timeout_sec:int> {<type:i|f|s> "
    "<cmp:==|<|<=|>|>=|*> [<value:int|float|string>]}\n\n"
    "When you declare tuple pattern, don't set value for *. Examples:\n"
    " output 4 i 1 s abc f 3.1415 s d \n -> output for tuple (1, "
    "\"abc\", 3.1415, \"d\")\n"
    " input 4 10 i == 1 s * f >= 3 s == d \n -> input for tuple "
    "(integer:1, string:*, float:>=3, string:\"d\"), timeout 10 seconds\n"
    "Ctrl^C or Ctrl^D to close program.\n";

void LindaCli::Run(std::atomic_bool *stop) {
  while (!*stop) {
    std::string command = "";
    std::cout << "$ ";
    std::cout.flush();
    std::cin >> command;
    if (std::cin.eof()) {
      break;
    }
    try {
      if (command == "help")
        Help();
      else if (command == "output")
        Output();
      else if (command == "input")
        Input();
      else if (command == "read")
        Read();
      else if (command != "")
        std::cout << "# Unknown command. See 'help'" << '\n';
    } catch (std::runtime_error &e) {
      std::cout << "# Invalid execute: " << e.what() << '\n';
      ClearStdinLine();
    }
  }
  std::cout << "\n# Quit\n";
}

template <>
uxp::Element LindaCli::ParseElement<uxp::Element>() {
  std::string type;
  std::cin >> type;
  if (type == "i") {
    return uxp::Element(GetVal<int>());
  } else if (type == "f") {
    return uxp::Element(GetVal<float>());
  } else if (type == "s") {
    return uxp::Element(GetVal<std::string>().c_str());
  } else {
    std::cout << "Element type '" + type + " is invalid\n";
    throw std::runtime_error("Invalid type");
  }
}

template <>
uxp::ElementDesc LindaCli::ParseElement<uxp::ElementDesc>() {
  std::string type, cond;
  std::cin >> type >> cond;
  auto condition = ConditionFromString(cond);
  if (type == "i") {
    return uxp::ElementDesc(GetVal<int32_t>(condition), condition);
  } else if (type == "f") {
    return uxp::ElementDesc(GetVal<float>(condition), condition);
  } else if (type == "s") {
    return uxp::ElementDesc(GetVal<std::string>(condition).c_str(), condition);
  } else {
    std::cout << "Element type '" + type + " is invalid\n";
    throw std::runtime_error("Invalid type");
  }
}

template <typename T, typename E>
T LindaCli::ParseTuple(size_t size) {
  T tuple;
  tuple.size = size;
  for (size_t i = 0; i < size; ++i) {
    tuple.elements[i] = ParseElement<E>();
  }
  if (tuple.size == 0) throw std::runtime_error("Invalid tuple");
  return tuple;
}

uxp::ElementDesc::Condition LindaCli::ConditionFromString(std::string cmp) {
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
T LindaCli::GetVal(uxp::ElementDesc::Condition cmp) {
  if (cmp == uxp::ElementDesc::ANY) return T();
  return GetVal<T>();
}

template <typename T>
T LindaCli::GetVal() {
  T val;
  std::cin >> val;
  return val;
}

void LindaCli::Help() { std::cout << help_text; }

void LindaCli::Output() {
  size_t size;
  std::cin >> size;
  auto tuple = ParseTuple<uxp::Tuple, uxp::Element>(size);
  linda->Output(tuple);
  std::cout << "> Output for " << tuple.ToString() << '\n';
}

void LindaCli::Input() {
  size_t size;
  int timeout;
  std::cin >> size >> timeout;
  auto tuple = ParseTuple<uxp::TupleDesc, uxp::ElementDesc>(size);
  std::cout << "> Input: wait for find tuple..." << '\n';
  auto result = linda->Input(tuple, timeout * 1000);
  if (result.size == 0)
    std::cout << "# Timeout expired.\n";
  else
    std::cout << "> Found: " << result.ToString() << '\n';
}

void LindaCli::Read() {
  size_t size;
  int timeout;
  std::cin >> size >> timeout;
  auto tuple = ParseTuple<uxp::TupleDesc, uxp::ElementDesc>(size);
  if (tuple.size == 0) {
    std::cout << "# Invalid tuple.";
    return;
  }
  std::cout << "> Read: wait for find tuple...\n";
  auto result = linda->Read(tuple, timeout * 1000);
  if (result.size == 0)
    std::cout << "# Timeout expired.\n";
  else
    std::cout << "> Found: " << result.ToString() << '\n';
}

void LindaCli::ClearStdinLine() {
  int c;
  while ((c = std::cin.get()) != '\n' && c != EOF) {
  }
  std::cin.clear();
}

}  // namespace client
