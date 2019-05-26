// Copyright 2019 UXP1A Students Team

#ifndef WIN32
#define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE element_tests

#include <boost/test/unit_test.hpp>

#include "src/Linda.h"

using uxp::Linda;
using Element = uxp::Element;
using ElementDesc = uxp::ElementDesc;
using Condition = ElementDesc::Condidtion;
using uxp::ElemType;

bool Chk(const Element &e, const ElementDesc &ed) {
  return ChkElem(&e, &ed);
}

BOOST_AUTO_TEST_CASE(int_is_not_float) {
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::INT;
  elem.value.int_ = 6;
  elem_desc.type = ElemType::FLOAT;
  elem_desc.value.float_ = 23.4321f;
  elem_desc.condition = Condition::ANY;
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(int_is_not_string) {
  const char *str = "jabłka są smaczne";
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::INT;
  elem.value.int_ = 1267;
  elem_desc.type = ElemType::STRING;
  elem_desc.str_size = strlen(str);
  memcpy(elem_desc.value.string_, str, strlen(str));
  elem_desc.condition = Condition::ANY;
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(float_is_not_int) {
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::FLOAT;
  elem.value.float_ = -3298.213;
  elem_desc.type = ElemType::INT;
  elem_desc.value.int_ = 1;
  elem_desc.condition = Condition::GREATER_EQ;
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(string_is_not_float) {
  const char *str = "jabłka są smaczne";
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::STRING;
  elem.str_size = strlen(str);
  memcpy(elem.value.string_, str, strlen(str));
  elem_desc.type = ElemType::FLOAT;
  elem_desc.value.float_ = 34.12;
  elem_desc.condition = Condition::ANY;
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(float_not_equal) {
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::FLOAT;
  elem.value.float_ = 12.3;
  elem_desc.type = ElemType::FLOAT;
  elem_desc.value.float_ = 12.3;
  elem_desc.condition = Condition::EQUAL;
  BOOST_CHECK(!Chk(elem, elem_desc));
}

BOOST_AUTO_TEST_CASE(strings_equal) {
  const char *str = "no ale pomarańcze też";
  Element elem;
  ElementDesc elem_desc;
  elem.type = ElemType::STRING;
  elem.str_size = strlen(str);
  memcpy(elem.value.string_, str, strlen(str));
  elem_desc.type = ElemType::STRING;
  elem_desc.str_size = strlen(str);
  memcpy(elem_desc.value.string_, str, strlen(str));
  elem_desc.condition = Condition::EQUAL;
  BOOST_CHECK(Chk(elem, elem_desc));
}

