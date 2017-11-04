#ifndef MOVE_HPP_INCLUDED
#define MOVE_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: move.hpp
// First created: 2015-09-13-17.34
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#if FOREST_ALTERNATIVE_FOR_MOVE == FAV_STD

// C++
#include <utility>

namespace forest
{
  using std::move;
  using std::forward;
}

#elif FOREST_ALTERNATIVE_FOR_MOVE == FAV_BOOST

#include <boost/move/utility.hpp>

namespace forest
{
  using boost::move;
  using boost::forward;;
}

#else

#error "Wrong alternative for move!"

#endif // FOREST_ALTERNATIVE_FOR_VECTOR


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::move;
using forest::forward;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // MOVE_HPP_INCLUDED
