#ifndef FOREST_MULTIMAP_HPP_INCLUDED
#define FOREST_MULTIMAP_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: multimap.hpp
// First created: 2015-09-12-02.37
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_MULTIMAP == FAV_STD

#include <map>

namespace forest
{
  using std::multimap;
  using std::swap;
}

#elif FOREST_ALTERNATIVE_FOR_MULTIMAP == FAV_BTREE

#include <forest/btree/btree_map.h>

namespace forest
{
  template <typename Key, typename Value,
            typename Compare = std::less<Key>,
            typename Alloc = std::allocator<std::pair<const Key, Value> >,
            int TargetNodeSize = 256>
  using multimap = btree::btree_multimap<Key, Value, Compare, Alloc, TargetNodeSize>;;
}

#elif FOREST_ALTERNATIVE_FOR_MULTIMAP == FAV_BOOST_FLAT

#include <boost/container/flat_map.hpp>

namespace forest
{
  template<typename Key, typename T, typename Compare = std::less<Key>,
           typename Allocator = boost::container::new_allocator< std::pair< Key, T> > >
  using multimap = boost::container::flat_multimap<Key, T, Compare, Allocator>;
}


#else

#error "Wrong alternative for multimap class!"

#endif // FOREST_ALTERNATIVE_FOR_MULTI_MAP


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::multimap;

#if FOREST_ALTERNATIVE_FOR_MULTIMAP == FAV_STD
using forest::swap;
#endif // FOREST_ALTERNATIVE_FOR_MULTIMAP

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_MULTIMAP_HPP_INCLUDED
