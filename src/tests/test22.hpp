#ifndef TEST22_HPP_INCLUDED
#define TEST22_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test22.hpp
// First created: 2015-10-16-10.31
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test22()
{
  std::cout << "Test 22: operations with map_attributes" << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  hyperdex::int_float_map mif{ { 1, 1.0 }, { 2, 2.0 }, { 3, 3.0 } };
  hyperdex::attributes attrs { {"mif", mif}, {"i", 10} };
  auto ans = client.put_sync("kv", "key1", attrs);
  std::cout << "Put: " << ans << std::endl;

//  ans = client.del_sync("kv", "key1");
//  std::cout << "Del: " << ans << std::endl;

//  hyperdex::predicates preds { {"i", 10, HYPERPREDICATE_EQUALS} };
//  hyperdex::hyperdex_group_count count = 0;
//  ans = client.group_del("kv", preds, count);
//  ans = client.loop();
//  std::cout << "Del: " << ans << std::endl;
//  std::cout << "Count: " << count << std::endl;

//  ans = client.get_partial_sync("kv", "key1", {{"mif"}}, attrs);
//  std::cout << "Get partial: " << ans << std::endl;
//  std::cout << attrs << std::endl;

  hyperdex::map_attributes mattrs { { "mif", 1, 2.0 }, { "mif", 2, 2.0 }, { "mif", 3, 2.0 } };
  ans = client.map_atomic_mul_sync("kv", "key1", mattrs);
  std::cout << "map_atomic_mul: " << ans << std::endl;

  hyperdex::predicates preds2 { {"i", 10, HYPERPREDICATE_EQUALS} };
  ans = client.cond_map_atomic_mul_sync("kv", "key1", preds2, mattrs);
  std::cout << "cond_map_atomic_mul: " << ans << std::endl;

  ans = client.get_partial_sync("kv", "key1", {{"mif"}}, attrs);
  std::cout << "get partial: " << ans << std::endl;

  std::cout << attrs << std::endl;

} // test22

#endif // TEST22_HPP_INCLUDED
