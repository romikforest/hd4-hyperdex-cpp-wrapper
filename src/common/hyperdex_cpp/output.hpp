#ifndef HYPERDEX_OUTPUT_HPP_INCLUDED
#define HYPERDEX_OUTPUT_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: output.hpp
// First created: 2015-10-18-07.22
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
#include <iostream>
#include <sstream>
#include <unordered_map> // need standard unordered_map for log_ds_status because not all
  // unordered map implementations understand brace initialization lists and have 'at' member function
#include <chrono>
#include <utility>

// forest
#include <forest/string.hpp>
#include <forest/date.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/iterators_and_traits.hpp>
#include <hyperdex_cpp/attribute_status.hpp>

namespace hyperdex
{

  namespace helper
  {

    inline void print_hyperdex_client_attribute(std::ostream& lhs,
                                                hyperdex_client_attribute const& attr,
                                                bool map = false, bool key = false)
    {
      std::ios::fmtflags f(lhs.flags());

      lhs << std::boolalpha << std::dec;

      if (!map || key) lhs << "\tattr: " << ((attr.attr && strlen(attr.attr)) ? attr.attr : "(Empty)") << std::endl;

      if (map && key) lhs << "\tmap_key: ";
      else lhs << "\tvalue: ";

      if (attr.value_sz)
      {
        switch (attr.datatype)
        {
          case HYPERDATATYPE_INT64:
            {
              assert(attr.value_sz == sizeof(hyperdex::hyperdex_int));
              hyperdex::hyperdex_int num;
              hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex::hyperdex_int), &num);
              lhs << num << " ( decimal number )";
            }
            break;

          case HYPERDATATYPE_FLOAT:
            {
              assert(attr.value_sz == sizeof(hyperdex::hyperdex_float));
              hyperdex::hyperdex_float num;
              hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex::hyperdex_float), &num);
              lhs << num << " ( floating point decimal number )";
            }
            break;

          case HYPERDATATYPE_LIST_STRING:

            {

              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_LIST_STRING, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              char const* str;
              size_t sz;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": " << forest::string(str, sz)
                   << " (size: " << sz << ")";
              }
              if (!~res) lhs << " (Malformed list of string)";
              else if (count) lhs << " (List of string, " << count << " items)";
              else lhs << " (Empty list of string)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_LIST_INT64:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_LIST_INT64, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_int num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_list_int_next(&iter, &num)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": "
                   << num << " ( decimal number )";
              }

              if (!~res) lhs << " (Malformed list of int)";
              else if (count) lhs << " (List of int, " << count << " items)";
              else lhs << " (Empty list of int)";

              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_LIST_FLOAT:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_LIST_FLOAT, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_float num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_list_float_next(&iter, &num)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": "
                   << num
                   << " ( floating point decimal number )";
              }
              if (!~res) lhs << " (Malformed list of float)";
              else if (count) lhs << " (List of float, " << count << " items)";
              else lhs << " (Empty list of float)";
              lhs << ss.str();

            }

            break;

          case HYPERDATATYPE_SET_STRING:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_SET_STRING, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              char const* str;
              size_t sz;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": " << forest::string(str, sz)
                   << " (size: " << sz << ")";
              }
              if (!~res) lhs << " (Malformed set of string)";
              else if (count) lhs << " (Set of string, " << count << " items)";
              else lhs << " (Empty set of string)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_SET_INT64:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_SET_INT64, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_int num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_set_int_next(&iter, &num)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": "
                   << num
                   << " ( decimal number )";
              }
              if (!~res) lhs << " (Malformed set of int)";
              else if (count) lhs << " (Set of int, " << count << " items)";
              else lhs << " (Empty set of int)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_SET_FLOAT:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_SET_FLOAT, attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_float num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_set_float_next(&iter, &num)) == 1)
              {
                ss << std::endl << "\t\tItem " << count++ << ": "
                   << num
                   << " ( floating point decimal number )";
              }
              if (!~res) lhs << " (Malformed set of float)";
              else if (count) lhs << " (Set of float, " << count << " items)";
              else lhs << " (Empty set of float)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_STRING_STRING:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_STRING_STRING,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              char const* key;
              size_t key_sz;
              char const* val;
              size_t val_sz;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_string_string_next(&iter,
                                                                      &key, &key_sz,
                                                                      &val, &val_sz)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << forest::string(key, key_sz) << " (size: " << key_sz << ")"
                   << std::endl << "\t\t\tValue: " << forest::string(val, val_sz) << " (size: " << val_sz << ")";
              }
              if (!~res) lhs << " (Malformed map string - string)";
              else if (count) lhs << " (Map string - string, " << count << " items)";
              else lhs << " (Empty map string - string)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_STRING_INT64:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_STRING_INT64,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              char const* key;
              size_t key_sz;
              hyperdex::hyperdex_int num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_string_int_next(&iter,
                                                                  &key, &key_sz,
                                                                  &num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << forest::string(key, key_sz) << " (size: " << key_sz << ")"
                   << std::endl << "\t\t\tValue: " << num << " ( decimal number )";
              }
              if (!~res) lhs << " (Malformed map string - int)";
              else if (count) lhs << " (Map string - int, " << count << " items)";
              else lhs << " (Empty map string - int)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_STRING_FLOAT:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_STRING_FLOAT,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              char const* key;
              size_t key_sz;
              hyperdex::hyperdex_float num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_string_float_next(&iter,
                                                                     &key, &key_sz,
                                                                     &num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << forest::string(key, key_sz) << " (size: " << key_sz << ")"
                   << std::endl << "\t\t\tValue: " << num << " ( floating point decimal number )";
              }
              if (!~res) lhs << " (Malformed map string - float)";
              else if (count) lhs << " (Map string - float, " << count << " items)";
              else lhs << " (Empty map string - float)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_INT64_STRING:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_INT64_STRING,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_int num;
              char const* val;
              size_t val_sz;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_int_string_next(&iter,
                                                                   &num,
                                                                   &val, &val_sz)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << num << " ( decimal number )"
                   << std::endl << "\t\t\tValue: " << forest::string(val, val_sz) << " (size: " << val_sz << ")";
              }
              if (!~res) lhs << " (Malformed map int - string)";
              else if (count) lhs << " (Map int - string, " << count << " items)";
              else lhs << " (Empty map int - string)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_INT64_INT64:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_INT64_INT64,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_int key_num;
              hyperdex::hyperdex_int val_num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_int_int_next(&iter,
                                                                &key_num,
                                                                &val_num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << key_num << " ( decimal number )"
                   << std::endl << "\t\t\tValue: " << val_num << " ( decimal number )";
              }
              if (!~res) lhs << " (Malformed map int - int)";
              else if (count) lhs << " (Map int - int, " << count << " items)";
              else lhs << " (Empty map int - int)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_INT64_FLOAT:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_INT64_FLOAT,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_int key_num;
              hyperdex::hyperdex_float val_num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_int_float_next(&iter,
                                                                  &key_num,
                                                                  &val_num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << key_num << " ( decimal number )"
                   << std::endl << "\t\t\tValue: " << val_num << " ( floating point decimal number )";
              }
              if (!~res) lhs << " (Malformed map int - float)";
              else if (count) lhs << " (Map int - float, " << count << " items)";
              else lhs << " (Empty map int - float)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_FLOAT_STRING:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_STRING_STRING,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_float num;
              char const* val;
              size_t val_sz;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_float_string_next(&iter,
                                                                     &num,
                                                                     &val, &val_sz)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << num << " ( floating point number )"
                   << std::endl << "\t\t\tValue: " << forest::string(val, val_sz) << " (size: " << val_sz << ")";
              }
              if (!~res) lhs << " (Malformed map float - string)";
              else if (count) lhs << " (Map float - string, " << count << " items)";
              else lhs << " (Empty map float - string)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_FLOAT_INT64:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_FLOAT_INT64,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_float key_num;
              hyperdex::hyperdex_int val_num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_float_int_next(&iter,
                                                                  &key_num,
                                                                  &val_num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << key_num << " ( floating point decimal number )"
                   << std::endl << "\t\t\tValue: " << val_num << " ( decimal number )";
              }
              if (!~res) lhs << " (Malformed map float - int)";
              else if (count) lhs << " (Map float - int, " << count << " items)";
              else lhs << " (Empty map float - int)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_MAP_FLOAT_FLOAT:

            {
              hyperdex_ds_iterator iter;
              hyperdex_ds_iterator_init(&iter, HYPERDATATYPE_MAP_FLOAT_FLOAT,
                                        attr.value, attr.value_sz);

              std::stringstream ss;
              hyperdex::hyperdex_int_ret res;
              hyperdex::hyperdex_float key_num;
              hyperdex::hyperdex_float val_num;
              size_t count = 0;

              while((res = hyperdex_ds_iterate_map_float_float_next(&iter,
                                                                    &key_num,
                                                                    &val_num)) == 1)
              {
                ss << std::endl << "\t\tItem "  << count++ << ":"
                   << std::endl << "\t\t\tKey: " << key_num << " ( floating point decimal number )"
                   << std::endl << "\t\t\tValue: " << val_num << " ( floating point decimal number )";
              }
              if (!~res) lhs << " (Malformed map float - float)";
              else if (count) lhs << " (Map float - float, " << count << " items)";
              else lhs << " (Empty map float - float)";
              lhs << ss.str();
            }

            break;

          case HYPERDATATYPE_TIMESTAMP_SECOND:
          case HYPERDATATYPE_TIMESTAMP_MINUTE:
          case HYPERDATATYPE_TIMESTAMP_HOUR:
          case HYPERDATATYPE_TIMESTAMP_DAY:
          case HYPERDATATYPE_TIMESTAMP_WEEK:
          case HYPERDATATYPE_TIMESTAMP_MONTH:
            {
              static_assert(sizeof(time_t) == sizeof(hyperdex::hyperdex_int),
                            "Size of time_t must be equivalent to size of hyperdex_int");
              assert(attr.value_sz == sizeof(time_t));
              time_t num;
              hyperdex_ds_unpack_int(attr.value, sizeof(time_t), &num);
              lhs << std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::duration<int128_t, std::nano>>
                                            {std::chrono::duration<int128_t, std::micro>(num)};
            }

            break;

          default:

            lhs << forest::string(attr.value, attr.value_sz);
            break;

        } // switch (attr.datatype)

      } else lhs << " ( none )";

      lhs << std::endl;

      lhs.flags(f);

      if (map && key) lhs << "\tmap_key_sz: " << attr.value_sz << std::endl;
      else            lhs << "\tvalue_sz: " << attr.value_sz << std::endl;

      if (map)
      {
        if (key) lhs << "\tmap_key_datatype: ";
        else     lhs << "\tvalue_datatype: ";
      }
      else lhs << "\tdatatype: ";

      lhs << attr.datatype << std::endl;

    } // print_hyperdex_client_attribute

    inline void print_predicate(std::ostream& lhs, hyperdex_client_attribute_check const& attr)
    {
      hyperdex_client_attribute a { attr.attr, attr.value, attr.value_sz, attr.datatype };
      print_hyperdex_client_attribute(lhs, a);

      lhs << "\tpredicate: " << attr.predicate << std::endl;
    }

    inline void print_map_attribute(std::ostream& lhs, hyperdex_client_map_attribute const& attr)
    {
      hyperdex_client_attribute key { attr.attr, attr.map_key, attr.map_key_sz, attr.map_key_datatype };
      print_hyperdex_client_attribute(lhs, key, true, true);
      hyperdex_client_attribute value { attr.attr, attr.value, attr.value_sz, attr.value_datatype };
      print_hyperdex_client_attribute(lhs, value, true);
    }

    template<typename T>
    inline forest::string output_helper(T&&) { return ""; }

    inline forest::string output_helper(forest::string const& str)
    {
      return str.size() ? forest::string{" (size: "} + forest::to_string(str.size()) + ")" : " (Empty)";
    }

    inline forest::string output_helper(char const* str)
    {
      size_t sz = strlen(str);
      return sz ? forest::string{" (size: "} + forest::to_string(sz) + ")" : " (Empty)";
    }

  } // namespace helper

  inline namespace base_output
  {

    inline std::ostream& operator << (std::ostream& lhs, hyperdex_client_attribute const& attr)
    {
      lhs << "Hyperdex client attribute:" << std::endl;

      hyperdex::helper::print_hyperdex_client_attribute(lhs, attr);

      return lhs << std::endl;
    }

    template<typename T>
    inline
    std::enable_if_t<std::is_same<T, hyperdex_client_attribute*>::value ||
                     std::is_same<T, hyperdex_client_attribute const*>::value,
    std::ostream&>
    operator << (std::ostream& lhs, T const& attr)
    {
      return lhs << *attr;
    }

    inline std::ostream& operator << (std::ostream& lhs, hyperdex_client_attribute_check const& attr)
    {
      lhs << "Hyperdex client attribute check:" << std::endl;

      hyperdex::helper::print_predicate(lhs, attr);

      return lhs << std::endl;
    }

    inline std::ostream& operator << (std::ostream& lhs, hyperdex_client_attribute_check const* attr)
    {
      return lhs << *attr;
    }

    inline std::ostream& operator << (std::ostream& lhs, hyperdex_client_map_attribute const& attr)
    {
      lhs << "Hyperdex client map attribute:" << std::endl;

      hyperdex::helper::print_map_attribute(lhs, attr);

      return lhs << std::endl;
    }

    inline std::ostream& operator << (std::ostream& lhs, hyperdex_client_map_attribute const* attr)
    {
      return lhs << *attr;
    }

    inline std::ostream& operator << (std::ostream& lhs,  hyperdex_admin_perf_counter const& pc)
    {
      lhs << "Hyperdex performance counter:" << std::endl;
      lhs << "\tid: " << pc.id << std::endl;
      lhs << "\ttime: " << std::chrono::time_point<std::chrono::system_clock,
                                                   std::chrono::duration<int128_t, std::nano>>
                          {std::chrono::duration<int128_t, std::nano>(pc.time)} << std::endl;
      lhs << "\tproperty: " << pc.property << std::endl;
      lhs << "\tmeasurement: " << pc.measurement << std::endl;
      return lhs;
    }

    inline std::ostream& operator << (std::ostream& lhs,  hyperdex_admin_perf_counter const* pc)
    {
      return lhs << *pc;
    }

  } // base_output

  inline namespace blob_output
  {

    inline std::ostream& operator << (std::ostream& lhs,  blob_pair const& attr)
    {
      return lhs << forest::string(attr.first, attr.second);
    }

    template<typename First, typename Second,
             typename = std::enable_if_t<!std::is_convertible<std::pair<First, Second>, blob_pair const&>::value>>
    inline std::ostream& operator << (std::ostream& lhs,  std::pair<First, Second> const& attr)
    {
      return lhs << "Pair: " << std::endl
                 << "Key: " << attr.first << std::endl
                 << "Value: " << attr.second << std::endl;
    }

    // inline std::ostream& operator << (std::ostream& lhs,  blob_pair const& attr)
    // inline std::ostream& operator << (std::ostream& lhs,  std::pair<First, Second> const& attr)
    // need using hyperdex::blob_output::operator <<
    // to use in other namespace, because std::pair is in namespace std

  } // namespace blob_output

  inline std::ostream& operator << (std::ostream& lhs,  an_attribute const& attr)
  {
    return lhs << attr.a;
  }

  template<typename T>
  inline
  std::enable_if_t<std::is_same<T, an_attribute*>::value || std::is_same<T, an_attribute const*>::value,
  std::ostream&>
  operator << (std::ostream& lhs,  T const& attr)
  {
    return lhs << attr->a;
  }

  using forest::string;
  using forest::to_string;

  namespace helper
  {

    struct object_name_helper1
    {
      public:

        template<size_t N>
        static string get(attribute_array<N> const&) noexcept
        {
          return string { "Attribute c++ array (attribute_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(an_attribute_array<N> const&) noexcept
        {
          return string { "Attribute c++ array (an_attribute_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(predicate_array<N> const&) noexcept
        {
          return string { "Predicate c++ array (predicate_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(map_attribute_array<N> const&) noexcept
        {
          return string { "Map attribute c++ array (map_attribute_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(name_array<N> const&) noexcept
        {
          return string { "Name c++ array (name_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(attribute_c_array<N> const&) noexcept
        {
          return string { "Attribute c array (attribute_c_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(an_attribute_c_array<N> const&) noexcept
        {
          return string { "Attribute c array (an_attribute_c_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(predicate_c_array<N> const&) noexcept
        {
          return string { "Predicate c array (predicate_c_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(map_attribute_c_array<N> const&) noexcept
        {
          return string { "Map attribute c array (map_attribute_c_array<" } + to_string(N) + ">)";
        }

        template<size_t N>
        static string get(name_c_array<N> const&) noexcept
        {
          return string { "Name c array (name_c_array<" } + to_string(N) + ">)";
        }

        static string get(attribute_vector const&) noexcept
        {
          return "Attribute vector (attribute_vector)";
        }

        static string get(an_attribute_vector const&) noexcept
        {
          return "Attribute vector (an_attribute_vector)";
        }

        static string get(predicate_vector const&) noexcept
        {
          return "Predicate vector (predicate_vector)";
        }

        static string get(map_attribute_vector const&) noexcept
        {
          return "Map attribute vector (map_attribute_vector)";
        }

        static string get(name_vector const&) noexcept
        {
          return "Name vector (name_vector)";
        }

        template<typename T>
        static std::enable_if_t<is_tuple_of_attributes<T>::value, string> get(T const&) noexcept
        {
          return "Tuple of attributes";
        }

        template<typename T>
        static std::enable_if_t<is_tuple_of_predicates<T>::value, string> get(T const&) noexcept
        {
          return "Tuple of predicates";
        }

        template<typename T>
        static std::enable_if_t<is_tuple_of_map_attributes<T>::value, string> get(T const&) noexcept
        {
          return "Tuple of map attributes";
        }

        template<typename T>
        static std::enable_if_t<is_tuple_of_names<T>::value, string> get(T const&) noexcept
        {
          return "Tuple of names";
        }

    }; // struct object_name_helper1

    struct object_name_helper2
    {
        template<typename T>
        static std::enable_if_t<is_attribute_container<T>::value, string> get(T const&) noexcept
        {
          return "Attribute container";
        }

        template<typename T>
        static std::enable_if_t<is_predicate_container<T>::value, string> get(T const&) noexcept
        {
          return "Predicate container";
        }

        template<typename T>
        static std::enable_if_t<is_map_attribute_container<T>::value, string> get(T const&) noexcept
        {
          return "Map attribute container";
        }

    }; // struct object_name_helper2

    template<typename T, typename = string>
    struct has_object_name_helper1: std::false_type {};

    template<typename T>
    struct has_object_name_helper1<T, decltype(object_name_helper1::get(std::declval<T>()))>: std::true_type {};

    template<typename T, typename = string>
    struct has_object_name_helper2: std::false_type {};

    template<typename T>
    struct has_object_name_helper2<T, decltype(object_name_helper2::get(std::declval<T>()))>: std::true_type {};

    struct object_name
    {
        template<typename T>
        static std::enable_if_t<
          !has_object_name_helper1<T>::value &&
          !has_object_name_helper2<T>::value,
        forest::string>
        get(T const&) noexcept
        {
          return "Unknown type";
        }

        template<typename T>
        static std::enable_if_t<has_object_name_helper1<T>::value, forest::string>
        get(T const& t) noexcept
        {
          return object_name_helper1::get(t);
        }

        template<typename T>
        static std::enable_if_t<
          !has_object_name_helper1<T>::value &&
           has_object_name_helper2<T>::value,
        forest::string>
        get(T const& t) noexcept
        {
          return object_name_helper2::get(t);
        }
    };

  } // namespace helper

  inline namespace containers_output
  {

    template<typename T>
    inline
    std::enable_if_t<hyperdex::is_attribute_container<T>::value     ||
                     hyperdex::is_predicate_container<T>::value     ||
                     hyperdex::is_map_attribute_container<T>::value ||
                     hyperdex::is_name_container<T>::value,
    std::ostream&>
    operator << (std::ostream& lhs,  T const& arg)
    {
      std::ios::fmtflags f(lhs.flags());
      std::stringstream ss;
      ss.flags(f);
      size_t count = 0;
      auto p = hyperdex::begin(arg);
      auto e = hyperdex::end(arg);

      lhs << hyperdex::helper::object_name::get(arg) << ": (";

      for (; p != e; ++count, ++p)
      {
        ss << "\tItem " << std::dec << count << ": ";
        ss.flags(f);
        ss << *p << hyperdex::helper::output_helper(*p) << std::endl;
      }

      if (count)
      {
        lhs << std::dec << count << " items)" << std::endl;
        lhs.flags(f);
        lhs << ss.str();
      }
      else lhs << "Empty)" << std::endl;

      return lhs;
    }

  } // namespace containers_output

  template<typename, template<typename, typename = void> typename>
  class list;

  template<typename T1, template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs,  list<T1, T2> const& l)
  {
    std::ios::fmtflags f(lhs.flags());
    std::stringstream ss;
    ss.flags(f);
    size_t count = 0;

    lhs << "hyperdex::list (" << list<T1, T2>::type << "): (";

    for (const auto& val: l)
    {
      ss << "\tItem " << std::dec << count << ": ";
      ss.flags(f);
      ss << val << helper::output_helper(val) << std::endl;
      ++count;
    }

    if (count)
    {
      lhs << std::dec << count << " items)" << std::endl;
      lhs.flags(f);
      lhs << ss.str();
    }
    else lhs << "Empty)" << std::endl;

    return lhs;
  }

  template<typename, template<typename, typename = void> typename>
  class set;

  template<typename T1, template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs,  set<T1, T2> const& s)
  {
    std::ios::fmtflags f(lhs.flags());
    std::stringstream ss;
    ss.flags(f);
    size_t count = 0;

    lhs << "hyperdex::set (" << set<T1, T2>::type << "): (";

    for (const auto& val: s)
    {
      ss << "\tItem " << std::dec << count << ": ";
      ss.flags(f);
      ss << val << helper::output_helper(val) << std::endl;
      ++count;
    }

    if (count)
    {
      lhs << std::dec << count << " items)" << std::endl;
      lhs.flags(f);
      lhs << ss.str();
    }
    else lhs << "Empty)" << std::endl;

    return lhs;
  }

  template<typename, typename, template<typename, typename = void> typename>
  class map;

  template<typename T1, typename T2, template<typename, typename = void> typename T3>
  inline std::ostream& operator << (std::ostream& lhs,  map<T1, T2, T3> const& m)
  {
    std::ios::fmtflags f(lhs.flags());
    std::stringstream ss;
    ss.flags(f);
    size_t count = 0;

    lhs << "hyperdex::map (" << map<T1, T2, T3>::type << "): (";

    for (const auto& val: m)
    {
      ss << "\tItem " << std::dec << count << ": ";
      ss.flags(f);
      ss << val << helper::output_helper(val) << std::endl;
      ++count;
    }

    if (count)
    {
      lhs << std::dec << count << " items)" << std::endl;
      lhs.flags(f);
      lhs << ss.str();
    }
    else lhs << "Empty)" << std::endl;

    return lhs;
  }

  template<template<typename, typename = void> typename>
  class basic_document;

  template<template<typename, typename = void> typename T>
  inline std::ostream& operator << (std::ostream& lhs,  basic_document<T> const& doc)
  {
    return lhs << doc.to_string();
  }

  template<hyperdatatype, template<typename, typename = void> typename>
  class timestamp;

  template<hyperdatatype T1, template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs, timestamp<T1, T2> rhs)
  {
    return lhs << rhs.stamp();
  }

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_attributes;

  template<template<typename, typename = void> typename T1,
           template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs,  basic_attributes<T1, T2> const& attr)
  {
    using std::endl;

    std::ios::fmtflags f(lhs.flags());

    lhs << std::boolalpha << std::dec
        << endl << "Attributes object:"
        << (attr.size() ? " (" + to_string(attr.size()) + " items)" : "")
        << endl << endl;

    if (!attr.size()) lhs << "\tEmpty" << endl << endl;
    else for (typename basic_attributes<T1, T2>::size_type i = 0; i < attr.size(); ++i)
    {
      lhs << "\tIndex: " << i << endl;
      helper::print_hyperdex_client_attribute(lhs, attr[i]);
      lhs << endl;

    } // for

    lhs.flags(f);

    return lhs;

  }

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_predicates;

  template<template<typename, typename = void> typename T1,
           template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs,  basic_predicates<T1, T2> const& attr)
  {
    using std::endl;

    std::ios::fmtflags f(lhs.flags());

    lhs << std::boolalpha << std::dec
        << endl << "Predicates object:"
        << (attr.size() ? " (" + std::to_string(attr.size()) + " items)" : "")
        << endl << endl;

    if (!attr.size()) lhs << "\tEmpty" << endl << endl;
    else for (typename basic_predicates<T1, T2>::size_type i = 0; i < attr.size(); ++i)
    {
      lhs << "\tIndex: " << i << endl;
      helper::print_predicate(lhs, attr[i]);
      lhs << endl;

    } // for

    lhs.flags(f);

    return lhs;

  }

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_map_attributes;

  template<template<typename, typename = void> typename T1,
           template<typename, typename = void> typename T2>
  inline std::ostream& operator << (std::ostream& lhs,  basic_map_attributes<T1, T2> const& attr)
  {
    using std::endl;

    std::ios::fmtflags f(lhs.flags());

    lhs << std::boolalpha << std::dec
        << endl << "Map attributes object:"
        << (attr.size() ? " (" + std::to_string(attr.size()) + " items)" : "")
        << endl << endl;

    if (!attr.size()) lhs << "\tEmpty" << endl << endl;
    else for (typename basic_map_attributes<T1, T2>::size_type i = 0; i < attr.size(); ++i)
    {
      lhs << "\tIndex: " << i << endl;
      helper::print_map_attribute(lhs, attr[i]);
      lhs << endl;

    } // for

    lhs.flags(f);

    return lhs;

  }

  inline std::ostream& operator << (std::ostream& lhs,  attribute_status const& val)
  {
    switch (val)
    {
      case attribute_status::ok         : lhs << "ok";          break;
      case attribute_status::not_found  : lhs << "not found";   break;
      case attribute_status::bad_request: lhs << "bad request"; break;
      case attribute_status::not_changed: lhs << "not changed"; break;
      case attribute_status::empty      : lhs << "empty";       break;
                                 default: lhs << "unknown";     break;
    };

    return lhs;
  }

  inline std::ostream& operator << (std::ostream& lhs,  forest::vector<attribute_status> const& val)
  {
    std::ios::fmtflags f(lhs.flags());
    lhs << "Fill status vector: (" << std::dec;
    if (val.size()) lhs << val.size() << " items)";
    else            lhs << "Empty)";
    lhs << std::endl;
    size_t i = 0;
    for (auto const& x: val)
    {
      lhs << "\tItem " << i << ": " << x << std::endl;
      ++i;
    }

    lhs.flags(f);
    return lhs;
  }

} // namespace hyperdex

using hyperdex::base_output::operator<<;
using hyperdex::containers_output::operator<<;

std::ostream& operator << (std::ostream& lhs,  hyperdex_admin_returncode rhs);
std::ostream& operator << (std::ostream& lhs, hyperdex_client_returncode rhs);

#endif // HYPERDEX_OUTPUT_HPP_INCLUDED
