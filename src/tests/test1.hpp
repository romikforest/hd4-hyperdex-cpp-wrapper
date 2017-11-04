#ifndef TEST1_HPP_INCLUDED
#define TEST1_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test1.hpp
// First created: 2015-10-16-10.23
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test1()
{
  std::cout << "Test 1: Simple use. Express tour." << std::endl;

  using forest::string;

  hyperdex::client client{};
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  float f = 5.4;
  hyperdex::string_list ls { "Oggr", string("Hoccc"), string("aa\0bb", 5) };

  hyperdex::attributes attrs {
                              {"v", "Hello, Piter!"},
                              {"i", 10.5},
                              { "f", f },
                              { "ls", ls }
                             };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return; }

  //hyperdex::attribute_vector av { {"v", "Val1", 4, HYPERDATATYPE_STRING } };

  //auto ans = client.put("kv", "some_key", {{ {"v", "Val1", 4, HYPERDATATYPE_STRING } }});
  //auto ans = client.put("kv", "some_key", av);
  //auto ans = client.put("kv", "some_key", hyperdex::attribute_vector{ {"v", "Val1", 4, HYPERDATATYPE_STRING } });
  auto ans = client.put("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "put error"; return; }
  else                      LOG(info)  << "put done";

  ans = client.get("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "get error"; return; }
  else                      LOG(info)  << "get done";

//  std::cout << "All elements:" << std::endl;
//
//  std::cout << attrs << std::endl;

//  std::cout << "All elements foreach:" << std::endl;
//
//  for (auto& x: attrs)
//  {
//    std::cout << x << std::endl;
//  }

  std::cout << "Selected elements:" << std::endl;

  string v  = attrs["v"];
  int64_t i = attrs["i"];
  double f_ = attrs["f"];

  std::cout << "v: "  << v
            << " i: " << i
            << " f: " << f_ << std::endl;

  v = ""; i = 0; f_ = 0;

//  attrs.by_names(tie(v,i,f_), {"v", "i", "f"});
//
//  std::cout << "From tie: v: "  << v
//            << " i: " << i
//            << " f: " << f_ << std::endl;
  hyperdex::attribute_status_vector statuses {};

  attrs.by_names(tie(v,i,f_), {"v", "i", "f"}, statuses);

  std::cout << "From tie with statuses: v: "  << v
            << " i: " << i
            << " f: " << f_ << std::endl;

  std::cout << "Statuses: "  << statuses << std::endl;

  ls = attrs["ls"];

  std::cout << "List of string: " << std::endl;
  for (const auto& val: ls)
  {
    std::cout << val << std::endl;
  }

} // test1

#endif // TEST1_HPP_INCLUDED
