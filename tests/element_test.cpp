// Copyright 2019 UXP1A Students Team

#ifndef WIN32
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE element_tests

#include <boost/test/unit_test.hpp>

#include "src/element.h"

using uxp::Element;
using uxp::ElementDesc;
using Condition = ElementDesc::Condition;
using uxp::ElemType;

bool Chk(const Element &e, const ElementDesc &ed) {
  return uxp::ChkElem(&e, &ed);
}

BOOST_AUTO_TEST_CASE(int_is_not_float) {
  Element elem(6);
  ElementDesc elem_desc(23.4321f, Condition::ANY);
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(int_is_not_string) {
  Element elem(1267);
  ElementDesc elem_desc("jabłka są smaczne", Condition::ANY);
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(float_is_not_int) {
  Element elem(-3298.213f);
  ElementDesc elem_desc(1, Condition::GREATER_EQ);
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(string_is_not_float) {
  Element elem("jabłka są smaczne");
  ElementDesc elem_desc(34.12f, Condition::ANY);
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(float_not_equal) {
  Element elem(12.3f);
  ElementDesc elem_desc(12.3f, Condition::EQUAL);
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(strings_equal) {
  Element elem("no ale pomarańcze też");
  ElementDesc elem_desc("no ale pomarańcze też", Condition::EQUAL);
  BOOST_CHECK(Chk(elem, elem_desc));
}

