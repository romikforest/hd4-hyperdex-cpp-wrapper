#ifndef HYPERDEX_MAP_TRAITS_HPP_INCLUDED
#define HYPERDEX_MAP_TRAITS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: map_traits.hpp
// First created: 2015-10-18-11.04
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
#include <hyperdex_cpp/common.hpp>

namespace hyperdex
{

  /////////////////////////////////////////////////////////////
  // common map iterator traits
  template<hyperdatatype MapType>
  class map_iterator_traits
  {
      static_assert(MapType != HYPERDATATYPE_MAP_STRING_STRING,
                  "Unsupported combination of hyperdex data types for map argument.");
  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_STRING_STRING>
  {
    public:

      using   key_type = blob_pair;
      using value_type = blob_pair;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_string_string_next(&iter,
                                                          &key.first, &key.second,
                                                          &value.first, &value.second);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_STRING_INT64>
  {
    public:

      using   key_type = blob_pair;
      using value_type = hyperdex_int;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_string_int_next(&iter,
                                                       &key.first, &key.second,
                                                       &value);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_STRING_FLOAT>
  {
    public:

      using   key_type = blob_pair;
      using value_type = hyperdex_float;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_string_float_next(&iter,
                                                         &key.first, &key.second,
                                                         &value);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_INT64_STRING>
  {
    public:

      using   key_type = hyperdex_int;
      using value_type = blob_pair;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_int_string_next(&iter, &key, &value.first, &value.second);
      }
  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_INT64_INT64>
  {
    public:

      using   key_type = hyperdex_int;
      using value_type = hyperdex_int;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_int_int_next(&iter, &key, &value);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_INT64_FLOAT>
  {
    public:

      using   key_type = hyperdex_int;
      using value_type = hyperdex_float;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_int_float_next(&iter, &key, &value);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_FLOAT_STRING>
  {
    public:

      using   key_type = hyperdex_float;
      using value_type = blob_pair;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_float_string_next(&iter, &key,
                                                         &value.first, &value.second);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_FLOAT_INT64>
  {
    public:

      using   key_type = hyperdex_float;
      using value_type = hyperdex_int;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_float_int_next(&iter, &key, &value);
      }

  };

  template<> class map_iterator_traits<HYPERDATATYPE_MAP_FLOAT_FLOAT>
  {
    public:

      using   key_type = hyperdex_float;
      using value_type = hyperdex_float;

      static hyperdex_int_ret next(hyperdex_ds_iterator& iter,
                                     key_type& key,
                                   value_type& value) noexcept
      {
        return hyperdex_ds_iterate_map_float_float_next(&iter, &key, &value);
      }

  };


  /////////////////////////////////////////////////////////////
  // common map traits
  template<hyperdatatype KeyType, hyperdatatype ValueType>
  class map_traits
  {
      static_assert(!(KeyType == ValueType || KeyType != ValueType),
                  "Unsupported combination of hyperdex data types for map argument.");
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_FLOAT;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_STRING>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_INT64>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_FLOAT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_FLOAT;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_SECOND>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_MINUTE>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_HOUR>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_DAY>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_WEEK>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_TIMESTAMP_MONTH>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_INT64;
  };

  ///

  template<> class map_traits<HYPERDATATYPE_STRING, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_INT64, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_FLOAT, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_FLOAT_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_SECOND, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MINUTE, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_HOUR, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_DAY, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_WEEK, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_TIMESTAMP_MONTH, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_INT64_STRING;
  };

  template<> class map_traits<HYPERDATATYPE_DOCUMENT, HYPERDATATYPE_DOCUMENT>
  {
    public:
      static constexpr hyperdatatype const type = HYPERDATATYPE_MAP_STRING_STRING;
  };

} // hyperdex

#endif // HYPERDEX_MAP_TRAITS_HPP_INCLUDED
