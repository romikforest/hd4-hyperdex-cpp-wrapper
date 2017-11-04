#ifndef TEST4_HPP_INCLUDED
#define TEST4_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test4.hpp
// First created: 2015-10-16-10.24
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>
using forest::string;

void test4()
{
  /*
  std::cout << "Test 4: Attributes transformations" << std::endl;

  std::cout << "Construction from: " << std::endl;

  struct in_data1
  {
    char const* s = "My text";
    string v = "Some test";
    int i = 10;
    float f = 5.5;
  } in1;

  hyperdex::attributes attrs {
                              { "v", in1.v },
                              { "s", in1.s },
                              { "i", in1.i },
                              { "f", in1.f }
                             };
  std::cout << "Simple: " << attrs << std::endl;

  hyperdex::an_attribute_array<2> aaa
  {{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING }
  }};

  hyperdex::attributes attrs2 { aaa };
  std::cout << "From c++ array " << attrs2 << std::endl;

  hyperdex::an_attribute aac[]
  {
    { "Attr1c", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2c", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs3 { aac };
  std::cout << "From c array: " << attrs3 << std::endl;

  hyperdex::an_attribute_vector aav
  {
    { "Attr1v", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2v", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs4 { aav };
  std::cout << "From vector: " << attrs4 << std::endl;

  hyperdex::an_attribute_tuple_2 aat
  {
    { "Attr1t", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2t", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs5 { aat };
  std::cout << "From tuple: " << attrs5 << std::endl;

  std::cout << "Assignment from: " << std::endl;

  attrs.set ({
              { "v", in1.v },
              { "s", in1.s },
              { "i", in1.i },
              { "f", in1.f }
            });

  std::cout << "Simple: " << attrs << std::endl;

  attrs2 = aaa;
  std::cout << "From c++ array " << attrs2 << std::endl;

  attrs3 = aac;
  std::cout << "From c array: " << attrs3 << std::endl;

  attrs4 = aav;
  std::cout << "From vector: " << attrs4 << std::endl;

  attrs5 = aat;
  std::cout << "From tuple: " << attrs5 << std::endl;
*/

  std::cout << "Test 4: Attributes transformations" << std::endl;

  std::cout << "Construction from: " << std::endl;

  struct in_data1
  {
    char const* s = "My text";
    string v = "Some test";
    int i = 10;
    float f = 5.5;
  } in1;

  hyperdex::attributes attrs {
                              { "v", in1.v },
                              { "s", in1.s },
                              { "i", in1.i },
                              { "f", in1.f }
                             };
  std::cout << "Simple: " << attrs << std::endl;

  hyperdex::attribute_array<2> aaa
  {{
    { "Attr1", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2", "Val2", 4, HYPERDATATYPE_STRING }
  }};

  hyperdex::attributes attrs2 { aaa };
  std::cout << "From c++ array " << attrs2 << std::endl;

  hyperdex_client_attribute aac[]
  {
    { "Attr1c", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2c", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs3 { aac };
  std::cout << "From c array: " << attrs3 << std::endl;

  hyperdex::attribute_vector aav
  {
    { "Attr1v", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2v", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs4 { aav };
  std::cout << "From vector: " << attrs4 << std::endl;

  hyperdex::attribute_tuple_2 aat
  {
    { "Attr1t", "Val1", 4, HYPERDATATYPE_STRING },
    { "Attr2t", "Val2", 4, HYPERDATATYPE_STRING }
  };

  hyperdex::attributes attrs5 { aat };
  std::cout << "From tuple: " << attrs5 << std::endl;

  std::cout << "Assignment from: " << std::endl;

  attrs.set ({
              { "v", in1.v },
              { "s", in1.s },
              { "i", in1.i },
              { "f", in1.f }
            });

  std::cout << "Simple: " << attrs << std::endl;

  attrs2 = aaa;
  std::cout << "From c++ array " << attrs2 << std::endl;

  attrs3 = aac;
  std::cout << "From c array: " << attrs3 << std::endl;

  attrs4 = aav;
  std::cout << "From vector: " << attrs4 << std::endl;

  attrs5 = aat;
  std::cout << "From tuple: " << attrs5 << std::endl;

} // test4

#endif // TEST4_HPP_INCLUDED
