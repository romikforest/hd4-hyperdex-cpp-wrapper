#ifndef TEST10_HPP_INCLUDED
#define TEST10_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test10.hpp
// First created: 2015-10-16-10.27
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test10()
{
  std::cout << "Test 10: " << std::endl;

  using forest::string;
  //hyperdex_client_attribute arr[4]
  //hyperdex::an_attribute arr[4]
  //hyperdex::attribute_array<4> arr
  //forest::array<hyperdex_client_attribute, 4> arr
  //hyperdex::an_attribute_vector arr
  //std::list<hyperdex::an_attribute> arr
//  hyperdex::an_attribute_tuple_4 arr
  //hyperdex::attributes attrs
//  {
//    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
//    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
//    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING },
//    { "Attr4", "Val4", 4, HYPERDATATYPE_STRING }
//  };

  int i = 5;
  char c = 8;
  forest::int128_t i128 = 10;

  hyperdex::attributes attrs {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4 },
    { "Attr3", "Val3" },
    { "Attr4", 5 },
    { string("Attr5"), "Val5", 4, HYPERDATATYPE_STRING },
    { string("Attr6"), "Val6", 4 },
    { string("Attr7"), "Val7" },
    { string("Attr8"), 5 },
    { string("Attr9"), string("Val10") },
    { "Attr10", 25.4  },
    { "Attr11", i  },
    { "Attr12", c  },
    { "Attr13", i128  },
    { "Attr14", string{"aaa\0bbb",7}  },
  };

  std::cout << attrs << std::endl;
//  std::cout << arr << std::endl;

} // test10

#endif // TEST10_HPP_INCLUDED
