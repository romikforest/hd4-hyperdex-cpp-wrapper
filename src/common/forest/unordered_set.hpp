#ifndef FOREST_UNORDERED_SET_HPP_INCLUDED
#define FOREST_UNORDERED_SET_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: unordered_set.hpp
// First created: 2015-09-11-21.35
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_STD

#include <unordered_set>

namespace forest
{
  using std::unordered_set;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_BOOST

#include <boost/unordered_set.hpp>

namespace forest
{
  using boost::unordered_set;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_STD_TR1

#include <tr1/unordered_set>

namespace forest
{
  using std::tr1::unordered_set;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_BOOST_TR1

#include <boost/tr1/unordered_set.hpp>

namespace forest
{
  using std::tr1::unordered_set;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_MICROSOFT_TR1

#include <unordered_set>

namespace forest
{
  using std::tr1::unordered_set;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_SPARSE_HASH

#include <sparsehash/sparse_hash_set>

namespace forest
{
  template <typename Value,
            typename HashFcn = SPARSEHASH_HASH<Value>,  // defined in sparseconfig.h
            typename EqualKey = std::equal_to<Value>,
            typename Alloc = google::libc_allocator_with_realloc<Value> >
  using unordered_set = google::sparse_hash_set<Value, HashFcn, EqualKey, Alloc>;
}

#elif FOREST_ALTERNATIVE_FOR_UNORDERED_SET == FAV_DENSE_HASH

#include <sparsehash/dense_hash_set>

namespace forest
{
  template <typename Value,
            typename HashFcn = SPARSEHASH_HASH<Value>,   // defined in sparseconfig.h
            typename EqualKey = std::equal_to<Value>,
            typename Alloc = google::libc_allocator_with_realloc<Value> >
  using unordered_set = google::dense_hash_set<Value, HashFcn, EqualKey, Alloc>;
}

#else

#error "Wrong alternative for unordered_set class!"

#endif // FOREST_ALTERNATIVE_FOR_UNORDERED_SET


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::unordered_set;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_UNORDERED_SET_HPP_INCLUDED
