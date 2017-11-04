#ifndef TEST7_HPP_INCLUDED
#define TEST7_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test7.hpp
// First created: 2015-10-16-10.25
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/attributes.hpp>
#include <hyperdex_cpp/predicates.hpp>
#include <hyperdex_cpp/map_attributes.hpp>
#include <hyperdex_cpp/c_string_traits.hpp>
#include <hyperdex_cpp/data_traits.hpp>
#include <hyperdex_cpp/output.hpp>
#include <iostream>
#include <list>
#include <hyperdex_cpp/linear_search.hpp>



void test7()
{
  std::cout << "Test 7: " << std::endl;
    using forest::string;
  //hyperdex_client_attribute arr[4]
  //hyperdex::attribute_array<4> arr
  //forest::array<hyperdex_client_attribute, 4> arr
  //hyperdex::an_attribute_vector arr
  //std::list<hyperdex::an_attribute> arr
  //hyperdex::an_attribute_tuple_4 arr
  //hyperdex::attributes attrs
//  {
//    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
//    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
//    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING },
//    { "Attr4", "Val4", 4, HYPERDATATYPE_STRING }
//  };
//
//  hyperdex::attributes attrs { arr };

  //hyperdex_client_attribute_check arr[4]
  //hyperdex::predicate_array<4> arr
  //forest::array<hyperdex_client_attribute_check, 4> arr
  //hyperdex::predicate_vector arr
  //std::list<hyperdex_client_attribute_check> arr
  //hyperdex::predicate_tuple_4 arr
  //hyperdex::predicates attrs
//  {
//    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
//    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
//    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
//    { "Attr4", "Val4", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS }
//  };

//  hyperdex::predicates attrs { arr };

//  hyperdex_client_map_attribute arr[4]
//  hyperdex::map_attribute_array<4> arr
//  forest::array<hyperdex_client_map_attribute, 4> arr
//  hyperdex::map_attribute_vector arr
//  std::list<hyperdex_client_map_attribute> arr
//  hyperdex::map_attribute_tuple_4 arr
  hyperdex::map_attributes attrs
  {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Key3", 4, HYPERDATATYPE_STRING, "Val3", 4, HYPERDATATYPE_STRING },
    { "Attr4", "Key4", 4, HYPERDATATYPE_STRING, "Val4", 4, HYPERDATATYPE_STRING }
  };

//  hyperdex::map_attributes attrs { arr };

//  std::cout << hyperdex::is_cpp_array<decltype(arr)>::value << std::endl;
//  std::cout << hyperdex::is_linear_safe_container<decltype(arr)>::value << std::endl;
//  std::cout << hyperdex::is_attribute_container<decltype(arr)>::value << std::endl;
//  std::cout << hyperdex::is_linear_safe_attribute_container<decltype(arr)>::value << std::endl;

  std::cout << attrs << std::endl;
//  std::cout << arr << std::endl;

  //std::cout << attrs.linear_search("Attr2") << std::endl;
  std::cout << hyperdex::linear_search(attrs, "Attr3") << std::endl;

  //std::cout << attrs.linear_search(string("Attr2")) << std::endl;
  std::cout << hyperdex::linear_search(attrs, string("Attr3")) << std::endl;

} // test7

#endif // TEST7_HPP_INCLUDED
