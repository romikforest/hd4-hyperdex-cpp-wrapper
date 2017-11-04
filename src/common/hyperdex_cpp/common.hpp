#ifndef HYPERDEX_COMMON_HPP_INCLUDED
#define HYPERDEX_COMMON_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: common.hpp
// First created: 2015-09-06-08.45
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifndef __cplusplus
  #error HyperDex c++ wrapper needs c++ compiler
#endif

#if __cplusplus <= 201103L
  #error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

static_assert(!~(-1), "Two's complement arithmetic is required in this code."
              " (or change all [!=] to [-1 ==], check and [~] operations)");

// C++
#include <utility>
#include <climits>

static_assert(CHAR_BIT == 8, "CHAR_BIT is not 8 on your machine. Library not tested for such environments.");

// hyperdex
#include <hyperdex/admin.h>
#include <hyperdex/client.h>
#include <hyperdex/datastructures.h>
#include <hyperdex/hyperspace_builder.h>

// forest
#include <forest/alternatives.hpp>
#include <forest/int128.hpp>
#include <forest/vector.hpp>
#include <forest/unique_ptr.hpp>

#ifndef DEFAULT_HYPERDEX_COORDINATOR
#define DEFAULT_HYPERDEX_COORDINATOR "127.0.0.1"
#endif // DEFAULT_HYPERDEX_COORDINATOR

#ifndef DEFAULT_HYPERDEX_PORT
#define DEFAULT_HYPERDEX_PORT 1982
#endif // DEFAULT_HYPERDEX_PORT

#ifndef STRINGIFICATION
#define STRINGIFICATION(x) #x
#endif // STRINGIFICATION

#ifndef DEFAULT_CONN_STR
#define DEFAULT_CONN_STR DEFAULT_HYPERDEX_COORDINATOR ":" STRINGIFICATION(DEFAULT_HYPERDEX_PORT)
#endif // DEFAULT_CONN_STR

#ifndef MAX_HYPERDEX_TUPLE_PARAMS
#define MAX_HYPERDEX_TUPLE_PARAMS 20
#endif // MAX_HYPERDEX_ATTRIBUTE_TUPLE_PARAMS

#define HYPERDEX_USE_GOOGLE_MACAROONS
#define HYPERDEX_USE_Z_INDEX

namespace hyperdex
{
  using forest::int128_t;
  using forest::uint128_t;

  // Data types

  #ifndef HYPERDEX_FLOAT_GUARD
  #define HYPERDEX_FLOAT_GUARD
  using hyperdex_float = double;
  #endif // HYPERDEX_FLOAT_GUARD

  #ifndef HYPERDEX_INT_GUARD
  #define HYPERDEX_INT_GUARD
  using hyperdex_int = int64_t;
  #endif // HYPERDEX_INT_GUARD

  #ifndef HYPERDEX_INT128_GUARD
  #define HYPERDEX_INT128_GUARD
  using hyperdex_int128 = uint128_t;
  #endif // HYPERDEX_INT128_GUARD

  #ifndef HYPERDEX_INT_RET_GUARD
  #define HYPERDEX_INT_RET_GUARD
  using hyperdex_int_ret = int;
  #endif // HYPERDEX_INT_RET_GUARD

  #ifndef HYPERDEX_TIMEOUT_GUARD
  #define HYPERDEX_TIMEOUT_GUARD
  using hyperdex_timeout = int;
  #endif // HYPERDEX_TIMEOUT_GUARD

  #ifndef HYPERDEX_OPID_GUARD
  #define HYPERDEX_OPID_GUARD
  using hyperdex_opid = int64_t;
  #endif // HYPERDEX_OPID_GUARD

  #ifndef HYPERDEX_GROUP_COUNT
  #define HYPERDEX_GROUP_COUNT
  using hyperdex_group_count = uint64_t;
  #endif // HYPERDEX_GROUP_COUNT

  using blob_pair = std::pair<char const*, size_t>;

  class hyperdex_blob_data {};

  struct arena_deleter
  {

    void operator () (hyperdex_ds_arena* arena) const noexcept
    {
      hyperdex_ds_arena_destroy(arena);
    }

  }; // struct arena_deleter

  using arena_vector = forest::vector<forest::unique_ptr<hyperdex_ds_arena, arena_deleter>>;

} // namespace hyperdex

#endif // HYPERDEX_COMMON_HPP_INCLUDED
