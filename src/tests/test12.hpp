#ifndef TEST12_HPP_INCLUDED
#define TEST12_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test12.hpp
// First created: 2015-10-16-10.27
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test12()
{
  std::cout << "Test 12: " << std::endl;

  using forest::string;

  int i = 5;
  char c = 8;
  forest::int128_t i128 = 10;

  hyperdex::map_attributes attrs {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2" },
    { "Attr3", "Key3", "Val3", 4, HYPERDATATYPE_STRING },
    { "Attr4", 5, 6 },
    { string("Attr5"), "Key5", 4, HYPERDATATYPE_STRING, 6 },
    { string("Attr6"), string("Val6"), HYPERPREDICATE_EQUALS },
    { "Attr7", i, string("ggg") },
    { "Attr8", c, string("ggg")  },
    { "Attr9", i128, string("ggg")  },
    { "Attr10", string{"aaa\0bbb",7}, string{"ccc\0ddd",7}  },
  };

  std::cout << attrs << std::endl;
//  std::cout << arr << std::endl;

} // test12

#endif // TEST12_HPP_INCLUDED
