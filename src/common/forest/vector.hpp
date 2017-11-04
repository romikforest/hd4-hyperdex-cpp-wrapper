#ifndef FOREST_VECTOR_HPP_INCLUDED
#define FOREST_VECTOR_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: vector.hpp
// First created: 2015-09-11-21.49
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_VECTOR == FAV_STD

#include <vector>

namespace forest
{
  using std::vector;
}

#elif FOREST_ALTERNATIVE_FOR_VECTOR == FAV_BOOST

#include <boost/container/vector.hpp>

namespace forest
{
  using boost::container::vector;
}

#else

#error "Wrong alternative for vector class!"

#endif // FOREST_ALTERNATIVE_FOR_VECTOR


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::vector;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_VECTOR_HPP_INCLUDED
