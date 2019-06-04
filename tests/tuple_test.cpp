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

bool Chk(const Element &e, const ElementDesc &ed) {
  return uxp::ChkElem(&e, &ed);
}

bool Chk(const Tuple &t, const TupleDesc &td) {
  return uxp::ChkTuple(&t, &td);
}

BOOST_AUTO_TEST_CASE(int_is_not_float) {
  Tuple t;
  TupleDesc td;
  t.size = 2;
  td.size = 2;
  t.elements[0] = Element(5);
  td.elements[0] = ElementDesc(6, Condition::LESS_EQ);
  t.elements[1] = Element("pizza");
  td.elements[1] = ElementDesc("spaghetti", Condition::ANY);
  BOOST_CHECK(Chk(t, td));
}

