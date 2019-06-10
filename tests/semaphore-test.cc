// Copyright 2019 UXP1A Students Team

#ifndef WIN32
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE element_tests

#include <boost/test/unit_test.hpp>

#include "src/semaphore.h"

BOOST_AUTO_TEST_CASE(test_semaphore_flow) {
  uxp::SemaphoreTable semtab("semkeypath");
  uxp::Semaphore sem(&semtab, 0);
  sem.Initialize(2);

  BOOST_TEST(sem.GetValue() == 2);

  sem.P();
  BOOST_TEST(sem.GetValue() == 1);

  sem.P();
  BOOST_TEST(sem.GetValue() == 0);

  sem.V();
  BOOST_TEST(sem.GetValue() == 1);
  BOOST_TEST(sem.IsZero() == false);

  sem.P();
  BOOST_TEST(sem.GetValue() == 0);
  BOOST_TEST(sem.IsZero() == true);

}
