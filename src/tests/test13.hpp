#ifndef TEST13_HPP_INCLUDED
#define TEST13_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test13.hpp
// First created: 2015-10-16-10.28
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/serializer.hpp>
#include <hyperdex_cpp/c_string_traits.hpp>
#include <hyperdex_cpp/data_traits.hpp>
#include <hyperdex_cpp/output.hpp>
#include <iostream>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/log.hpp>

void test13()
{
  std::cout << "Test 13: Serializer" << std::endl;

  using forest::string;

  hyperdex::serializer s{};

  hyperdex::attribute_c_array<5> aa{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    s.serialize(hyperdex::attribute_tag, string{"Attr2"}, "Val2", 4, HYPERDATATYPE_STRING),
    s.serialize(hyperdex::attribute_tag, string{"Attr3"}, "Val3", 4),
    s.serialize(hyperdex::attribute_tag, string{"Attr4"}, string("Val4")),
    s.serialize(hyperdex::attribute_tag, string{"Attr5"}, 5)
  };

  if (!s) LOG(error) << "Serialization error!";

  std::cout << aa << std::endl;

  hyperdex::predicate_c_array<7> pa{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS },
    s.serialize(hyperdex::predicate_tag, string{"Attr2"}, "Val2", 4, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS),
    s.serialize(hyperdex::predicate_tag, string{"Attr3"}, "Val3", 4, HYPERDATATYPE_STRING),
    s.serialize(hyperdex::predicate_tag, string{"Attr4"}, "Val4", 4),
    s.serialize(hyperdex::predicate_tag, string{"Attr5"}, string("Val5"), HYPERPREDICATE_EQUALS),
    s.serialize(hyperdex::predicate_tag, string{"Attr6"}, 5, HYPERPREDICATE_EQUALS),
    s.serialize(hyperdex::predicate_tag, string{"Attr7"}, 5)
  };

  if (!s) LOG(error) << "Serialization error!";

  std::cout << pa << std::endl;

  hyperdex::map_attribute_c_array<7> ma{
    { "Attr1", "Key1", 4, HYPERDATATYPE_STRING, "Val1", 4, HYPERDATATYPE_STRING },
    s.serialize(hyperdex::map_attribute_tag, string{"Attr2"}, "Key2", 4, HYPERDATATYPE_STRING, "Val2", 4, HYPERDATATYPE_STRING ),
    s.serialize(hyperdex::map_attribute_tag, string{"Attr3"}, "Key3", 4, HYPERDATATYPE_STRING, "Val3"),
    s.serialize(hyperdex::map_attribute_tag, string{"Attr4"}, "Key4", "Val4", 4, HYPERDATATYPE_STRING),
    s.serialize(hyperdex::map_attribute_tag, string{"Attr5"}, string("Key5"), string("Val5")),
    s.serialize(hyperdex::map_attribute_tag, string{"Attr6"}, 5, 8),
    s.serialize(hyperdex::map_attribute_tag, string{"Attr7"}, string("Key\0 with 0 sym", 15), string("Val\0 with 0 sym", 15))
  };

  if (!s) LOG(error) << "Serialization error!";

  std::cout << ma << std::endl;

} // test13

#endif // TEST13_HPP_INCLUDED
