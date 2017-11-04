#ifndef TEST28_HPP_INCLUDED
#define TEST28_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test28.hpp
// First created: 2015-10-24-12.43
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/hyperdex.hpp>

void test28()
{
  std::cout << "Test 28: Admin. Dump config." << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  char const* config;

  auto ans = admin.dump_config_sync(config);

  if (!ans) { LOG(error) << "Config dumping failed."; std::cout << ans << std::endl; return; }

  std::cout << "Config info:" << std::endl << config << std::endl;
}

#endif // TEST28_HPP_INCLUDED
