#ifndef TEST16_HPP_INCLUDED
#define TEST16_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test16.hpp
// First created: 2015-10-16-10.29
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test16()
{
  std::cout << "Test 16: " << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  hyperdex::attributes attrs { {"s", "lolol"}, {"i", 10}, {"v", "ggg"} };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return; }

  string space = "kv";
  string key = "some_key";

  auto ans = client.put_sync(space, key, attrs);

  if (!ans) { LOG(error) << "put error"; return; }
  else        LOG(info)  << "put done";

  //hyperdex::name_c_array<3> const nms {"v", "s", "i"};
  hyperdex::name_vector nms {"v", "s", "i"};

  ans = client.get_partial_sync(space, {"some_key", 8}, nms, attrs);
  // ans = client.get_partial_sync(space, key, {nms, 3}, attrs);

  if (!ans) { LOG(error) << "get_partial error"; return; }
  else        LOG(info)  << "get_partial done";

  std::cout << "Result: " << attrs << std::endl;

} // test16

#endif // TEST16_HPP_INCLUDED
