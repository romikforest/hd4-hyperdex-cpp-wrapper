#ifndef TEST18_HPP_INCLUDED
#define TEST18_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test18.hpp
// First created: 2015-10-16-10.29
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test18()
{
  std::cout << "Test 18: Group and search operations" << std::endl;

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

  ans = client.search("kv", preds, attrs);
  std::cout << "Search request: " << ans << std::endl << std::endl;

  size_t j = 0;

  while(!(ans = client.loop()).search_done()) // && !ans.is_last_op()
  {
    std::cout << "Loop answer: " << ans << std::endl;
    std::cout << "Result: k: " << string{attrs["k"]}
              << " i: " << (int64_t)attrs["i"]
              << " v: " << (string)attrs["v"] << std::endl << std::endl;
    ++j;
  }

  std::cout << "Count of iterations " << j << std::endl;
  std::cout << "Last loop answer: " << ans << std::endl;

//  {
//    struct hyperdex_client* client = NULL;
//
//    struct hyperdex_client_attribute const* a;
//    size_t as;
//
//    int64_t op_id;
//    enum hyperdex_client_returncode op_status;
//
//    int64_t loop_id;
//    enum hyperdex_client_returncode loop_status;
//
//
//    client = hyperdex_client_create("127.0.0.1", 1982);
//
//    op_id = hyperdex_client_search(client, "kv", preds, preds.size(), &op_status, &a, &as);
//
//    std::cout << "REQ: op_id: " << op_id << " op status: " << op_status << std::endl;
//
//    const hyperdex_client_attribute ca { "v", "", 0, HYPERDATATYPE_STRING };
//
//    hyperdex_client_put(client, "kv", "some_key", 8, &ca, 1, &op_status);
//    hyperdex_client_put(client, "kv", "some_key", 8, &ca, 1, &op_status);
//    hyperdex_client_put(client, "kv", "some_key", 8, &ca, 1, &op_status);
//
//    for (int i = 0; i < 9; ++i)
//    {
//      std::cout << std::endl << "Iteration: " << i << std::endl;
//
//      loop_id = hyperdex_client_loop(client, -1, &loop_status);
//
//      std::cout << "LOOP: loop_id: " << loop_id << " loop status: " << loop_status << std::endl
//                << " op_id: " << op_id << " op status: " << op_status << std::endl;
//
//
//      hyperdex::attributes attrs{a, as};
//      std::cout << "Result: k: " << string{attrs["k"]}
//                << " i: " << (int64_t)attrs["i"]
//                << " v: " << (string)attrs["v"] << std::endl;
//    }
//
//
//    hyperdex_client_destroy_attrs(a, as);
//    hyperdex_client_destroy(client);
//  }

} // test18

#endif // TEST18_HPP_INCLUDED
