#ifndef HYPERDEX_C_STRING_TRAITS_HPP_INCLUDED
#define HYPERDEX_C_STRING_TRAITS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: c_string_traits.hpp
// First created: 2015-10-18-11.13
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#if __cplusplus <= 201103L
  #error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

#if __cplusplus <= 201103L
  #error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

// C++
#include <cstddef>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>

// forest
#include <forest/alternatives.hpp>
#include <forest/string.hpp>

#if FOREST_ALTERNATIVE_FOR_LOCALE == FAV_BOOST
#ifdef FOREST_USE_WSTRINGS
#include <forest/locale.hpp>
#endif // FOREST_USE_WSTRINGS
#endif // FOREST_ALTERNATIVE_FOR_LOCALE

namespace hyperdex
{
  using forest::string;

  /////////////////////////////////////////////////////////////
  // C-string traits

  // template:
  template <typename CStringType, typename Enable = void>
  class c_string_traits
  {
    static_assert(!sizeof(CStringType), "Unsupported type for c-string argument.");
  };

  // C-string traits specialization for std::nullptr_t
  template<> class c_string_traits<std::nullptr_t>
  {
    public:

      static constexpr size_t const aren_max = 0;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        std::nullptr_t)
                                        noexcept
      {
        res_str = nullptr;
        return 0;
      }

  }; // C-string traits specialization for std::nullptr_t

  // C-string traits specialization for char const*
  template<> class c_string_traits<char const*>
  {
    public:

      static constexpr size_t const aren_max = 0;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        char const*  c_string)
                                        noexcept
      {
        res_str = c_string;
        return 0;
      }

  }; // C-string traits specialization for char const*

  // C-string traits specialization for char*
  template<> class c_string_traits<char*>
  {
    public:

      static constexpr size_t const aren_max = 0;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        char const*  c_string)
                                        noexcept
      {
        res_str = c_string;
        return 0;
      }

  }; // C-string traits specialization for char*

  // C-string traits specialization for string
  template<> class c_string_traits<string>
  {
    public:

      static constexpr size_t const aren_max = 1;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        string const& c_string)
                                        noexcept
      {
        res_str = c_string.c_str();
        return 0;
      }

      // just don't forward, if you don't want to copy
      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        string&& c_string)
                                        noexcept
      {
        res_str = nullptr;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }

        arenas.emplace_back(arena, arena_deleter{});

        size_t sz;

        if (!~hyperdex_ds_copy_string(arena, c_string.c_str(), c_string.size() + 1, &status, &res_str, &sz))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

  }; // C-string traits specialization for string

  #if FOREST_ALTERNATIVE_FOR_LOCALE == FAV_BOOST
  #ifdef FOREST_USE_WSTRINGS

  using forest::wstring;

  // C-string traits specialization for wstring
  template<> class c_string_traits<wstring>
  {
    public:

      static constexpr size_t const aren_max = c_string_traits<string>::aren_max;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        wstring const& c_string)
                                        noexcept
      {
        return c_string_traits<string>::serialize(arenas, status, res_str,
                                                  forest::locale::conv::utf_to_utf<char>(c_string.c_str()));
      }

  }; // C-string traits specialization for wstring

  // C-string traits specialization for wchar_t const*
  template<> class c_string_traits<wchar_t const*>
  {
    public:

      static constexpr size_t const aren_max = c_string_traits<string>::aren_max;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        wchar_t const*  c_string)
                                        noexcept
      {
        return c_string_traits<string>::serialize(arenas, status, res_str,
                                                  forest::locale::conv::utf_to_utf<char>(c_string));
      }

  }; // C-string traits specialization for wchar_t const*

  #endif // FOREST_USE_WSTRINGS
  #endif // FOREST_ALTERNATIVE_FOR_LOCALE

} // namespace hyperdex

#endif // HYPERDEX_C_STRING_TRAITS_HPP_INCLUDED
