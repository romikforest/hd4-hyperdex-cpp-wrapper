#ifndef HYPERDEX_DATA_TRAITS_HPP_INCLUDED
#define HYPERDEX_DATA_TRAITS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: data_traits.hpp
// First created: 2015-09-15-11.26
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

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>

// forest
#include <forest/alternatives.hpp>
#include <forest/endian.hpp>
#include <forest/string.hpp>

// C++
#include <type_traits>
#include <cstddef>

static_assert(std::is_same<hyperdex::hyperdex_int, int64_t>::value,
              "This file was designed for 64 bit int fields in hyperdex database");

static_assert(std::is_same<hyperdex::hyperdex_float, double>::value && sizeof(double) == 8,
              "This file was designed for 64 bit float fields in hyperdex database");

#ifdef FOREST_USE_WSTRINGS
#include <cwchar>
#endif // FOREST_USE_WSTRINGS

#ifdef FOREST_USE_SERIALIZATION
#include <sstream>
#include <forest/serialization.hpp>
#endif // FOREST_USE_SERIALIZATION

namespace hyperdex
{
  using forest::int128_t;
  using forest::uint128_t;
  using forest::string;
  using forest::to_string;
  using forest::little_to_native;
  using forest::native_to_little;
  using forest::swap_bytes_if_be;

  /////////////////////////////////////////////////////////////
  // Primitive data traits

  #ifndef FOREST_USE_SERIALIZATION

  // template
  template <typename PrimitiveData, typename Enable = void>
  class primitive_data_traits
  {
    static_assert(!sizeof(PrimitiveData), "Unsupported type for argument of primitive hyperdex type.");
  };

  #else

  using forest::oarchive;
  using forest::iarchive;

  // template
  template <typename PrimitiveData, typename Enable = void>
  class primitive_data_traits
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        PrimitiveData const& data)
                                        noexcept
      {
        try
        {
          res_str  = nullptr;
          res_size = 0;

          std::stringstream ss;

          try
          {
            oarchive oa(ss);
            oa << data;
          }
          catch(...) { log_ds_status(status = HYPERDEX_DS_NOMEM); return -1; }

          auto arena = hyperdex_ds_arena_create();
          if (!arena) { log_ds_status(status = HYPERDEX_DS_NOMEM); return -1; }
          arenas.emplace_back(arena, arena_deleter{});

          if (!~hyperdex_ds_copy_string(arena, ss.str().c_str(), ss.str().size(), &status, &res_str, &res_size))
          {
            log_ds_status(status);
            arenas.pop_back();
            return -1;
          }
        }
        catch(...) {  status = HYPERDEX_DS_WRONG_STATE; return -1; }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          PrimitiveData const& data)
                                          noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_list_append_string(list, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         PrimitiveData const& data)
                                         noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_set_insert_string(set, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             PrimitiveData const& data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_key_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             PrimitiveData const& data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_val_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static PrimitiveData deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_STRING);

        PrimitiveData data {};

        try
        {
          std::stringstream ss { { attr.value, attr.value_sz } };
          iarchive ia(ss);
          ia >> data;
        }
        catch(...) { log_ds_status(HYPERDEX_DS_WRONG_STATE); }

        return data;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, PrimitiveData& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = PrimitiveData {}; } catch(...) {}
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, PrimitiveData& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = PrimitiveData {}; } catch(...) {}
          if (!~res) log_malformed_set();
        }
        return res;

      }

  }; // template for primitive_data_traits

  #endif // FOREST_USE_SERIALIZATION

  // primitive data traits specialization for blob_pair
  template <> class primitive_data_traits<blob_pair>
  {
    static_assert(true, "Primitive data type for std::pair<char const*, size_t> is not permitted.");
  };

  // primitive data traits specialization for char const*
  template<> class primitive_data_traits<char const*>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 0;
      static constexpr          bool const is_string = true;
      static constexpr          bool const nondeserializable = true;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        res_str  = data;
        res_size = strlen(data);
        return 0;
      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          char const* data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_string(list, data, strlen(data), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         char const* data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_string(set, data, strlen(data), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_string(map, data, strlen(data), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_string(map, data, strlen(data), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return nullptr;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::list with return type char const* is not possible as deserialization "
                    "char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return -1;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::set with return type char const* is not possible as deserialization "
                    "char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return -1;
      }

  }; // primitive data traits specialization for char const*

  // primitive data traits specialization for char*
  template<> class primitive_data_traits<char*>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<char const*>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<char const*>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<char const*>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<char const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<char const*>::is_string;
      static constexpr          bool const nondeserializable = primitive_data_traits<char const*>::nondeserializable;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<char const*>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<char const*>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        return primitive_data_traits<char const*>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          char const* data)
                                          noexcept
      {
        return primitive_data_traits<char const*>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         char const* data)
                                         noexcept
      {
        return primitive_data_traits<char const*>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        return primitive_data_traits<char const*>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        return primitive_data_traits<char const*>::map_insert_val(arenas, status, map, data);
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char*.");

        return nullptr;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::list with return type char* is not possible as deserialization "
                    "char* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char*.");

        return -1;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::set with return type char* is not possible as deserialization "
                    "char* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char*.");

        return -1;
      }

  }; // primitive data traits specialization for char*

  // primitive data traits specialization for std::nullptr_t
  template<> class primitive_data_traits<std::nullptr_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<char const*>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<char const*>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<char const*>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<char const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<char const*>::is_string;
      static constexpr          bool const nondeserializable = primitive_data_traits<char const*>::nondeserializable;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<char const*>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<char const*>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        return primitive_data_traits<char const*>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          char const* data)
                                          noexcept
      {
        return primitive_data_traits<char const*>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         char const* data)
                                         noexcept
      {
        return primitive_data_traits<char const*>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        return primitive_data_traits<char const*>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char const* data)
                                             noexcept
      {
        return primitive_data_traits<char const*>::map_insert_val(arenas, status, map, data);
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return nullptr;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::list with return type char const* is not possible as deserialization "
                    "char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return -1;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::set with return type char const* is not possible as deserialization "
                    "char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return -1;
      }

  }; // primitive data traits specialization for std::nullptr_t

  #ifdef FOREST_USE_WSTRINGS

  using forest::wstring;

  // primitive data traits specialization for wchar_t const*
  template<> class primitive_data_traits<wchar_t const*>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 0;
      static constexpr          bool const is_string = true;
      static constexpr          bool const nondeserializable = true;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wchar_t const* data)
                                        noexcept
      {
        try
        {
          res_str  = reinterpret_cast<char const*>(data);
          res_size = wcslen(data) * sizeof(wchar_t);
        } catch(...) { status = HYPERDEX_DS_WRONG_STATE; return -1; }
        return 0;
      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          wchar_t const* data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_string(list,
                                             reinterpret_cast<char const*>(data),
                                             wcslen(data) * sizeof(wchar_t),
                                             &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         wchar_t const* data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_string(set,
                                            reinterpret_cast<char const*>(data),
                                            wcslen(data) * sizeof(wchar_t),
                                            &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wchar_t const* data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_string(map,
                                                reinterpret_cast<char const*>(data),
                                                wcslen(data) * sizeof(wchar_t),
                                                &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wchar_t const* data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_string(map,
                                                reinterpret_cast<char const*>(data),
                                                wcslen(data) * sizeof(wchar_t),
                                                &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      template<typename Any>
      static wchar_t const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of wchar_t const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized wchar_t const*.");

        return nullptr;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::list with return type wchar_t const* is not possible as deserialization "
                    "wchar_t const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized wchar_t const*.");

        return -1;
      }

      template<typename Any>
      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator&, Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Creation of hyperdex::set with return type wchar_t const* is not possible as deserialization "
                    "wchar_t const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized wchar_t const*.");

        return -1;
      }

  }; // primitive data traits specialization for wchar_t const*

  #endif // FOREST_USE_WSTRINGS

  // primitive data traits specialization for string
  template<> class primitive_data_traits<string>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = true;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        size_t& res_size,
                                        string const& data)
                                        noexcept
      {
        res_str  = data.c_str();
        res_size = data.size();
        return 0;
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        string&& data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        if (!~hyperdex_ds_copy_string(arena, data.c_str(), data.size(), &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          string const& data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_string(list, data.c_str(), data.size(), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         string const& data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_string(set, data.c_str(), data.size(), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             string const& data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_string(map, data.c_str(), data.size(), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             string const& data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_string(map, data.c_str(), data.size(), &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static string deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               attr.datatype == HYPERDATATYPE_STRING);

        if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return to_string(num);
        }
        else if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return to_string(num);
        }

        return { attr.value, attr.value_sz };
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, string& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = string {}; } catch(...) {}
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, string& value)  noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = string {}; } catch(...) {}
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for string

  #ifdef FOREST_USE_WSTRINGS

  // primitive data traits specialization for wstring
  template<> class primitive_data_traits<wstring>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = primitive_data_traits<string>::aren_max;
      static constexpr          bool const is_string = true;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode&,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wstring const& data)
                                        noexcept
      {
        res_str  = reinterpret_cast<char const*>(data.c_str());
        res_size = data.size() * sizeof(wchar_t);
        return 0;
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wstring&& data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        if (!~hyperdex_ds_copy_string(arena,
                                      reinterpret_cast<char const*>(data.c_str()),
                                      data.size() * sizeof(wchar_t),
                                      &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          wstring const& data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_string(list,
                                             reinterpret_cast<char const*>(data.c_str()),
                                             data.size() * sizeof(wchar_t),
                                             &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         wstring const& data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_string(set,
                                            reinterpret_cast<char const*>(data.c_str()),
                                            data.size() * sizeof(wchar_t),
                                            &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wstring const& data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_string(map,
                                                reinterpret_cast<char const*>(data.c_str()),
                                                data.size() * sizeof(wchar_t),
                                                &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wstring const& data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_string(map,
                                                reinterpret_cast<char const*>(data.c_str()),
                                                data.size() * sizeof(wchar_t),
                                                &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static wstring deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_STRING);
        return wstring(reinterpret_cast<wchar_t const*>(attr.value),
                       attr.value_sz * sizeof(char) / sizeof(wchar_t));
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, wstring& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = wstring {}; } catch(...) {}
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, wstring& value)  noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          try { value = wstring {}; } catch(...) {}
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for wstring

  #endif // FOREST_USE_WSTRINGS

  // primitive data traits specialization for int64 (hyperdex_int)
  template<> class primitive_data_traits<hyperdex_int>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_INT64;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_INT64;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_INT64;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_INT64;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_INT64;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_int data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        if (!~hyperdex_ds_copy_int(arena, data, &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          hyperdex_int data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_int(list, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         hyperdex_int data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_int(set, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_int data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_int(map, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_int data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_int(map, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               (attr.datatype == HYPERDATATYPE_STRING &&
                (attr.value_sz == sizeof(hyperdex_int128) || !attr.value_sz) ) );

        if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(hyperdex_int128))
        {
          return little_to_native(*reinterpret_cast<hyperdex_int128 const*>(attr.value));
        }

        return 0;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, hyperdex_int& value) noexcept
      {
        return hyperdex_ds_iterate_list_int_next(&iter, &value);
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, hyperdex_int& value)  noexcept
      {
        return hyperdex_ds_iterate_set_int_next(&iter, &value);
      }

  }; // primitive data traits specialization for int64 (hyperdex_int)

  // primitive data traits specialization for uint32_t
  template<> class primitive_data_traits<uint32_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint32_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          uint32_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        hyperdex_ds_set* set,
                                        uint32_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint32_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                            hyperdex_ds_returncode& status,
                                            hyperdex_ds_map* map,
                                            uint32_t data)
                                            noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static uint32_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, uint32_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, uint32_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for uint32_t

  // primitive data traits specialization for int32_t
  template<> class primitive_data_traits<int32_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int32_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          int32_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         int32_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int32_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int32_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static int32_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, int32_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, int32_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for int32_t

  // primitive data traits specialization for int8_t
  template<> class primitive_data_traits<int8_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int8_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          int8_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         int8_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int8_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int8_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static int8_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, int8_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, int8_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for int8_t

  // primitive data traits specialization for uint8_t
  template<> class primitive_data_traits<uint8_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint8_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          uint8_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         uint8_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint8_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint8_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static uint8_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, uint8_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, uint8_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for uint8_t

  // primitive data traits specialization for char
  template<> class primitive_data_traits<char>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          char data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         char data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             char data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static char deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, char& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, char& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for char

  #ifdef FOREST_USE_WSTRINGS

  // primitive data traits specialization for wchar_t
  template<> class primitive_data_traits<wchar_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wchar_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          wchar_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         wchar_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wchar_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             wchar_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static wchar_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, wchar_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, wchar_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for wchar_t

  #endif // FOREST_USE_WSTRINGS

  // primitive data traits specialization for uint64_t
  template<> class primitive_data_traits<uint64_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint64_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          uint64_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         uint64_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint64_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint64_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static uint64_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, uint64_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, uint64_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for uint64_t

  // primitive data traits specialization for int16_t
  template<> class primitive_data_traits<int16_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int16_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          int16_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         int16_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int16_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int16_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static int16_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, int16_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, int16_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for int16_t

  // primitive data traits specialization for uint16_t
  template<> class primitive_data_traits<uint16_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_int>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_int>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_int>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_int>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_int>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint16_t data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          uint16_t data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         uint16_t data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint16_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             uint16_t data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data);
      }

      static int16_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, uint16_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, uint16_t& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for uint16_t

  // primitive data traits specialization for double (hyperdex_float)
  template<> class primitive_data_traits<hyperdex_float>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_FLOAT;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_FLOAT;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_FLOAT;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_FLOAT;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_FLOAT;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_float data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        if (!~hyperdex_ds_copy_float(arena, data, &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector&,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          hyperdex_float data)
                                          noexcept
      {
        if (!~hyperdex_ds_list_append_float(list, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector&,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         hyperdex_float data)
                                         noexcept
      {
        if (!~hyperdex_ds_set_insert_float(set, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_float data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_key_float(map, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector&,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_float data)
                                             noexcept
      {
        if (!~hyperdex_ds_map_insert_val_float(map, data, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_float deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               (attr.datatype == HYPERDATATYPE_STRING &&
                (attr.value_sz == sizeof(hyperdex_int128) || !attr.value_sz) ));

        if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(hyperdex_int128))
        {
          return little_to_native(*reinterpret_cast<hyperdex_int128 const*>(attr.value));
        }

        return 0;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, hyperdex_float& value) noexcept
      {
        return hyperdex_ds_iterate_list_float_next(&iter, &value);
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, hyperdex_float& value) noexcept
      {
        return hyperdex_ds_iterate_set_float_next(&iter, &value);
      }

  }; // primitive data traits specialization for double (hyperdex_float)

  // primitive data traits specialization for float
  template<> class primitive_data_traits<float>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<hyperdex_float>::type;
      static constexpr hyperdatatype const list_type = primitive_data_traits<hyperdex_float>::list_type;
      static constexpr hyperdatatype const  set_type = primitive_data_traits<hyperdex_float>::set_type;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_float>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<hyperdex_float>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<hyperdex_float>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        float data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_float>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          float data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_float>::list_append(arenas, status, list, data);
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         float data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_float>::set_insert(arenas, status, set, data);
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             float data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_float>::map_insert_key(arenas, status, map, data);
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             float data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_float>::map_insert_val(arenas, status, map, data);
      }

      static float deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_float>::deserialize(attr);
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, float& value) noexcept
      {
        hyperdex_float num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_float_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, float& value) noexcept
      {
        hyperdex_float num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_float_next(&iter, &num);
        if (res == 1) value = num;
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for float

  // primitive data traits specialization for uint128 (hyperdex_int128)
  template<> class primitive_data_traits<hyperdex_int128>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_int128 data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        hyperdex_int128 e_data = native_to_little(data);
        if (!~hyperdex_ds_copy_string(arena, (char const*)(&e_data), sizeof(hyperdex_int128),
                                      &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          hyperdex_int128 data)
                                          noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_list_append_string(list, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         hyperdex_int128 data)
                                         noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_set_insert_string(set, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_int128 data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_key_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             hyperdex_int128 data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_val_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int128 deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               (attr.datatype == HYPERDATATYPE_STRING &&
                 (attr.value_sz == sizeof(hyperdex_int128) || !attr.value_sz)
               ));

        if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(hyperdex_int128))
        {
          return little_to_native(*reinterpret_cast<hyperdex_int128 const*>(attr.value));
        }
        else if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return num;
        }

        return 0;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, hyperdex_int128& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, hyperdex_int128& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for uint128 (hyperdex_int128)

  // primitive data traits specialization for int128_t
  template<> class primitive_data_traits<int128_t>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int128_t data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        int128_t e_data = native_to_little(data);
        if (!~hyperdex_ds_copy_string(arena, (char const*)(&e_data), sizeof(int128_t),
                                      &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          int128_t data)
                                          noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_list_append_string(list, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         int128_t data)
                                         noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_set_insert_string(set, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int128_t data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_key_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             int128_t data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_val_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static int128_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               (attr.datatype == HYPERDATATYPE_STRING &&
                 (attr.value_sz == sizeof(int128_t) || !attr.value_sz)
               ));

        if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(int128_t))
        {
          return little_to_native(*reinterpret_cast<int128_t const*>(attr.value));
        }
        else if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return num;
        }

        return 0;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, int128_t& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, int128_t& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for int128_t

  // primitive data traits specialization for long double
  template<> class primitive_data_traits<long double>
  {
    public:

      static constexpr hyperdatatype const      type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        long double data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status = HYPERDEX_DS_NOMEM);
          return -1;
        }
        arenas.emplace_back(arena, arena_deleter{});

        swap_bytes_if_be(&data, sizeof(data));
        if (!~hyperdex_ds_copy_string(arena, (char const*)(&data), sizeof(long double),
                                      &status, &res_str, &res_size))
        {
          log_ds_status(status);
          arenas.pop_back();
          return -1;
        }

        return 0;

      }

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          long double data)
                                          noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_list_append_string(list, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         long double data)
                                         noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_set_insert_string(set, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             long double data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_key_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             long double data)
                                             noexcept
      {
        char const* str;
        size_t size;
        if (!~serialize(arenas, status, str, size, data))
        {
          log_ds_status(status);
          return -1;
        }

        if (!~hyperdex_ds_map_insert_val_string(map, str, size, &status))
        {
          log_ds_status(status);
          return -1;
        }

        return 0;
      }

      static long double deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_INT64 ||
               attr.datatype == HYPERDATATYPE_FLOAT ||
               (attr.datatype == HYPERDATATYPE_STRING &&
                 (attr.value_sz == sizeof(long double) || !attr.value_sz)
               ));

        if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(long double))
        {
          long double res = *reinterpret_cast<long double const*>(attr.value);
          swap_bytes_if_be(&res, sizeof(res));
          return res;
        }
        if (attr.datatype == HYPERDATATYPE_INT64 && attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return num;
        }
        else if (attr.datatype == HYPERDATATYPE_FLOAT && attr.value_sz == sizeof(hyperdex_float))
        {
          hyperdex_float num;
          hyperdex_ds_unpack_float(attr.value, sizeof(hyperdex_float), &num);
          return num;
        }

        return 0;
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, long double& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, long double& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = 0;
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for long double

  template<template<typename, typename = void> typename>
  class basic_document;

  // primitive data traits specialization for basic_document
  template<typename Document>
  class primitive_data_traits<Document, std::enable_if_t<Document::is_document>>
  {
    public:

      static constexpr hyperdatatype const type      = HYPERDATATYPE_DOCUMENT;
      static constexpr        size_t const aren_max  = 1;
      static constexpr          bool const is_string = true;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const  list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const   set_type = HYPERDATATYPE_SET_STRING;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          Document const& data)
                                          noexcept
      {
        return primitive_data_traits<string>::list_append(arenas, status, list, data.to_string());
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         Document const& data)
                                         noexcept
      {
        return primitive_data_traits<string>::set_insert(arenas, status, set, data.to_string());
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             Document const& data)
                                             noexcept
      {
        return primitive_data_traits<string>::map_insert_key(arenas, status, map, data.to_string());
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             Document const& data)
                                             noexcept
      {
        return primitive_data_traits<string>::map_insert_val(arenas, status, map, data.to_string());
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        Document const& data)
                                        noexcept
      {
        return primitive_data_traits<string>::serialize(arenas, status, res_str, res_size, data.to_string());
      }

      static Document deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_DOCUMENT || attr.datatype == HYPERDATATYPE_STRING);
        return { attr.value, attr.value_sz };
      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, Document& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_DOCUMENT } );
        else
        {
          try { value = Document {}; } catch(...) {}
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, Document& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_DOCUMENT } );
        else
        {
          try { value = Document {}; } catch(...) {}
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for basic_document

  // primitive data traits specialization for hyperdex::timestamp
  template<typename TimeStamp>
  class primitive_data_traits<TimeStamp, std::enable_if_t<TimeStamp::is_timestamp>>
  {
    public:

      static constexpr hyperdatatype const type = TimeStamp::type;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_INT64;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_INT64;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_INT64;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_INT64;

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          TimeStamp data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int>::list_append(arenas, status, list,
                                                                data.to_hyperdex());
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         TimeStamp data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int>::set_insert(arenas, status, set, data.to_hyperdex());
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             TimeStamp data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_key(arenas, status, map, data.to_hyperdex());
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             TimeStamp data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int>::map_insert_val(arenas, status, map, data.to_hyperdex());
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        TimeStamp data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status,
                                                              res_str, res_size,
                                                              data.to_hyperdex());
      }

      static TimeStamp deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_STRING           ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_SECOND ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_MINUTE ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_HOUR   ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_DAY    ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_WEEK   ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_MONTH);

        if (attr.value_sz == sizeof(time_t))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return TimeStamp{}.from_hyperdex(num);
        }

        return {};

      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, TimeStamp& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_int_next(&iter, &num);
        if (res == 1) value.from_hyperdex(num);
        else
        {
          value = TimeStamp {};
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, TimeStamp& value) noexcept
      {
        hyperdex_int num;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_int_next(&iter, &num);
        if (res == 1) value.from_hyperdex(num);
        else
        {
          value = TimeStamp {};
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for hyperdex::timestamp

  template<hyperdatatype, template<typename, typename = void> typename>
  class timestamp;

  template <typename, typename> class complex_data_traits;

  using full_timestamp   = timestamp<HYPERDATATYPE_STRING, complex_data_traits>;

  // primitive data traits specialization for hyperdex::full_timestamp
  template<typename FullTimestamp>
  class primitive_data_traits<FullTimestamp, std::enable_if_t<std::is_same<FullTimestamp, full_timestamp>::value>>
  {
    public:

      static constexpr hyperdatatype const type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const list_type = HYPERDATATYPE_LIST_STRING;
      static constexpr hyperdatatype const  set_type = HYPERDATATYPE_SET_STRING;
      static constexpr        size_t const  aren_max = primitive_data_traits<hyperdex_int128>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;
      static constexpr hyperdatatype const map_key_type = HYPERDATATYPE_STRING;
      static constexpr hyperdatatype const map_val_type = HYPERDATATYPE_STRING;

      static hyperdex_int_ret list_append(arena_vector& arenas,
                                          hyperdex_ds_returncode& status,
                                          hyperdex_ds_list* list,
                                          FullTimestamp data)
                                          noexcept
      {
        return primitive_data_traits<hyperdex_int128>::list_append(arenas, status, list,
                                                                   *reinterpret_cast<int128_t*>(&data));
      }

      static hyperdex_int_ret set_insert(arena_vector& arenas,
                                         hyperdex_ds_returncode& status,
                                         hyperdex_ds_set* set,
                                         FullTimestamp data)
                                         noexcept
      {
        return primitive_data_traits<hyperdex_int128>::set_insert(arenas, status, set,
                                                                  *reinterpret_cast<int128_t*>(&data));
      }

      static hyperdex_int_ret map_insert_key(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             FullTimestamp data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int128>
          ::map_insert_key(arenas, status, map, *reinterpret_cast<int128_t*>(&data));
      }

      static hyperdex_int_ret map_insert_val(arena_vector& arenas,
                                             hyperdex_ds_returncode& status,
                                             hyperdex_ds_map* map,
                                             FullTimestamp data)
                                             noexcept
      {
        return primitive_data_traits<hyperdex_int128>
          ::map_insert_val(arenas, status, map, *reinterpret_cast<int128_t*>(&data));
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        FullTimestamp data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int128>::serialize(arenas, status,
                                                                 res_str, res_size,
                                                                 *reinterpret_cast<int128_t*>(&data));
      }

      static FullTimestamp deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_STRING           ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_SECOND ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_MINUTE ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_HOUR   ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_DAY    ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_WEEK   ||
               attr.datatype == HYPERDATATYPE_TIMESTAMP_MONTH);

        if (attr.datatype == HYPERDATATYPE_STRING && attr.value_sz == sizeof(int128_t))
        {
          return *reinterpret_cast<FullTimestamp const*>(attr.value);
        }
        else if (attr.value_sz == sizeof(hyperdex_int))
        {
          hyperdex_int num;
          hyperdex_ds_unpack_int(attr.value, sizeof(hyperdex_int), &num);
          return FullTimestamp{}.from_hyperdex(num);
        }

        return FullTimestamp {};

      }

      static hyperdex_int_ret iterate_list(hyperdex_ds_iterator& iter, FullTimestamp& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_list_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = FullTimestamp {};
          if (!~res) log_malformed_list();
        }
        return res;
      }

      static hyperdex_int_ret iterate_set(hyperdex_ds_iterator& iter, FullTimestamp& value) noexcept
      {
        char const* str;
        size_t sz;
        hyperdex_int_ret res = hyperdex_ds_iterate_set_string_next(&iter, &str, &sz);
        if (res == 1) value = deserialize( { nullptr, str, sz, HYPERDATATYPE_STRING } );
        else
        {
          value = FullTimestamp {};
          if (!~res) log_malformed_set();
        }
        return res;
      }

  }; // primitive data traits specialization for hyperdex::full_timestamp


  /////////////////////////////////////////////////////////////
  // complex data traits

  #ifndef FOREST_USE_SERIALIZATION

  // template
  template <typename ComplexData, typename Enable = void>
  class complex_data_traits
  {
    static_assert(!sizeof(ComplexData), "Unsupported type for argument of complex hyperdex type.");
  };

  #else

  // template
  template <typename ComplexData, typename Enable = void>
  class complex_data_traits
  {
    public:

      static constexpr hyperdatatype const type = HYPERDATATYPE_STRING;
      static constexpr        size_t const aren_max = 1;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        ComplexData const& data)
                                        noexcept
      {
        try
        {
          res_str  = nullptr;
          res_size = 0;

          std::stringstream ss;

          try
          {
            oarchive oa(ss);
            oa << data;
          }
          catch(...) { log_ds_status(status = HYPERDEX_DS_NOMEM); return -1; }

          auto arena = hyperdex_ds_arena_create();
          if (!arena) { log_ds_status(status = HYPERDEX_DS_NOMEM); return -1; }
          arenas.emplace_back(arena, arena_deleter{});

          if (!~hyperdex_ds_copy_string(arena, ss.str().c_str(), ss.str().size(), &status, &res_str, &res_size))
          {
            log_ds_status(status);
            arenas.pop_back();
            return -1;
          }
        }
        catch(...) { status = HYPERDEX_DS_WRONG_STATE; return -1; }

        return 0;

      }

      static ComplexData deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == HYPERDATATYPE_STRING);

        ComplexData data {};

        try
        {
          std::stringstream ss { { attr.value, attr.value_sz } };
          iarchive ia(ss);
          ia >> data;
        }
        catch(...) { log_ds_status(HYPERDEX_DS_WRONG_STATE); }

        return data;
      }

  }; // template for complex_data_traits

  #endif // FOREST_USE_SERIALIZATION

  // complex data traits specialization for blob_pair
  template <> class complex_data_traits<blob_pair>
  {
    static_assert(true, "Complex data type for std::pair<char const*, size_t> is not permitted.");
  };

  // complex data traits specialization for char const*
  template<> class complex_data_traits<char const*>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<char const*>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<char const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<char const*>::is_string;
      static constexpr          bool const nondeserializable = true;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        return primitive_data_traits<char const*>::serialize(arenas, status, res_str, res_size, data);
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return nullptr;
      }

  }; // complex data traits specialization for char const*

  // complex data traits specialization for char*
  template<> class complex_data_traits<char*>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<char const*>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<char const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<char const*>::is_string;
      static constexpr          bool const nondeserializable = true;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        return primitive_data_traits<char const*>::serialize(arenas, status, res_str, res_size, data);
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return nullptr;
      }

  }; // complex data traits specialization for char*

  // complex data traits specialization for std::nullptr_t
  template<> class complex_data_traits<std::nullptr_t>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<char const*>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<char const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<char const*>::is_string;
      static constexpr          bool const nondeserializable = true;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char const* data)
                                        noexcept
      {
        return primitive_data_traits<char const*>::serialize(arenas, status, res_str, res_size, data);
      }

      template<typename Any>
      static char const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of char const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized char const*.");

        return nullptr;
      }

  }; // complex data traits specialization for std::nullptr_t

  #ifdef FOREST_USE_WSTRINGS

  // complex data traits specialization for wchar_t const*
  template<> class complex_data_traits<wchar_t const*>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<wchar_t const*>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<wchar_t const*>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<wchar_t const*>::is_string;
      static constexpr          bool const nondeserializable = true;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wchar_t const* data)
                                        noexcept
      {
        return primitive_data_traits<wchar_t const*>::serialize(arenas, status, res_str, res_size, data);
      }

      template<typename Any>
      static wchar_t const* deserialize(Any) noexcept
      {
        static_assert(!sizeof(Any),
                    "Deserialization of wchar_t const* is not possible due the potential problems with"
                    " memory deallocation and as there are null-terminated and fixed-sized wchar_t const*.");

        return nullptr;
      }

  }; // complex data traits specialization for wchar_t const*

  #endif // FOREST_USE_WSTRINGS

  // complex data traits specialization for string
  template<> class complex_data_traits<string>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<string>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<string>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<string>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<string>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        string const& data)
                                        noexcept
      {
        return primitive_data_traits<string>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        string&& data)
                                        noexcept
      {
        return primitive_data_traits<string>::serialize(arenas, status, res_str, res_size, std::move(data));
      }

      static string deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<string>::deserialize(attr);
      }

  }; // complex data traits specialization for string

  #ifdef FOREST_USE_WSTRINGS

  // complex data traits specialization for wstring
  template<> class complex_data_traits<wstring>
  {
    public:

      static constexpr hyperdatatype const      type = primitive_data_traits<wstring>::type;
      static constexpr        size_t const  aren_max = primitive_data_traits<wstring>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<wstring>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<wstring>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wstring const& data)
                                        noexcept
      {
        return primitive_data_traits<wstring>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wstring&& data)
                                        noexcept
      {
        return primitive_data_traits<wstring>::serialize(arenas, status, res_str, res_size, std::move(data));
      }

      static wstring deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<wstring>::deserialize(attr);
      }

  }; // complex data traits specialization for wstring

  #endif // FOREST_USE_WSTRINGS

  // complex data traits specialization for int64 (hyperdex_int)
  template<> class complex_data_traits<hyperdex_int>
  {
    public:

      static constexpr hyperdatatype const     type  = primitive_data_traits<hyperdex_int>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_int data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int>::deserialize(attr);
      }

  }; // complex data traits specialization for int64 (hyperdex_int)

  // complex data traits specialization for uint32_t
  template<> class complex_data_traits<uint32_t>
  {
    public:

      static constexpr hyperdatatype const      type = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint32_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static uint32_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<uint32_t>::deserialize(attr);
      }

  }; // complex data traits specialization for uint32_t

  // complex data traits specialization for int32_t
  template<> class complex_data_traits<int32_t>
  {
    public:

      static constexpr hyperdatatype const type      = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const aren_max  = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int32_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static int32_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<int32_t>::deserialize(attr);
      }

  }; // complex data traits specialization for int32_t

  // complex data traits specialization for int8_t
  template<> class complex_data_traits<int8_t>
  {
    public:

      static constexpr hyperdatatype const type      = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const aren_max  = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int8_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static int8_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<int8_t>::deserialize(attr);
      }

  }; // complex data traits specialization for int8_t

  // complex data traits specialization for uint8_t
  template<> class complex_data_traits<uint8_t>
  {
    public:

      static constexpr hyperdatatype const type      = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const aren_max  = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint8_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static uint8_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<uint8_t>::deserialize(attr);
      }

  }; // complex data traits specialization for uint8_t

  // complex data traits specialization for char
  template<> class complex_data_traits<char>
  {
    public:

      static constexpr hyperdatatype const type      = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const aren_max  = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        char data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static char deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<char>::deserialize(attr);
      }

  }; // complex data traits specialization for char

  #ifdef FOREST_USE_WSTRINGS

  // complex data traits specialization for wchar_t
  template<> class complex_data_traits<wchar_t>
  {
    public:

      static constexpr hyperdatatype const      type = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        wchar_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static wchar_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<wchar_t>::deserialize(attr);
      }

  }; // complex data traits specialization for wchar_t

  #endif // FOREST_USE_WSTRINGS

  // complex data traits specialization for uint64_t
  template<> class complex_data_traits<uint64_t>
  {
    public:

      static constexpr hyperdatatype const      type = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint64_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static uint64_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<uint64_t>::deserialize(attr);
      }

  }; // complex data traits specialization for uint64_t

  // complex data traits specialization for int16_t
  template<> class complex_data_traits<int16_t>
  {
    public:

      static constexpr hyperdatatype const      type = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int16_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static int16_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<int16_t>::deserialize(attr);
      }

  }; // complex data traits specialization for int16_t

  // complex data traits specialization for uint16_t
  template<> class complex_data_traits<uint16_t>
  {
    public:

      static constexpr hyperdatatype const      type = complex_data_traits<hyperdex_int>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_int>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_int>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_int>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        uint16_t data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_int>::serialize(arenas, status, res_str, res_size, data);
      }

      static uint16_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<uint16_t>::deserialize(attr);
      }

  }; // complex data traits specialization for uint16_t

  // complex data traits specialization for double (hyperdex_float)
  template<> class complex_data_traits<hyperdex_float>
  {
    public:

      static constexpr hyperdatatype const type      = primitive_data_traits<hyperdex_float>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<hyperdex_float>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<hyperdex_float>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<hyperdex_float>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_float data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_float>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_float deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_float>::deserialize(attr);
      }

  }; // complex data traits specialization for double (hyperdex_float)

  // complex data traits specialization for float
  template<> class complex_data_traits<float>
  {
    public:

      static constexpr hyperdatatype const  type     = complex_data_traits<hyperdex_float>::type;
      static constexpr        size_t const  aren_max = complex_data_traits<hyperdex_float>::aren_max;
      static constexpr          bool const is_string = complex_data_traits<hyperdex_float>::is_string;
      static constexpr          bool const nondeserializable
                                                     = complex_data_traits<hyperdex_float>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        float data)
                                        noexcept
      {
        return complex_data_traits<hyperdex_float>::serialize(arenas, status, res_str, res_size, data);
      }

      static float deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<float>::deserialize(attr);
      }

  }; // complex data traits specialization for float

  // complex data traits specialization for uint128 (hyperdex_int128)
  template<> class complex_data_traits<hyperdex_int128>
  {
    public:

      static constexpr hyperdatatype const type      = primitive_data_traits<hyperdex_int128>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<hyperdex_int128>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<hyperdex_int128>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<hyperdex_int128>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        hyperdex_int128 data)
                                        noexcept
      {
        return primitive_data_traits<hyperdex_int128>::serialize(arenas, status, res_str, res_size, data);
      }

      static hyperdex_int128 deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<hyperdex_int128>::deserialize(attr);
      }

  }; // complex data traits specialization for uint128 (hyperdex_int128)

  // complex data traits specialization for int128_t
  template<> class complex_data_traits<int128_t>
  {
    public:

      static constexpr hyperdatatype const type      = primitive_data_traits<int128_t>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<int128_t>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<int128_t>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<int128_t>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        int128_t data)
                                        noexcept
      {
        return primitive_data_traits<int128_t>::serialize(arenas, status, res_str, res_size, data);
      }

      static int128_t deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<int128_t>::deserialize(attr);
      }

  }; // complex data traits specialization for int128_t

  // complex data traits specialization for long double
  template<> class complex_data_traits<long double>
  {
    public:

      static constexpr hyperdatatype const type      = primitive_data_traits<long double>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<long double>::aren_max;
      static constexpr          bool const is_string = primitive_data_traits<long double>::is_string;
      static constexpr          bool const nondeserializable
                                                     = primitive_data_traits<long double>::nondeserializable;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        long double data)
                                        noexcept
      {
        return primitive_data_traits<long double>::serialize(arenas, status, res_str, res_size, data);
      }

      static long double deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<long double>::deserialize(attr);
      }

  }; // complex data traits specialization for long double

  // complex data traits specialization for basic_document
  template<typename Document>
  class complex_data_traits<Document, std::enable_if_t<Document::is_document>>
  {
    public:

      static constexpr hyperdatatype const type         = primitive_data_traits<Document>::type;
      static constexpr        size_t const aren_max     = primitive_data_traits<Document>::aren_max;
      static constexpr          bool const is_string    = primitive_data_traits<Document>::is_string;
      static constexpr          bool const nondeserializable
                                                        = primitive_data_traits<Document>::nondeserializable;
      static constexpr hyperdatatype const    list_type = primitive_data_traits<Document>::list_type;
      static constexpr hyperdatatype const     set_type = primitive_data_traits<Document>::set_type;
      static constexpr hyperdatatype const map_key_type = primitive_data_traits<Document>::map_key_type;
      static constexpr hyperdatatype const map_val_type = primitive_data_traits<Document>::map_val_type;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        Document const& data)
                                        noexcept
      {
        return primitive_data_traits<Document>::serialize(arenas, status, res_str, res_size, data);
      }

      static Document deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<Document>::deserialize(attr);
      }

  }; // complex data traits specialization for basic_document

  // complex data traits specialization for hyperdex::list
  template<typename ListData>
  class complex_data_traits<ListData, std::enable_if_t<ListData::is_list>>
  {
    public:

      static constexpr hyperdatatype const type      = ListData::type;
      static constexpr        size_t const aren_max  = 0;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        ListData const& data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        if (!~data.finalize(res_str, res_size))
        {
          log_ds_status(status = data.status());
          return -1;
        }

        return 0;

      } // serialize

      static ListData deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == ListData::type);
        return { attr.value, attr.value_sz };
      }

  }; // complex data traits specialization for hyperdex::list

  // complex data traits specialization for hyperdex::set
  template<typename SetData>
  class complex_data_traits<SetData, std::enable_if_t<SetData::is_set>>
  {
    public:

      static constexpr hyperdatatype const type      = SetData::type;
      static constexpr        size_t const aren_max  = 0;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        SetData const& data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        if (!~data.finalize(res_str, res_size))
        {
          log_ds_status(status = data.status());
          return -1;
        }

        return 0;

      } // serialize

      static SetData deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == SetData::type);
        return { attr.value, attr.value_sz };
      }

  }; // complex data traits specialization for hyperdex::set

  // complex data traits specialization for hyperdex::map
  template<typename MapData>
  class complex_data_traits<MapData, std::enable_if_t<MapData::is_map>>
  {
    public:

      static constexpr hyperdatatype const type      = MapData::type;
      static constexpr        size_t const aren_max  = 0;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;

      static hyperdex_int_ret serialize(arena_vector&,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        MapData const& data)
                                        noexcept
      {
        res_str  = nullptr;
        res_size = 0;

        if (!~data.finalize(res_str, res_size))
        {
          log_ds_status(status = data.status());
          return -1;
        }

        return 0;

      } // serialize

      static MapData deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == MapData::type);
        return { attr.value, attr.value_sz };
      }

  }; // complex data traits specialization for hyperdex::map

  // complex data traits specialization for hyperdex::timestamp
  template<typename TimeStamp>
  class complex_data_traits<TimeStamp, std::enable_if_t<TimeStamp::is_timestamp>>
  {
    public:

      static constexpr hyperdatatype const type      = primitive_data_traits<TimeStamp>::type;
      static constexpr        size_t const aren_max  = primitive_data_traits<TimeStamp>::aren_max;
      static constexpr          bool const is_string = false;
      static constexpr          bool const nondeserializable = false;

      static hyperdex_int_ret serialize(arena_vector& arenas,
                                        hyperdex_ds_returncode& status,
                                        char const*& res_str,
                                        size_t& res_size,
                                        TimeStamp data)
                                        noexcept
      {
        return primitive_data_traits<TimeStamp>::serialize(arenas, status, res_str, res_size, data);
      }

      static TimeStamp deserialize(hyperdex_client_attribute const& attr) noexcept
      {
        return primitive_data_traits<TimeStamp>::deserialize(attr);
      }

  }; // complex data traits specialization for hyperdex::timestamp

} // namespace hyperdex


#endif // HYPERDEX_DATA_TRAITS_HPP_INCLUDED
