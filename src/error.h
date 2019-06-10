// Copyright 2019 UXP1A Students Team

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <string>
#include <cstring>
#include <iostream>

namespace uxp {

inline std::string EXCEPTION_MSG(const char *msg) {
  int e = errno;
  errno = 0;
  return std::string(msg) + ":\n\t" + std::strerror(e) + '\n';
}

inline void ERROR_PRINT(const char *msg) {
  std::cerr << EXCEPTION_MSG(msg);
  errno = 0;
}

template <typename T>
inline void Throw(const char *msg) {
  throw T(EXCEPTION_MSG(msg));
}

}  // namespace uxp

#endif  // SRC_ELEMENT_H_
