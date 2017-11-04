#ifndef HYPERDEX_HPP_INCLUDED
#define HYPERDEX_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: hyperdex.hpp
// First created: 2015-09-06-08.43
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifndef __cplusplus
  #error HyperDex c++ wrapper needs c++ compiler
#endif

#if __cplusplus <= 201103L
  #error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/admin.hpp>
#include <hyperdex_cpp/client.hpp>
#include <hyperdex_cpp/list.hpp>
#include <hyperdex_cpp/set.hpp>
#include <hyperdex_cpp/map.hpp>
#include <hyperdex_cpp/document.hpp>
#include <hyperdex_cpp/timestamp.hpp>
#include <hyperdex_cpp/serializer.hpp>
#include <hyperdex_cpp/c_string_traits.hpp>
#include <hyperdex_cpp/data_traits.hpp>
#include <hyperdex_cpp/map_traits.hpp>
#include <hyperdex_cpp/output.hpp>
#include <hyperdex_cpp/macaroons.hpp>

#include <hyperdex_cpp/conversions.hpp>
#include <hyperdex_cpp/fill_names.hpp>
#include <hyperdex_cpp/linear_search.hpp>

// Automatically included:
//#include <hyperdex_cpp/admin_answer.hpp>
//#include <hyperdex_cpp/client_answer.hpp>
//#include <hyperdex_cpp/attributes.hpp>
//#include <hyperdex_cpp/predicates.hpp>
//#include <hyperdex_cpp/map_attributes.hpp>
//#include <hyperdex_cpp/iterators_and_traits.hpp>
//#include <hyperdex_cpp/log.hpp>
//#include <hyperdex_cpp/attribute_status.hpp>

#endif // HYPERDEX_HPP_INCLUDED
