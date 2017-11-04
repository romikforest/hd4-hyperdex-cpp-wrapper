#ifndef FOREST_MAP_HPP_INCLUDED
#define FOREST_MAP_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: map.hpp
// First created: 2015-09-12-02.09
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_MAP == FAV_STD

#include <map>

namespace forest
{
  using std::map;
  using std::swap;
}

#elif FOREST_ALTERNATIVE_FOR_MAP == FAV_BTREE

#include <forest/btree/btree_map.h>

namespace forest
{
  template <typename Key, typename Value,
            typename Compare = std::less<Key>,
            typename Alloc = std::allocator<std::pair<const Key, Value> >,
            int TargetNodeSize = 256>
  using map = btree::btree_map<Key, Value, Compare, Alloc, TargetNodeSize>;
}

#elif FOREST_ALTERNATIVE_FOR_MAP == FAV_BOOST_FLAT

#include <boost/container/flat_map.hpp>

namespace forest
{
  template<typename Key, typename T, typename Compare = std::less<Key>,
           typename Allocator = boost::container::new_allocator< std::pair< Key, T> > >
  using map = boost::container::flat_map<Key, T, Compare, Allocator>;
}

#else

#error "Wrong alternative for map class!"

#endif // FOREST_ALTERNATIVE_FOR_MAP


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::map;

#if FOREST_ALTERNATIVE_FOR_MAP == FAV_STD
using forest::swap;
#endif // FOREST_ALTERNATIVE_FOR_MAP

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // FOREST_MAP_HPP_INCLUDED
