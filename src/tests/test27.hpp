#ifndef TEST27_HPP_INCLUDED
#define TEST27_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test27.hpp
// First created: 2015-10-24-12.24
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/hyperdex.hpp>

void test27()
{
  std::cout << "Test 27: Admin. Backup test." << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  char const* backups;

  auto ans = admin.backup_sync("/home/romik/Документы/Уроки/Информатика/Тест/пробы/hd2/backup/backup1", backups);

  if (!ans) { LOG(error) << "Backup failed."; std::cout << ans << std::endl; return; }

  std::cout << "Backup info:" << std::endl << backups << std::endl;
}

#endif // TEST27_HPP_INCLUDED
