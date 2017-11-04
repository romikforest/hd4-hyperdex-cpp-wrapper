#ifndef TEST17_HPP_INCLUDED
#define TEST17_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test17.hpp
// First created: 2015-10-16-10.29
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test17()
{
  std::cout << "Test 17: Cond put" << std::endl;

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

  attrs.set({ { "s", "af" }, { "i", 20 }, { "v", "se" } });
  //hyperdex::predicates preds { { "i", 20, HYPERPREDICATE_EQUALS } };
  hyperdex::predicates preds { { "i", 10, HYPERPREDICATE_EQUALS } };

  ans = client.cond_put_sync(space, key, preds, attrs);
  //ans = client.cond_put_sync(space, key, {{{"v", "ggg", 3, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS}}}, attrs);
//  ans = client.cond_put_sync(space, key,
//                             {{{"v", "ggg", 3, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS}}},
//                             {{{"v", "feihoa", 6, HYPERDATATYPE_STRING }}});

  if (!ans) { LOG(error) << "put error"; return; }
  else        LOG(info)  << "put done";

  hyperdex::name_vector nms {"v", "s", "i"};

  ans = client.get_partial_sync(space, key, nms, attrs);

  if (!ans) { LOG(error) << "get_partial error"; return; }
  else        LOG(info)  << "get_partial done";

  std::cout << "Result: " << attrs << std::endl;

} // test17

#endif // TEST17_HPP_INCLUDED
