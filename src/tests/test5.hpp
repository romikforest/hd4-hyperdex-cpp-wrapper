#ifndef TEST5_HPP_INCLUDED
#define TEST5_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test5.hpp
// First created: 2015-10-16-10.25
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/attributes.hpp>
#include <hyperdex_cpp/c_string_traits.hpp>
#include <hyperdex_cpp/data_traits.hpp>
#include <hyperdex_cpp/output.hpp>
#include <iostream>
#include <list>
voidtest5()
{
  //hyperdex_client_attribute arr[3]
  //hyperdex::attribute_array<3> arr
  //hyperdex::an_attribute_vector arr
  //std::list<hyperdex::an_attribute> arr
  //hyperdex::an_attribute_tuple_3 arr
  hyperdex::attributes arr
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

  return 0;

} // test 5

//#include <hyperdex_cpp/timestamp.hpp>
//#include <hyperdex_cpp/output.hpp>

//#include <hyperdex_cpp/list.hpp>

//#include <hyperdex_cpp/set.hpp>

//#include <hyperdex_cpp/map.hpp>

//#include <hyperdex_cpp/map_traits.hpp>
//#include <hyperdex_cpp/c_string_traits.hpp>
//#include <hyperdex_cpp/data_traits.hpp>

//#include <hyperdex_cpp/serializer.hpp>
//#include <hyperdex_cpp/output.hpp>

//#include <hyperdex_cpp/admin_answer.hpp>
//#include <hyperdex_cpp/client_answer.hpp>

//#include <hyperdex_cpp/admin.hpp>
//#include <hyperdex_cpp/attributes.hpp>

//#include <hyperdex_cpp/predicates.hpp>
//#include <hyperdex_cpp/map_attributes.hpp>

//#include <hyperdex_cpp/client.hpp>
//#include <hyperdex_cpp/hyperdex.hpp>

#endif // TEST5_HPP_INCLUDED
