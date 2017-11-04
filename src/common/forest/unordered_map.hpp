#ifndef FOREST_UNORDERED_MAP_HPP_INCLUDED
#define FOREST_UNORDERED_MAP_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: unordered_map.hpp
// First created: 2015-09-11-21.14
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_STD

#include <unordered_map>

namespace forest
{
  using std::unordered_map;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_BOOST

#include <boost/unordered_map.hpp>

namespace forest
{
  using boost::unordered_map;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_STD_TR1

#include <tr1/unordered_map>

namespace forest
{
  using std::tr1::unordered_map;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_BOOST_TR1

#include <boost/tr1/unordered_map.hpp>

namespace forest
{
  using std::tr1::unordered_map;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_MICROSOFT_TR1

#include <unordered_map>

namespace forest
{
  using std::tr1::unordered_map;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_SPARSE_HASH

#include <sparsehash/sparse_hash_map>

namespace forest
{
  template <typename Key, typename T,
            typename HashFcn = SPARSEHASH_HASH<Key>,   // defined in sparseconfig.h
            typename EqualKey = std::equal_to<Key>,
            typename Alloc = google::libc_allocator_with_realloc<std::pair<const Key, T> > >
  using unordered_map = google::sparse_hash_map<Key, T, HashFcn, EqualKey, Alloc>;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_MAP == FAV_DENSE_HASH

#include <sparsehash/dense_hash_map>

namespace forest
{
  template <typename Key, typename T,
            typename HashFcn = SPARSEHASH_HASH<Key>,   // defined in sparseconfig.h
            typename EqualKey = std::equal_to<Key>,
            typename Alloc = google::libc_allocator_with_realloc<std::pair<const Key, T> > >
  using unordered_map = google::dense_hash_map<Key, T, HashFcn, EqualKey, Alloc>;
}

#else

#error "Wrong alternative for unordered map class!"

#endif // FOREST_ALTERNATIVE_FOR_UNORDERED_MAP


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::unordered_map;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_UNORDERED_MAP_HPP_INCLUDED
