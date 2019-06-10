// Copyright 2019 UXP1A Students Team

#ifndef CLIENT_LINDCLI_H_
#define CLIENT_LINDCLI_H_

#include <atomic>
#include <iostream>
#include <string>

#include "src/linda.h"

namespace client {
class LindaCli {
 public:
  LindaCli() = delete;
  explicit LindaCli(uxp::Linda *linda) : linda(linda) {}
  LindaCli(const LindaCli &) = delete;
  void Run(std::atomic_bool *stop);
  void Help();

 private:
  uxp::Linda *linda;

  static const char help_text[];

  uxp::ElementDesc::Condition ConditionFromString(std::string cmp);
  template <typename T>
  T GetVal(uxp::ElementDesc::Condition cmp);
  template <typename T>
  T GetVal();

  template <typename E>
  E ParseElement();
  template <typename T, typename E>
  T ParseTuple(size_t size);

  void Output();
  void Input();
  void Read();

  void ClearStdinLine();
};
}  // namespace client

#include "client/lindacli.cc"

#endif  // CLIENT_LINDCLI_H_
