#ifndef HYPERDEX_BASE_TYPES_HPP_INCLUDED
#define HYPERDEX_BASE_TYPES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: base_types.hpp
// First created: 2015-10-18-07.23
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

// C++
#include <type_traits>

// boost
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>

// forest
#include <forest/array.hpp>
#include <forest/tuple.hpp>
#include <forest/vector.hpp>
#include <forest/string.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>

namespace hyperdex
{
  using forest::array;
  using forest::tuple;
  using forest::vector;

  template<typename T, size_t N>
  using c_array = T[N];

  template<typename T, size_t N>
  using cpp_array = forest::array<T,N>;

  template<typename, typename> class complex_data_traits;

  class an_attribute final
  {
    public:

      hyperdex_client_attribute a;

      constexpr an_attribute() noexcept: a { nullptr, nullptr, 0, HYPERDATATYPE_STRING } {};

      constexpr an_attribute(hyperdex_client_attribute const& attr) noexcept: a { attr } {}

      constexpr an_attribute(an_attribute const& attr) noexcept: a { attr.a }  {}

      constexpr an_attribute(char const* attr,
                             char const* value,
                             size_t value_sz,
                             hyperdatatype type = HYPERDATATYPE_STRING)
                             noexcept
      : a { attr, value, value_sz, type }
      {}

      constexpr char const*   name() const noexcept { return  a.attr;     }
      constexpr char const*  value() const noexcept { return  a.value;    }
      constexpr size_t    value_sz() const noexcept { return  a.value_sz; }
      forest::string  value_string() const          { return  forest::string{ a.value, a.value_sz }; }
      constexpr hyperdatatype type() const noexcept { return  a.datatype; }
      constexpr bool         empty() const noexcept { return !a.value_sz; }

      template<typename T,
               template<typename, typename = void> typename ComplexTraits = complex_data_traits,
               typename = std::enable_if_t<!std::is_same<std::decay_t<T>, blob_pair>::value>>
      explicit operator T() const
      {
        return ComplexTraits<std::decay_t<T>>::deserialize(a);
      }

      constexpr operator hyperdex_client_attribute() const noexcept
      {
        return a;
      }

  }; // class an_attribute

  static_assert(sizeof(hyperdex_client_attribute) == sizeof(hyperdex::an_attribute),
                "Size of an_attribute must be equal to size of hyperdex_client_attribute."
                " Class an_attribute must not inherit from other classes or have virtual members.");

  static_assert(sizeof(array<hyperdex_client_attribute, 10>) == sizeof(hyperdex_client_attribute[10]),
                  "Template class `array` must save data as a common array.");

  static_assert(sizeof(tuple<hyperdex_client_attribute, hyperdex_client_attribute,
                             hyperdex_client_attribute, hyperdex_client_attribute,
                             hyperdex_client_attribute>) == sizeof(hyperdex_client_attribute[5]),
                  "Template class `tuple` must save data as a common array in the reverse order.");

  static constexpr an_attribute const bad_attribute_element { nullptr, nullptr, 0, HYPERDATATYPE_STRING };

  template<size_t N>
  using an_attribute_array  = array<an_attribute, N>;

  using an_attribute_vector = vector<an_attribute>;

  template<size_t N>
  using an_attribute_c_array  = an_attribute[N];

  template<size_t N>
  using attribute_array  = array<hyperdex_client_attribute, N>;

  using attribute_vector = vector<hyperdex_client_attribute>;

  template<size_t N>
  using attribute_c_array  = hyperdex_client_attribute[N];

  template<size_t N>
  using predicate_array  = array<hyperdex_client_attribute_check, N>;

  using predicate_vector = vector<hyperdex_client_attribute_check>;

  template<size_t N>
  using predicate_c_array  = hyperdex_client_attribute_check[N];

  template<size_t N>
  using map_attribute_array  = array<hyperdex_client_map_attribute, N>;

  using map_attribute_vector = vector<hyperdex_client_map_attribute>;

  template<size_t N>
  using map_attribute_c_array  = hyperdex_client_map_attribute[N];

  template<size_t N>
  using name_array = array<char const*, N>;

  using name_vector = vector<char const*>;

  template<size_t N>
  using name_c_array = char const*[N];

  using string_vector = vector<forest::string>;

  #define HYPERDEX_SHOWTEXT(z, i, text) text

  #define ATTRIBUTE_TUPLE(Z, N, _) \
    using BOOST_PP_CAT(attribute_tuple_,N) = \
      tuple<BOOST_PP_ENUM(N,HYPERDEX_SHOWTEXT,hyperdex_client_attribute)>; \
      /**/

  BOOST_PP_REPEAT(BOOST_PP_INC(MAX_HYPERDEX_TUPLE_PARAMS), ATTRIBUTE_TUPLE, _)

  #undef ATTRIBUTE_TUPLE

  #define AN_ATTRIBUTE_TUPLE(Z, N, _) \
    using BOOST_PP_CAT(an_attribute_tuple_,N) = \
      tuple<BOOST_PP_ENUM(N,HYPERDEX_SHOWTEXT,an_attribute)>; \
      /**/

  BOOST_PP_REPEAT(BOOST_PP_INC(MAX_HYPERDEX_TUPLE_PARAMS), AN_ATTRIBUTE_TUPLE, _)

  #undef AN_ATTRIBUTE_TUPLE

  #define PREDICATE_TUPLE(Z, N, _) \
    using BOOST_PP_CAT(predicate_tuple_,N) = \
      tuple<BOOST_PP_ENUM(N,HYPERDEX_SHOWTEXT,hyperdex_client_attribute_check)>; \
      /**/

  BOOST_PP_REPEAT(BOOST_PP_INC(MAX_HYPERDEX_TUPLE_PARAMS), PREDICATE_TUPLE, _)

  #undef PREDICATE_TUPLE

  #define MAP_ATTRIBUTE_TUPLE(Z, N, _) \
    using BOOST_PP_CAT(map_attribute_tuple_,N) = \
      tuple<BOOST_PP_ENUM(N,HYPERDEX_SHOWTEXT,hyperdex_client_map_attribute)>; \
      /**/

  BOOST_PP_REPEAT(BOOST_PP_INC(MAX_HYPERDEX_TUPLE_PARAMS), MAP_ATTRIBUTE_TUPLE, _)

  #undef MAP_ATTRIBUTE_TUPLE

  #define HYPERDEX_CLIENT_NAME_TUPLE(Z, N, _) \
    using BOOST_PP_CAT(name_tuple_,N) = \
      tuple<BOOST_PP_ENUM(N,HYPERDEX_SHOWTEXT,char const*)>; \
      /**/

  BOOST_PP_REPEAT(BOOST_PP_INC(MAX_HYPERDEX_TUPLE_PARAMS), HYPERDEX_CLIENT_NAME_TUPLE, _)

  #undef HYPERDEX_CLIENT_NAME_TUPLE

  #undef HYPERDEX_SHOWTEXT

} // namespace hyperdex

#endif // HYPERDEX_BASE_TYPES_HPP_INCLUDED
