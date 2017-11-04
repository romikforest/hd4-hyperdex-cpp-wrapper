#ifndef TEST24_HPP_INCLUDED
#define TEST24_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test24.hpp
// First created: 2015-10-16-10.32
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test24()
{
  std::cout << "Test 24: Perf counters" << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  hyperdex_admin_perf_counter pc;

  auto ans = admin.enable_perf_counters(pc);

  for (int i = 0; i < 70; ++i)
  {
    ans = admin.loop();
    if (!ans) { LOG(error) << "Can't enable perf counters"; return; }

    std::cout << "Item " << i << ":" << std::endl << pc << std::endl;
  }

  admin.disable_perf_counters();

  if (!ans) { LOG(error) << "Can't disable perf counters"; return; }

//  {
//    hyperdex_admin_returncode status;
//    hyperdex_admin_returncode lstatus;
//    hyperdex_admin_perf_counter pc;
//
//    struct hyperdex_admin* admin = hyperdex_admin_create("127.0.0.1", 1982);
//    int64_t op_id = hyperdex_admin_enable_perf_counters(admin, &status, &pc);
//    int64_t loop_id = hyperdex_admin_loop(admin, -1, &lstatus);
//
//    std::cout << "op id: " << op_id << " status: " << status << std::endl
//              << "loop id: " << loop_id << " lstatus: " << lstatus << std::endl
//              << " pc: " << pc << std::endl;
//
//    for (int i = 0; i < 70; ++i)
//    {
//      std::cout << "Item " << i << ":" << std::endl;
//      loop_id = hyperdex_admin_loop(admin, -1, &lstatus);
//
//      std::cout << "op id: " << op_id << " status: " << status << std::endl
//                << "loop id: " << loop_id << " lstatus: " << lstatus << std::endl
//                << " pc: " << pc << std::endl;
//    }
//
//    hyperdex_admin_disable_perf_counters(admin);
//    hyperdex_admin_destroy(admin);
//  }

} // test24

#endif // TEST24_HPP_INCLUDED
