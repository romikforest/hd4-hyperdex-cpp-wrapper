#ifndef TEST21_HPP_INCLUDED
#define TEST21_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test21.hpp
// First created: 2015-10-16-10.31
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test21()
{
  std::cout << "Test 21: cond_put_or_create_sync" << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  // first
  hyperdex::attributes attrs { {"i", 10}, {"v", "set key1 i=10"} };
  auto ans = client.put_if_not_exist_sync("kv", "key1", attrs);
  std::cout << ans << std::endl;

  // second
  attrs.set({ {"i", 10}, {"v", "second"} });
  hyperdex::predicates preds { {"i", 10, HYPERPREDICATE_EQUALS} };
  ans = client.cond_put_or_create_sync("kv", "key1", preds, attrs);
  std::cout << ans << std::endl;

  client.get_partial_sync("kv", "key1", {{"v"}}, attrs);

  std::cout << attrs << std::endl;

} // test21

#endif // TEST21_HPP_INCLUDED
