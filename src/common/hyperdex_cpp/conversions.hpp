#ifndef HYPERDEX_CONVERSIONS_HPP_INCLUDED
#define HYPERDEX_CONVERSIONS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: conversions.hpp
// First created: 2015-10-18-12.03
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

// hyperdex
#include <hyperdex_cpp/base_types.hpp>

namespace hyperdex
{

  inline
  hyperdex_client_attribute
  attribute_from_predicate(hyperdex_client_attribute_check const& pred) noexcept
  {
    return { pred.attr, pred.value, pred.value_sz, pred.datatype };
  }

  inline
  an_attribute
  an_attribute_from_predicate(hyperdex_client_attribute_check const& pred) noexcept
  {
    return { pred.attr, pred.value, pred.value_sz, pred.datatype };
  }

  inline
  hyperdex_client_attribute_check
  predicate_from_attribute(hyperdex_client_attribute const& a,
                           hyperpredicate pred = HYPERPREDICATE_EQUALS) noexcept
  {
    return { a.attr, a.value, a.value_sz, a.datatype, pred };
  }

  inline
  hyperdex_client_attribute_check
  predicate_from_attribute(an_attribute const& a,
                           hyperpredicate pred = HYPERPREDICATE_EQUALS) noexcept
  {
    return { a.a.attr, a.a.value, a.a.value_sz, a.a.datatype, pred };
  }

  inline
  hyperdex_client_attribute_check
  predicate_from_attribute(char const* name,
                           hyperdex_client_attribute const& a,
                           hyperpredicate pred = HYPERPREDICATE_EQUALS) noexcept
  {
    return { name, a.value, a.value_sz, a.datatype, pred };
  }

  inline
  hyperdex_client_attribute_check
  predicate_from_attribute(char const* name,
                           an_attribute const& a,
                           hyperpredicate pred = HYPERPREDICATE_EQUALS) noexcept
  {
    return { name, a.a.value, a.a.value_sz, a.a.datatype, pred };
  }

  inline
  hyperdex_client_attribute
  attribute_from_map_key(hyperdex_client_map_attribute const& a) noexcept
  {
    return { a.attr, a.map_key, a.map_key_sz, a.map_key_datatype };
  }

  inline
  an_attribute
  an_attribute_from_map_key(hyperdex_client_map_attribute const& a) noexcept
  {
    return { a.attr, a.map_key, a.map_key_sz, a.map_key_datatype };
  }

  inline
  hyperdex_client_attribute
  attribute_from_map_key(char const* name, hyperdex_client_map_attribute const& a) noexcept
  {
    return { name, a.map_key, a.map_key_sz, a.map_key_datatype };
  }

  inline
  an_attribute
  an_attribute_from_map_key(char const* name, hyperdex_client_map_attribute const& a) noexcept
  {
    return { name, a.map_key, a.map_key_sz, a.map_key_datatype };
  }

  inline
  hyperdex_client_attribute
  attribute_from_map_value(hyperdex_client_map_attribute const& a) noexcept
  {
    return { a.attr, a.value, a.value_sz, a.value_datatype };
  }

  inline
  an_attribute
  an_attribute_from_map_value(hyperdex_client_map_attribute const& a) noexcept
  {
    return { a.attr, a.value, a.value_sz, a.value_datatype };
  }

  inline
  hyperdex_client_attribute
  attribute_from_map_value(char const* name, hyperdex_client_map_attribute const& a) noexcept
  {
    return { name, a.value, a.value_sz, a.value_datatype };
  }

  inline
  an_attribute
  an_attribute_from_map_value(char const* name, hyperdex_client_map_attribute const& a) noexcept
  {
    return { name, a.value, a.value_sz, a.value_datatype };
  }

  inline
  hyperdex_client_map_attribute
  map_attribute_from_attributes(char const* name,
                                hyperdex_client_attribute const& a1,
                                hyperdex_client_attribute const& a2) noexcept
  {
    return { name,
             a1.value, a1.value_sz, a1.datatype,
             a2.value, a2.value_sz, a2.datatype };
  }

} // namespace hyperdex

#endif // HYPERDEX_CONVERSIONS_HPP_INCLUDED
