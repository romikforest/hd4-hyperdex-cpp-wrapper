#ifndef FOREST_MULTISET_HPP_INCLUDED
#define FOREST_MULTISET_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: multiset.hpp
// First created: 2015-09-12-02.37
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_MULTISET == FAV_STD

#include <set>

namespace forest
{
  using std::multiset;
  using std::swap;
}

#elif FOREST_ALTERNATIVE_FOR_MULTISET == FAV_BTREE

#include <forest/btree/btree_set.h>

namespace forest
{
  template <typename Key,
            typename Compare = std::less<Key>,
            typename Alloc = std::allocator<Key>,
            int TargetNodeSize = 256>
  using multiset = btree::btree_multiset<Key, Compare, Alloc, TargetNodeSize>;
}

#elif FOREST_ALTERNATIVE_FOR_MULTISET == FAV_BOOST_FLAT

#include <boost/container/flat_set.hpp>

namespace forest
{
  template<typename Key, typename Compare = std::less<Key>,
           typename Allocator = boost::container::new_allocator<Key> >
  using multiset = boost::container::flat_multiset<Key, Compare, Allocator>;
}

#else

#error "Wrong alternative for multiset class!"

#endif // FOREST_ALTERNATIVE_FOR_MULTISET


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::multiset;

#if FOREST_ALTERNATIVE_FOR_MULTISET == FAV_STD
using forest::swap;
#endif // FOREST_ALTERNATIVE_FOR_MULTISET

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // FOREST_MULTISET_HPP_INCLUDED
