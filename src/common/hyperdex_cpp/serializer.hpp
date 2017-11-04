#ifndef HYPERDEX_SERIALIZER_HPP_INCLUDED
#define HYPERDEX_SERIALIZER_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: serializer.hpp
// First created: 2015-10-18-11.42
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

// forest
#include <forest/tools.hpp>

// C++
#include <type_traits>

// hyperdex
#include <hyperdex_cpp/common.hpp>


namespace hyperdex
{
  struct attribute_tag_t {};
  struct predicate_tag_t {};
  struct map_attribute_tag_t {};

  static constexpr     attribute_tag_t const     attribute_tag{};
  static constexpr     predicate_tag_t const     predicate_tag{};
  static constexpr map_attribute_tag_t const map_attribute_tag{};

  template <typename, typename> class c_string_traits;
  template <typename, typename> class complex_data_traits;

  template<template<typename, typename = void> typename CStringTraits = c_string_traits,
           template<typename, typename = void> typename ComplexTraits = complex_data_traits>
  class basic_serializer final
  {
    public:
      FOREST_NONCOPYABLE_CLASS(basic_serializer);
      FOREST_NONMOVABLE_CLASS(basic_serializer);
      FOREST_NONALLOCATABLE_CLASS(basic_serializer);

       basic_serializer() = default;
      ~basic_serializer() = default;

      template<typename AttrName>
      hyperdex_client_attribute serialize(attribute_tag_t, AttrName&& name,
                                          char const* value,
                                          size_t sz,
                                          hyperdatatype datatype = HYPERDATATYPE_STRING)
                                          const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING };

        char const* str;
        if (!~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_, status_, str,
                                                               std::forward<AttrName>(name)))
          locked = true;
        return { str, value, sz, datatype };
      }

      template<typename AttrName, typename AttrData>
      hyperdex_client_attribute serialize(attribute_tag_t, AttrName&& name, AttrData&& data) const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING };

        hyperdex_client_attribute attr { nullptr, nullptr, 0,
                                         ComplexTraits<std::decay_t<AttrData>>::type };

        if (!~ComplexTraits<std::decay_t<AttrData>>::serialize(arenas_,
                                                               status_,
                                                               attr.value, attr.value_sz,
                                                               std::forward<AttrData>(data)) ||
            !~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                               status_,
                                                               attr.attr,
                                                               std::forward<AttrName>(name)))
        {
          locked = true;
        }

        return attr;
      }

      template<typename AttrName>
      hyperdex_client_attribute_check serialize(predicate_tag_t, AttrName&& name,
                                                char const* value,
                                                size_t sz,
                                                hyperdatatype datatype = HYPERDATATYPE_STRING,
                                                hyperpredicate pred =  HYPERPREDICATE_EQUALS)
                                                const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS };

        char const* str;
        if (!~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_, status_, str,
                                                               std::forward<AttrName>(name)))
          locked = true;
        return { str, value, sz, datatype, pred };
      }

      template<typename AttrName, typename AttrData>
      hyperdex_client_attribute_check serialize(predicate_tag_t, AttrName&& name, AttrData&& data,
                                                hyperpredicate pred = HYPERPREDICATE_EQUALS) const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, HYPERPREDICATE_EQUALS };

        hyperdex_client_attribute_check attr { nullptr, nullptr, 0,
                                         ComplexTraits<std::decay_t<AttrData>>::type,
                                         pred };

        if (!~ComplexTraits<std::decay_t<AttrData>>::serialize(arenas_,
                                                               status_,
                                                               attr.value, attr.value_sz,
                                                               std::forward<AttrData>(data)) ||

            !~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                               status_,
                                                               attr.attr,
                                                               std::forward<AttrName>(name)))
        {
          locked = true;
        }

        return attr;
      }

      template<typename AttrName>
      hyperdex_client_map_attribute serialize(map_attribute_tag_t, AttrName&& name,
                                              char const* key,
                                              size_t key_sz,
                                              hyperdatatype key_datatype,
                                              char const* value,
                                              size_t value_sz,
                                              hyperdatatype value_datatype
                                              )
                                              const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, nullptr, 0, HYPERDATATYPE_STRING };

        char const* str;
        if (!~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_, status_, str, std::forward<AttrName>(name)))
          locked = true;
        return { str, key, key_sz, key_datatype, value, value_sz, value_datatype };
      }

      template<typename AttrName, typename KeyData>
      hyperdex_client_map_attribute serialize(map_attribute_tag_t, AttrName&& name, KeyData&& data,
                                              char const* value,
                                              size_t value_sz,
                                              hyperdatatype value_datatype)
                                              const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, nullptr, 0, HYPERDATATYPE_STRING };

        hyperdex_client_map_attribute attr { nullptr, nullptr, 0,
                                             ComplexTraits<std::decay_t<KeyData>>::type,
                                             value, value_sz, value_datatype };

        if (!~ComplexTraits<std::decay_t<KeyData>>::serialize(arenas_,
                                                              status_,
                                                              attr.map_key, attr.map_key_sz,
                                                              std::forward<KeyData>(data)) ||
            !~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                               status_,
                                                               attr.attr,
                                                               std::forward<AttrName>(name)))
        {
          locked = true;
        }

        return attr;
      }

      template<typename AttrName, typename ValueData>
      hyperdex_client_map_attribute serialize(map_attribute_tag_t, AttrName&& name,
                                              char const* key,
                                              size_t key_sz,
                                              hyperdatatype key_datatype,
                                              ValueData&& data)
                                              const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, nullptr, 0, HYPERDATATYPE_STRING };

        hyperdex_client_map_attribute attr { nullptr,
                                             key, key_sz, key_datatype,
                                             nullptr, 0,
                                             ComplexTraits<std::decay_t<ValueData>>::type };

        if (!~ComplexTraits<std::decay_t<ValueData>>::serialize(arenas_,
                                                                status_,
                                                                attr.value, attr.value_sz,
                                                                std::forward<ValueData>(data)) ||
            !~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                               status_,
                                                               attr.attr,
                                                               std::forward<AttrName>(name)))
        {
          locked = true;
        }

        return attr;
      }

      template<typename AttrName, typename KeyData, typename ValueData>
      hyperdex_client_map_attribute serialize(map_attribute_tag_t,
                                              AttrName&& name, KeyData&& k, ValueData&& v) const noexcept
      {
        if (locked) return { nullptr, nullptr, 0, HYPERDATATYPE_STRING, nullptr, 0, HYPERDATATYPE_STRING };

        hyperdex_client_map_attribute attr { nullptr,
                                             nullptr, 0,
                                             ComplexTraits<std::decay_t<KeyData>>::type,
                                             nullptr, 0,
                                             ComplexTraits<std::decay_t<ValueData>>::type };

        if (!~ComplexTraits<std::decay_t<KeyData>>::serialize(arenas_,
                                                              status_,
                                                              attr.map_key, attr.map_key_sz,
                                                              std::forward<KeyData>(k)) ||
            !~ComplexTraits<std::decay_t<ValueData>>::serialize(arenas_,
                                                                status_,
                                                                attr.value, attr.value_sz,
                                                                std::forward<ValueData>(v)) ||
            !~CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                               status_,
                                                               attr.attr,
                                                               std::forward<AttrName>(name)))
        {
          locked = true;
        }

        return attr;
      }

      hyperdex_ds_returncode status() const noexcept { locked = false; return status_; }

      explicit operator bool() const noexcept { locked = false; return status_ == HYPERDEX_DS_SUCCESS; }

      void reserve(arena_vector::size_type n) noexcept
      {
        try { arenas_.reserve(n); } catch(...) {}
      }

      template<typename ...T>
      void reserve(attribute_tag_t) noexcept
      {
        static_assert(!(sizeof...(T) % 2 || !sizeof...(T)),
              "Template function hyperdex::serializer.reserve<...>(attribute_tag_t)"
              " must have not null even template parameters number."
              " In the each pair the first template parameter is for "
              "name and the second for serialized data class.");

        constexpr size_t const size = aren_count_<T...>();
        try
        {
          arenas_.reserve(size);
        }
        catch(...) {}
      }

      template<typename ...T>
      void reserve(predicate_tag_t) noexcept
      {
        static_assert(!(sizeof...(T) % 2 || !sizeof...(T)),
              "Template function hyperdex::serializer.reserve<...>(predicate_tag_t)"
              " must have not null even template parameters number."
              " In the each pair the first template parameter is for "
              "name and the second for serialized data class.");

        constexpr size_t const size = aren_count_<T...>();
        try
        {
          arenas_.reserve(size);
        }
        catch(...) {}
      }

      template<typename ...T>
      void reserve(map_attribute_tag_t) noexcept
      {
        static_assert(!(sizeof...(T) % 3 || !sizeof...(T)),
              "Template function hyperdex::serializer.reserve<...>(map_attribute_tag_t)"
              " must have not null template parameters number."
              " In the each group the first template parameter is for "
              "name, the second for serialized key class and the third "
              "for serialized data.");

        constexpr size_t const size = map_aren_count_<T...>();
        try
        {
          arenas_.reserve(size);
        }
        catch(...) {}
      }

    private:

      template<typename Name, typename Data>
      static constexpr size_t aren_count_() noexcept
      {
        return CStringTraits<std::decay_t<Name>>::aren_max +
               ComplexTraits<std::decay_t<Data>>::aren_max;
      }

      template<typename Name1, typename Data1,
               typename Name2, typename Data2,
               typename ... T>
      static constexpr size_t aren_count_() noexcept
      {
        return CStringTraits<std::decay_t<Name1>>::aren_max +
               ComplexTraits<std::decay_t<Data1>>::aren_max +
               aren_count_<Name2, Data2, T...>();

      }

      template<typename Name, typename Key, typename Data>
      static constexpr size_t map_aren_count_() noexcept
      {
        return CStringTraits<std::decay_t<Name>>::aren_max +
               ComplexTraits<std::decay_t<Key>>::aren_max +
               ComplexTraits<std::decay_t<Data>>::aren_max;
      }

      template<typename Name1, typename Key1, typename Data1,
               typename Name2, typename Key2, typename Data2,
               typename ... T>
      static constexpr size_t map_aren_count_() noexcept
      {
        return CStringTraits<std::decay_t<Name1>>::aren_max +
               ComplexTraits<std::decay_t<Key1>>::aren_max  +
               ComplexTraits<std::decay_t<Data1>>::aren_max +
               aren_count_<Name2, Key2, Data2, T...>();

      }

    private:

      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };
      mutable arena_vector arenas_ {};
      mutable bool locked = false;

  }; // class basic_serializer

  using serializer = basic_serializer<>;

}

#endif // HYPERDEX_SERIALIZER_HPP_INCLUDED
