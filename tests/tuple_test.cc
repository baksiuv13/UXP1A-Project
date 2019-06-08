// Copyright 2019 UXP1A Students Team

#ifndef WIN32
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE tuple_tests

#include <boost/test/unit_test.hpp>

#include "src/element.h"
#include "src/tuple.h"

using uxp::Element;
using uxp::ElementDesc;
using Condition = ElementDesc::Condition;
using uxp::ElemType;
using uxp::Tuple;
using uxp::TupleDesc;

bool Chk(const Tuple &t, const TupleDesc &td) {
  return uxp::CheckTuple(&t, &td);
}

BOOST_AUTO_TEST_CASE(check_int_less_str_any) {
  Tuple t;
  t.size = 2;
  t.elements[0] = Element(5);
  t.elements[1] = Element("pizza");
  TupleDesc td;
  td.size = 2;
  td.elements[0] = ElementDesc(6, Condition::LESS_EQ);
  td.elements[1] = ElementDesc("spaghetti", Condition::ANY);

  BOOST_CHECK(Chk(t, td));
}

BOOST_AUTO_TEST_CASE(check_int_equal_str_greater_float_less) {
  Tuple t;
  t.size = 3;
  t.elements[0] = Element(5);
  t.elements[1] = Element("haha");
  t.elements[2] = Element(9.6f);

  TupleDesc td;
  td.size = 3;
  td.elements[0] = ElementDesc(5, Condition::EQUAL);
  td.elements[1] = ElementDesc("chehehe", Condition::GREATER_EQ);
  td.elements[2] = ElementDesc(10.0f, Condition::LESS);

  BOOST_CHECK(Chk(t, td));
}
