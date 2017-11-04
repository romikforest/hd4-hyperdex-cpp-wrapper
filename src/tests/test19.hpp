#ifndef TEST19_HPP_INCLUDED
#define TEST19_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test19.hpp
// First created: 2015-10-16-10.30
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test19()
{
  std::cout << "Test 19: Search describe" << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  hyperdex::attributes attrs { {"i", 10}, {"v", "set key1 i=10"} };
  client.put_sync("kv", "key1", attrs);

  attrs.set({ {"i", 10}, {"v", "set key2 i=10"} });
  client.put_sync("kv", "key2", attrs);

  attrs.set({ {"i", 10}, {"v", "set key3 i=10"} });
  client.put_sync("kv", "key3", attrs);

  attrs.set({ {"i", 10}, {"v", "set key4 i=10"} });
  client.put_sync("kv", "key4", attrs);

  attrs.set({ {"i", 55}, {"v", "set key5 i=55"} });
  client.put_sync("kv", "key5", attrs);

  attrs.set({ {"i", 55}, {"v", "set key6 i=55"} });
  client.put_sync("kv", "key6", attrs);

  attrs.set({ {"i", 55}, {"v", "set key7 i=55"} });
  client.put_sync("kv", "key7", attrs);

  attrs.set({ {"i", 55}, {"v", "set key8 i=55"} });
  client.put_sync("kv", "key8", attrs);

  hyperdex::predicates preds { { "i", 55, HYPERPREDICATE_EQUALS } };
  attrs.set({ { "i", 22 } });
  hyperdex::hyperdex_group_count count { 0 };

  client.group_put_sync("kv", preds, attrs, count);
  std::cout << "Put done for " << count << " keys." << std::endl;

  count = 0;

  preds.set({ { "i", 22, HYPERPREDICATE_EQUALS } });
  auto ans = client.count_sync("kv", preds, count);

  if (!ans) std::cout << ans << std::endl;

  std::cout << "Count shows " << count << " keys." << std::endl;


  char const* descr = nullptr;
  ans = client.search_describe_sync("kv", preds, descr);

  std::cout << "Description: " << descr << std::endl;

  std::cout << "Answer: " << ans << std::endl;

} // test19

#endif // TEST19_HPP_INCLUDED
