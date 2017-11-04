#ifndef TEST20_HPP_INCLUDED
#define TEST20_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test20.hpp
// First created: 2015-10-16-10.30
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test20()
{
  std::cout << "Test 20: " << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  // first
  hyperdex::attributes attrs { {"i", 10}, {"v", "set key1 i=10"} };
  auto ans = client.put_if_not_exist_sync("kv", "key1", attrs);
  std::cout << ans << std::endl;

  // second
  attrs.set({ {"i", 10}, {"v", "second"} });
  ans = client.put_if_not_exist_sync("kv", "key1", attrs);
  std::cout << ans << std::endl;

  client.get_partial_sync("kv", "key1", {{"v"}}, attrs);

  std::cout << attrs << std::endl;

} // test20

#endif // TEST20_HPP_INCLUDED
