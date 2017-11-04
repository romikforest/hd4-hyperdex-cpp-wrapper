#ifndef TEST11_HPP_INCLUDED
#define TEST11_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test11.hpp
// First created: 2015-10-16-10.27
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test11()
{
  std::cout << "Test 11: " << std::endl;

  using forest::string;

  int i = 5;
  char c = 8;
  forest::int128_t i128 = 10;

  hyperdex::predicates attrs {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
    { "Attr3", "Val3", HYPERPREDICATE_EQUALS },
    { "Attr4", 5, HYPERPREDICATE_EQUALS },
    { string("Attr5"), "Val5", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
    { string("Attr7"), "Val7", HYPERPREDICATE_EQUALS },
    { string("Attr8"), 5, HYPERPREDICATE_EQUALS },
    { string("Attr9"), string("Val10"), HYPERPREDICATE_EQUALS },
    { "Attr10", 25.4, HYPERPREDICATE_EQUALS  },
    { "Attr11", i, HYPERPREDICATE_EQUALS },
    { "Attr12", c, HYPERPREDICATE_EQUALS  },
    { "Attr13", i128, HYPERPREDICATE_EQUALS  },
    { "Attr14", string{"aaa\0bbb",7}, HYPERPREDICATE_EQUALS  },
  };

  std::cout << attrs << std::endl;
//  std::cout << arr << std::endl;

} // test11

#endif // TEST11_HPP_INCLUDED
