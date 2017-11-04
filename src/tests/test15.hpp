#ifndef TEST15_HPP_INCLUDED
#define TEST15_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test15.hpp
// First created: 2015-10-16-10.28
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test15()
{
  std::cout << "Test 15: Atomic add" << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  hyperdex::attributes attrs { {"i", 10} };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return; }

  string space = "kv";
  string key = "some_key";

  auto ans = client.put_sync(space, key, attrs);

  if (!ans) { LOG(error) << "put error"; return; }
  else        LOG(info)  << "put done";

  attrs.set({ {"i", 2} });

  ans = client.atomic_add_sync(space, key, attrs);

  if (!ans) { LOG(error) << "atomic_add error"; return; }
  else        LOG(info)  << "atomic_add done";

  ans = client.get_sync(space, key, attrs);

  if (!ans) { LOG(error) << "get error"; return; }
  else        LOG(info)  << "get done";

  std::cout << "Result: " << attrs["i"] << std::endl;

} // test15

#endif // TEST15_HPP_INCLUDED
