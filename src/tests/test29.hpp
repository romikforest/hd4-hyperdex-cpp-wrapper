#ifndef TEST29_HPP_INCLUDED
#define TEST29_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test29.hpp
// First created: 2015-10-24-13.06
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/hyperdex.hpp>

void test29()
{

  std::cout << "Test 29: Admin. Indices." << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  auto ans = admin.add_index("kv", "i");
  std::cout << ans << std::endl;

  ans = admin.loop();
  std::cout << ans << std::endl;

  if (!ans) { LOG(error) << "Unable to add index."; std::cout << ans << std::endl; return; }

  char const* data;

  ans = admin.list_indices_sync("kv", data);

  if (!ans) { LOG(error) << "Can't list indices."; return; }

  std::cout << "Indices are:" << std::endl << data << std::endl;

  ans = admin.rm_index(19);
  if (!ans) { LOG(error) << "Can't remove index."; return; }
  LOG(info) << "Index removed";
}

#endif // TEST29_HPP_INCLUDED
