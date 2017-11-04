#ifndef TEST14_HPP_INCLUDED
#define TEST14_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test14.hpp
// First created: 2015-10-16-10.28
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test14()
{
  std::cout << "Test 14: Many queries" << std::endl;

  using forest::string;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

  hyperdex::attributes attrs { {"v", "Hello, Piter!"} };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return; }

  auto ans1 = client.put("kv", "some_key1", attrs);
  auto ans2 = client.put("kv", "some_key2", attrs);
  auto ans3 = client.put("kv", "some_key3", attrs);
  auto ans4 = client.put("kv", "some_key4", attrs);

  std::cout << "Answers for put before loop: "
            << ans1 << std::endl
            << ans2 << std::endl
            << ans3 << std::endl
            << ans4 << std::endl;

  if (!client.loop_all()) { LOG(error) << "put error"; return; }
  else                      LOG(info)  << "put done";

  std::cout << "Answers for put after loop: "
            << ans1 << std::endl
            << ans2 << std::endl
            << ans3 << std::endl
            << ans4 << std::endl;

  hyperdex::attributes attrs1, attrs2, attrs3, attrs4;

  ans1 = client.get("kv", "some_key1", attrs1);
  ans2 = client.get("kv", "some_key2", attrs2);
  ans3 = client.get("kv", "some_key3", attrs3);
  ans4 = client.get("kv", "some_key4", attrs4);

  std::cout << "Answers for get before loop: "
            << ans1 << std::endl
            << ans2 << std::endl
            << ans3 << std::endl
            << ans4 << std::endl;


  if (!client.loop_all()) { LOG(error) << "get error"; return; }
  else                      LOG(info)  << "get done";

  std::cout << "Answers for get after loop: "
            << ans1 << std::endl
            << ans2 << std::endl
            << ans3 << std::endl
            << ans4 << std::endl;

  std::cout << "Results: "
            << attrs1["v"] << std::endl
            << attrs2["v"] << std::endl
            << attrs3["v"] << std::endl
            << attrs4["v"] << std::endl;

} // test14

#endif // TEST14_HPP_INCLUDED
