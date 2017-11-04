#ifndef HYPERDEX_LINEAR_SEARCH_HPP_INCLUDED
#define HYPERDEX_LINEAR_SEARCH_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: linear_search.hpp
// First created: 2015-10-20-06.07
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

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/iterators_and_traits.hpp>

namespace hyperdex
{
  template<typename, typename> class c_string_traits;

  template<typename Attribute>
  inline
  std::enable_if_t<is_attribute<Attribute>::value ||
                   is_predicate<Attribute>::value ||
                   is_map_attribute<Attribute>::value,
                   size_t>
  linear_search(Attribute const* attrs, size_t size, char const* name) noexcept
  {
    if (name)
    {
      register auto ptr = attribute_filter(attrs);
      register auto end = ptr + size;
      for (; ptr < end; ++ptr)
      {
        if (ptr->attr && !strcmp(ptr->attr, name)) { size -= end - ptr; break; }
      }
    }
    return size;
  }

  template<typename Container>
  inline
  std::enable_if_t<is_attribute_container<Container>::value ||
                   is_predicate_container<Container>::value ||
                   is_map_attribute_container<Container>::value,
                   size_t>
  linear_search(Container const& attrs, char const* name) noexcept
  {
    size_t res = container_size(attrs);
    if (name)
    {
      auto ptr = begin(attrs);
      auto e   = end(attrs);
      for (; ptr != e; ++ptr)
      {
        if (attribute_filter(&*ptr)->attr && !strcmp(attribute_filter(&*ptr)->attr, name))
        { res -= std::distance(ptr,e); break; }
      }
    }
    return res;
  }

  template<typename Attribute, typename CString,
           template<typename, typename = void> typename CStringTraits = c_string_traits>
  inline
  std::enable_if_t<is_attribute<Attribute>::value ||
                   is_predicate<Attribute>::value ||
                   is_map_attribute<Attribute>::value,
                   size_t>
  linear_search(Attribute const* attrs, size_t size, CString const& name) noexcept
  {
    arena_vector arenas {};
    hyperdex_ds_returncode status;

    char const* str;
    if (!~CStringTraits<std::decay_t<CString>>
      ::serialize(arenas, status, str, name)) return size;

    return linear_search(attrs, size, str);
  }

  template<typename Container, typename CString,
           template<typename, typename = void> typename CStringTraits = c_string_traits>
  inline
  std::enable_if_t<is_attribute_container<Container>::value ||
                   is_predicate_container<Container>::value ||
                   is_map_attribute_container<Container>::value,
                   size_t>
  linear_search(Container const& attrs, CString const& name) noexcept
  {
    arena_vector arenas {};
    hyperdex_ds_returncode status;

    char const* str;
    if (!~CStringTraits<std::decay_t<CString>>
      ::serialize(arenas, status, str, name)) return container_size(attrs);

    return linear_search(attrs, str);
  }

} // namespace hyperdex


#endif // HYPERDEX_LINEAR_SEARCH_HPP_INCLUDED
