#ifndef TEST23_HPP_INCLUDED
#define TEST23_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test23.hpp
// First created: 2015-10-16-10.31
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

void test23()
{
  std::cout << "Test 23: Admin. working with spaces." << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  char const* description = "space new key int k attributes v, s, int i create 8 partitions tolerate 2 failures";

  bool res = admin.validate_space(description);

  if (res)   LOG(info)  << "Space validated successfully.";
  else     { LOG(error) << "Incorrect space description."; return; }

  auto ans = admin.add_space_sync(description);

  if (ans)   LOG(info)  << "Space added.";
  else     { LOG(error) << "Space is not added."; return; }

  char const* data;

  ans = admin.list_spaces_sync(data);

  if (!ans) { LOG(error) << "Can't list spaces."; return; }

  std::cout << "Spaces are:" << std::endl << data << std::endl;

  ans = admin.list_indices_sync("new", data);

  if (!ans) { LOG(error) << "Can't list indices."; return; }

  std::cout << "Indices are:" << std::endl << data << std::endl;

  ans = admin.list_subspaces_sync("new", data);

  if (!ans) { LOG(error) << "Can't list subspaces."; return; }

  std::cout << "Subspaces are:" << std::endl << data << std::endl;

  ans = admin.rm_space_sync("new");

  if (ans)   LOG(info)  << "Space removed.";
  else     { LOG(error) << "Space can't be removed."; return; }

} // test23

#endif // TEST23_HPP_INCLUDED
