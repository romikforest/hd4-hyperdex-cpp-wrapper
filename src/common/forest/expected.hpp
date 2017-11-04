#ifndef FOREST_EXPECTED_HPP_INCLUDED
#define FOREST_EXPECTED_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: expected.hpp
// First created: 2015-09-12-01.51
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_EXPECTED == FAV_PROTO

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Weffc++"

#include <boost/expected/expected.hpp>
#include <boost/expected/expected_monad.hpp>
#include <boost/expected/optional_monad.hpp>
#include <boost/expected/conversions/expected_to_optional.hpp>
#include <boost/expected/conversions/expected_to_future.hpp>

#pragma GCC diagnostic pop

namespace forest
{
  using boost::expected;
  using boost::make_expected;
  using boost::make_unexpected;
}

#else

#error "Wrong alternative for expected class!"

#endif // FOREST_ALTERNATIVE_FOR_EXPECTED


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::expected;
using forest::make_expected;
using forest::make_unexpected;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_EXPECTED_HPP_INCLUDED
