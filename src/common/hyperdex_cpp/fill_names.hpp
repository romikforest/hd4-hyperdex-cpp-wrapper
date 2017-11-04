#ifndef HYPERDEX_FILL_NAMES_HPP_INCLUDED
#define HYPERDEX_FILL_NAMES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: fill_names.hpp
// First created: 2015-10-18-11.50
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

  template<typename Attribute>
  inline
  std::enable_if_t< is_non_cv<Attribute>::value &&
                   (is_attribute<Attribute>::value ||
                    is_predicate<Attribute>::value ||
                    is_map_attribute<Attribute>::value)>
  null_names(Attribute* attrs, size_t attrs_size) noexcept
  {
    register auto ptr = attribute_filter(attrs);
    register auto end = ptr + attrs_size;
    for (; ptr != end; ++ptr) ptr->attr = nullptr;
  }

  template<typename Container>
  inline
  std::enable_if_t< is_container_of_non_cv<Container>::value &&
                   (is_attribute_container<Container>::value ||
                    is_predicate_container<Container>::value ||
                    is_map_attribute_container<Container>::value)>
  null_names(Container& attrs) noexcept
  {
    auto ptr = begin(attrs);
    auto   e = end(attrs);

    for (; ptr != e; ++ptr) attribute_filter(&*ptr)->attr = nullptr;
  }

  template<typename Attribute>
  inline
  std::enable_if_t< is_non_cv<Attribute>::value &&
                   (is_attribute<Attribute>::value ||
                    is_predicate<Attribute>::value ||
                    is_map_attribute<Attribute>::value),
                   bool>
  fill_names(Attribute* attrs,
             size_t attrs_size,
             char const* const* names,
             size_t names_size
            ) noexcept
  {
    register auto ptr = attribute_filter(attrs);
    register auto end = ptr + std::min(attrs_size, names_size);
    for (; ptr != end; ++ptr, ++names) ptr->attr = *names;
    return attrs_size == names_size;
  }

  template<size_t N, typename Attribute>
  inline
  std::enable_if_t< is_non_cv<Attribute>::value &&
                   (is_attribute<Attribute>::value ||
                    is_predicate<Attribute>::value ||
                    is_map_attribute<Attribute>::value),
                   bool>
  fill_names(Attribute* attrs,
             size_t attrs_size,
             name_c_array<N> const& names
            ) noexcept
  {
    register  auto attr_ptr = attribute_filter(attrs);
    register  auto end = attr_ptr + std::min(attrs_size, container_size(names));
    register  auto name_ptr = begin(names);

    for (; attr_ptr != end; ++name_ptr, ++attr_ptr) attr_ptr->attr = *name_ptr;

    return attrs_size == container_size(names);
  }

  template<typename Attribute, typename NameContainer>
  inline
  std::enable_if_t< is_non_cv<Attribute>::value &&
                   (is_attribute<Attribute>::value ||
                    is_predicate<Attribute>::value ||
                    is_map_attribute<Attribute>::value)&&
                   is_name_container<NameContainer>::value,
                   bool>
  fill_names(Attribute* attrs,
             size_t attrs_size,
             NameContainer const& names
            ) noexcept
  {
    register  auto attr_ptr = attribute_filter(attrs);
    register  auto end = attr_ptr + std::min(attrs_size, container_size(names));
    register  auto name_ptr = begin(names);

    for (; attr_ptr != end; ++name_ptr, ++attr_ptr) attr_ptr->attr = *name_ptr;

    return attrs_size == container_size(names);
  }

  template<typename Container>
  inline
  std::enable_if_t< is_container_of_non_cv<Container>::value &&
                   (is_attribute_container<Container>::value ||
                    is_predicate_container<Container>::value ||
                    is_map_attribute_container<Container>::value),
                   bool>
  fill_names(Container& attrs,
             char const* const* names,
             size_t names_size) noexcept
  {
    auto ptr = begin(attrs);
    auto end = names + std::min(container_size(attrs), names_size);

    for (; names != end; ++ptr, ++names) attribute_filter(&*ptr)->attr = *names;

    return container_size(attrs) == names_size;
  }

  template<size_t N, typename Container>
  inline
  std::enable_if_t< is_container_of_non_cv<Container>::value &&
                   (is_attribute_container<Container>::value ||
                    is_predicate_container<Container>::value ||
                    is_map_attribute_container<Container>::value),
                   bool>
  fill_names(Container& attrs,
             name_c_array<N> const& names
            ) noexcept
  {
    auto attr_ptr = begin(attrs);
    auto name_ptr = begin(names);
    for (register auto size = std::min(container_size(attrs), container_size(names));
         size; ++attr_ptr, ++name_ptr, --size) attribute_filter(&*attr_ptr)->attr = *name_ptr;
    return container_size(attrs) == container_size(names);
  }

  template<typename Container, typename NameContainer>
  inline
  std::enable_if_t< is_container_of_non_cv<Container>::value &&
                   (is_attribute_container<Container>::value ||
                    is_predicate_container<Container>::value ||
                    is_map_attribute_container<Container>::value) &&
                   is_name_container<NameContainer>::value,
                   bool>
  fill_names(Container& attrs,
             NameContainer const& names
            ) noexcept
  {
    auto attr_ptr = begin(attrs);
    auto name_ptr = begin(names);
    for (register auto size = std::min(container_size(attrs), container_size(names));
         size; ++attr_ptr, ++name_ptr, --size) attribute_filter(&*attr_ptr)->attr = *name_ptr;
    return container_size(attrs) == container_size(names);
  }

} // namespace hyperdex


#endif // HYPERDEX_FILL_NAMES_HPP_INCLUDED
