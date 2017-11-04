#ifndef TEST2_HPP_INCLUDED
#define TEST2_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test2.hpp
// First created: 2015-10-16-10.23
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test2()
{
  std::cout << "Test 2: Print" << std::endl;

  hyperdex_client_attribute a1 { "Attribute1", "Value1", 6, HYPERDATATYPE_STRING },
                            a2 { "Attribute2", "Value2", 6, HYPERDATATYPE_STRING };

  hyperdex::an_attribute aa1 { "An attribute1", "Value1", 6, HYPERDATATYPE_STRING },
                         aa2 { "An attribute2", "Value2", 6, HYPERDATATYPE_STRING };

  std::cout << "Standard hyperdex attributes:" << std::endl <<  a1 <<  a2;
  std::cout << "Wrapped hyperdex attributes:"  << std::endl << aa1 << aa2;

  std::cout << "Standard hyperdex attributes from pointer:" << std::endl <<  &a1 <<  &a2;
  std::cout << "Wrapped hyperdex attributes from pointer:"  << std::endl << &aa1 << &aa2;

  hyperdex_client_attribute_check p1 { "Predicate1", "Value1", 6, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS    },
                                  p2 { "Predicate2", "Value2", 6, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN };

  std::cout << "Standard hyperdex predicate:" << std::endl << p1 << p2;
  std::cout << "Standard hyperdex predicate from pointer:" << std::endl << &p1 << &p2;

  hyperdex_client_map_attribute m1 { "MapAttribute1",
                                     "Key1",   4, HYPERDATATYPE_STRING,
                                     "Value1", 6, HYPERDATATYPE_STRING },
                                m2 { "MapAttribute2",
                                     "Key2",   4, HYPERDATATYPE_STRING,
                                     "Value2", 6, HYPERDATATYPE_STRING };

  std::cout << "Standard hyperdex map attribute:" << std::endl << m1 << m2;
  std::cout << "Standard hyperdex map attribute from pointer:" << std::endl << &m1 << &m2;

  hyperdex_admin_perf_counter pc { 5, 25, "ggg", 100 };

  std::cout << "Standard hyperdex admin_perf_counter:" << std::endl << pc;
  std::cout << "Standard hyperdex admin_perf_counter from pointer:" << std::endl << &pc;

  {
    using hyperdex::operator<<;

    std::pair<int, int> iip { 10, 20 };
    std::cout << "Pair int - int: " << iip << std::endl;

    hyperdex::blob_pair bp { "Some string", 11 };
    std::cout << "blob pairs: " << bp << " " << std::make_pair("Blob (not printing)", 4) << std::endl;
  }

  hyperspace_returncode hs { HYPERSPACE_UNKNOWN_ATTR };
  std::cout << "hyperspace_returncode: " << hs << std::endl;

  hyperdex_ds_returncode ds { HYPERDEX_DS_WRONG_STATE };
  std::cout << "hyperdex_ds_returncode: " << ds << std::endl;
  std::cout << "Error output for serialization/deserialization:" << std::endl;
  hyperdex::log_ds_status(ds);
  std::cout << "Error output for malformed list:" << std::endl;
  hyperdex::log_malformed_list();
  std::cout << "Error output for malformed set:"  << std::endl;
  hyperdex::log_malformed_set();
  std::cout << "Error output for malformed map:"  << std::endl;
  hyperdex::log_malformed_map();

  ////
  hyperdex::attributes attrs { { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
                               { "Attr2", 5 },
                               { "Attr3", 10.3 }
                             };

  std::cout << "Attributes: " << attrs << std::endl;

  // array<hyperdex_client_attribute, 3> attrs_arr
  hyperdex::attribute_array<3> attrs_arr
  {{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  }};

  std::cout << "Attributes array (C++): " << attrs_arr << std::endl;

  //hyperdex_client_attribute attrs_c_arr[]
  hyperdex::attribute_c_array<3> attrs_c_arr
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "Attributes array (C): " << attrs_c_arr << std::endl;

  //vector<hyperdex_client_attribute> attrs_vec
  hyperdex::attribute_vector attrs_vec
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "Attributes vector: " << attrs_vec << std::endl;

  hyperdex::attribute_tuple_3 attrs_tup
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "Attributes tuple: " << attrs_tup << std::endl;

  // array<an_attribute, 3> an_attrs_arr
  hyperdex::an_attribute_array<3> an_attrs_arr
  {{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  }};

  std::cout << "An attributes array (C++): " << an_attrs_arr << std::endl;

  //an_attribute an_attrs_c_arr[]
  hyperdex::an_attribute_c_array<3> an_attrs_c_arr
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "An attributes array (C): " << an_attrs_c_arr << std::endl;

  //vector<an_attribute> an_attrs_vec
  hyperdex::an_attribute_vector an_attrs_vec
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "An attributes vector: " << an_attrs_vec << std::endl;

  hyperdex::an_attribute_tuple_3 an_attrs_tup
  {
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING },
    { "Attr3", "Val3", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "An attributes tuple: " << an_attrs_tup << std::endl;

  hyperdex::predicates preds { { "Pred1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_CONTAINS_LESS_THAN },
                               { "Pred2", 5,         HYPERPREDICATE_LENGTH_EQUALS },
                               { "Pred3", 10.3,      HYPERPREDICATE_LESS_THAN },
                               { "", "", 0, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN }
                             };

  std::cout << "Predicates: " << preds << std::endl;

  // array<hyperdex_client_attribute_check, 3> preds_arr
  hyperdex::predicate_array<3> preds_arr
  {{
    { "Pred1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN },
    { "Pred2", "Val2", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN },
    { "Pred3", "Val3", 0, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN }
  }};

  std::cout << "Predicates array: " << preds_arr << std::endl;

  hyperdex::predicate_tuple_3 preds_tup
  {
    { "Pred1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN },
    { "Pred2", "Val2", 0, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN },
    { "Pred3", "Val3", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_LESS_THAN }
  };

  std::cout << "Predicates tuple: " << preds_tup << std::endl;

  hyperdex::name_array<3> nm_arr { "Name1", "Name2", "" };

  std::cout << "Name C++ array: " << nm_arr << std::endl;

  hyperdex::name_tuple_3 nm_tup { "Name1", "Name2", "" };

  std::cout << "Name tuple: " << nm_tup << std::endl;

  using forest::string;

  hyperdex::map_attributes map_a
  {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING },
    { string("Attr3"), string("Key3"), "Val3", 4, HYPERDATATYPE_STRING },
    { string("Attr4"), "Key4", 4, HYPERDATATYPE_STRING, string("Val4") },
    { "Attr5", "Key5", "Val5" },
    { "Attr6", "Key6", 10 }
  };

  std::cout << "Map attributes: " << map_a << std::endl;

  hyperdex::map_attribute_vector map_v
  {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "Map attribute vector: " << map_v << std::endl;

  hyperdex::map_attribute_tuple_2 map_t
  {
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING }
  };

  std::cout << "Map attribute tuple: " << map_t << std::endl;

} // test2

#endif // TEST2_HPP_INCLUDED
