#ifndef TEST6_HPP_INCLUDED
#define TEST6_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test6.hpp
// First created: 2015-10-16-10.25
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/attributes.hpp>
#include <hyperdex_cpp/c_string_traits.hpp>
#include <hyperdex_cpp/data_traits.hpp>
#include <hyperdex_cpp/output.hpp>
#include <iostream>
#include <list>

void test6()
{
  using forest::string;
  //hyperdex_client_attribute arr[3]
  //hyperdex::attribute_array<3> arr
  //hyperdex::an_attribute_vector arr
  //std::list<hyperdex::an_attribute> arr
  hyperdex::an_attribute_tuple_3 arr
  //hyperdex::attributes arr
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs {};
  attrs = arr;

  std::cout << attrs << std::endl;

//  std::cout << hyperdex::is_attribute_container<std::list<hyperdex_client_attribute>>::value << std::endl;
//  std::cout << hyperdex::is_attribute_container<std::list<hyperdex::an_attribute>>::value << std::endl;

  string v1, v2, v3, v4;
  hyperdex::attribute_status_vector sv;

  attrs.by_names(std::tie(v1,v2,v3, v4), {"Attr1", "Attr2", "Attr3"});

  std::cout << "v1: " << v1 << std::endl;
  std::cout << "v2: " << v2 << std::endl;
  std::cout << "v3: " << v3 << std::endl;
  std::cout << "v4: " << v4 << std::endl;
  std::cout << sv << std::endl;

} // test 6

#endif // TEST6_HPP_INCLUDED
