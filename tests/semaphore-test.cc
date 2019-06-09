// Copyright 2019 UXP1A Students Team

#ifndef WIN32
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE element_tests

#include <boost/test/unit_test.hpp>

#include "src/semaphore.h"

BOOST_AUTO_TEST_CASE(test_semaphore_flow) {
  uxp::Semaphore sem = uxp::Semaphore(0);
  sem.initialize(2);

  BOOST_TEST(sem.getValue() == 2);

  sem.P();
  BOOST_TEST(sem.getValue() == 1);

  sem.P();
  BOOST_TEST(sem.getValue() == 0);

  sem.V();
  BOOST_TEST(sem.getValue() == 1);
  BOOST_TEST(sem.isZero() == false);

  sem.P();
  BOOST_TEST(sem.getValue() == 0);
  BOOST_TEST(sem.isZero() == true);

  bool close_result = uxp::Semaphore::closeSemTable();
  BOOST_TEST(close_result == true);
  BOOST_TEST(sem.getValue() == -1);
}
