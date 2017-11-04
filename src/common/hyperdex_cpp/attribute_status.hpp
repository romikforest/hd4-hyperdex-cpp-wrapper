#ifndef HYPERDEX_ATTRIBUTE_STATUS_HPP_INCLUDED
#define HYPERDEX_ATTRIBUTE_STATUS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: attribute_status.hpp
// First created: 2015-10-22-10.41
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

// forest
#include <forest/vector.hpp>

namespace hyperdex
{
  enum class attribute_status { ok, not_found, empty, bad_request, not_changed };
  using attribute_status_vector = forest::vector<attribute_status>;
}

#endif // HYPERDEX_ATTRIBUTE_STATUS_HPP_INCLUDED
