#ifndef FOREST_SET_HPP_INCLUDED
#define FOREST_SET_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: set.hpp
// First created: 2015-09-12-02.10
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_SET == FAV_STD

#include <set>

namespace forest
{
  using std::set;
  using std::swap;
}

#elif FOREST_ALTERNATIVE_FOR_SET == FAV_BTREE

#include <forest/btree/btree_set.h>

namespace forest
{
  template <typename Key,
            typename Compare = std::less<Key>,
            typename Alloc = std::allocator<Key>,
            int TargetNodeSize = 256>
  using set = btree::btree_set<Key, Compare, Alloc, TargetNodeSize>;
}

#elif FOREST_ALTERNATIVE_FOR_SET == FAV_BOOST_FLAT

#include <boost/container/flat_set.hpp>

namespace forest
{
  template<typename Key, typename Compare = std::less<Key>,
           typename Allocator = boost::container::new_allocator<Key> >
  using set = boost::container::flat_set<Key, Compare, Allocator>;
}

#else

#error "Wrong alternative for set class!"

#endif // FOREST_ALTERNATIVE_FOR_SET


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::set;
#if FOREST_ALTERNATIVE_FOR_SET == FAV_STD
using forest::swap;
#endif // FOREST_ALTERNATIVE_FOR_SET

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // FOREST_SET_HPP_INCLUDED
