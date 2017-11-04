#ifndef TEST8_HPP_INCLUDED
#define TEST8_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test8.hpp
// First created: 2015-10-16-10.26
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
#include <hyperdex_cpp/fill_names.hpp>
#include <boost/type_index.hpp>
#include <forest/type_names.hpp>

void test8()
{
  std::cout << "Test 8: " << std::endl;
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

//  hyperdex::attributes attrs { arr };


//  std::cout << hyperdex::is_container_of_non_cv<decltype(arr)>::value << std::endl;
//  std::cout << !std::is_same<hyperdex::container_element_t<decltype(arr)>, void>::value << std::endl;
//  std::cout << std::is_same<std::remove_cv_t<hyperdex::container_element_t<decltype(arr)>>,
//                                             hyperdex::container_element_t<decltype(arr)>
//                           >::value << std::endl;
//  std::cout << boost::typeindex::type_id_with_cvr<std::remove_cv_t<hyperdex::container_element_t<decltype(arr)>>>().pretty_name() << std::endl;
//  std::cout << boost::typeindex::type_id_with_cvr<hyperdex::container_element_t<decltype(arr)>>().pretty_name() << std::endl;


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
  std::list<hyperdex_client_map_attribute> arr
//  hyperdex::map_attribute_tuple_4 arr
//  hyperdex::map_attributes attrs
  {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Key3", 4, HYPERDATATYPE_STRING, "Val3", 4, HYPERDATATYPE_STRING },
    { "Attr4", "Key4", 4, HYPERDATATYPE_STRING, "Val4", 4, HYPERDATATYPE_STRING }
  };

//  hyperdex::map_attributes attrs { arr };

// char const* nms[]
  hyperdex::name_vector nms
  //hyperdex::name_tuple_4 nms
   {"Name1", "Name2", "Name3", "Name4"};

  //hyperdex::fill_names(arr, 4, nms, 5);
//    hyperdex::fill_names(arr, nms, 5);
//    hyperdex::fill_names(arr, 4, nms);
    hyperdex::fill_names(arr, nms);
//    hyperdex::null_names(arr, 4);
//    hyperdex::null_names(arr);


//  std::cout << attrs << std::endl;
  std::cout << arr << std::endl;

  forest::print_type_name<std::remove_cv_t<hyperdex::container_element_t<decltype(arr)>>>();
  forest::print_type_name<hyperdex::container_element_t<decltype(arr)>>();

} // test8

#endif // TEST8_HPP_INCLUDED
