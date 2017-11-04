#ifndef FOREST_OPTIONAL_HPP_INCLUDED
#define FOREST_OPTIONAL_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: optional.hpp
// First created: 2015-09-12-01.35
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_OPTIONAL == FAV_EXPERIMENTAL

#include <experimental/optional>

namespace forest
{
  using std::experimental::optional;
  using std::experimental::nullopt_t;
  using std::experimental::in_place_t;
  using std::experimental::in_place;
  using std::experimental::swap;
  using std::experimental::bad_optional_access;
}

#elif FOREST_ALTERNATIVE_FOR_OPTIONAL == FAV_BOOST

#include <boost/optional.hpp>
#include <boost/optional/bad_optional_access.hpp>
#include <boost/optional/optional_fwd.hpp>


namespace forest
{
  using boost::optional;
  using nullopt_t = boost::none_t;
  using boost::bad_optional_access;
  using boost::swap;
}

#else

#error "Wrong alternative for optional class!"

#endif // FOREST_ALTERNATIVE_FOR_OPTIONAL

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::optional;
using forest::nullopt_t;
using forest::bad_optional_access;
using forest::swap;

#if FOREST_ALTERNATIVE_FOR_OPTIONAL != FAV_BOOST
using forest::in_place_t;
using forest::in_place;
#endif // FOREST_ALTERNATIVE_FOR_OPTIONAL

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_OPTIONAL_HPP_INCLUDED
