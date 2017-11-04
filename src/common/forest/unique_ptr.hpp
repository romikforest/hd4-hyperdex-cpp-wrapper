#ifndef FOREST_UNIQUE_PTR_HPP_INCLUDED
#define FOREST_UNIQUE_PTR_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: unique_ptr.hpp
// First created: 2015-09-11-20.24
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_UNIQUE_PTR == FAV_STD

#include <memory>

namespace forest
{

  using std::unique_ptr;
  using std::make_unique;

} // namespace forest

#elif FOREST_ALTERNATIVE_FOR_UNIQUE_PTR == FAV_BOOST

#include <boost/move/unique_ptr.hpp>
#include <boost/make_unique.hpp>

namespace forest
{

  using boost::movelib::unique_ptr;
  using boost::make_unique;

} // namespace forest

#elif FOREST_ALTERNATIVE_FOR_UNIQUE_PTR == FAV_INTERPROCESS

#include <boost/interprocess/smart_ptr/unique_ptr.hpp>

namespace forest
{

  using boost::interprocess::unique_ptr;

} // namespace forest

#else

#error "Wrong alternative for unique_ptr class!"

#endif // FOREST_ALTERNATIVE_FOR_UNIQUE_PTR

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::unique_ptr;

#if FOREST_ALTERNATIVE_FOR_UNIQUE_PTR != FAV_INTERPROCESS
using forest::make_unique;
#endif // FOREST_ALTERNATIVE_FOR_UNIQUE_PTR

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_UNIQUE_PTR_HPP_INCLUDED
