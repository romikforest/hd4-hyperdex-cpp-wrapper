#ifndef HYPERDEX_LIST_HPP_INCLUDED
#define HYPERDEX_LIST_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: list.hpp
// First created: 2015-10-18-08.54
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
#include <initializer_list>
#include <type_traits>

// forest
#include <forest/tools.hpp>
#include <forest/string.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/log.hpp>

namespace hyperdex
{

  template <typename, typename> class primitive_data_traits;

  template<typename DataType,
           template<typename, typename = void> typename PrimitiveTraits = primitive_data_traits>
  class list final
  {
      template<typename T> struct wrapper {};
      using     string_t = std::true_type;
      using non_string_t = std::false_type;

      struct helper_1
      {
        helper_1();
        helper_1(char const*, size_t);
        helper_1(an_attribute const&);
        helper_1(an_attribute const*);
        helper_1(hyperdex_client_attribute const&);
        helper_1(hyperdex_client_attribute const*);
        helper_1(std::initializer_list<blob_pair> args);
      };

      class helper_2
      {
          struct helper_2_string
          {
            helper_2_string(list*, string_t, char const*);
            helper_2_string(list*, string_t, blob_pair const&);
          };

        public:

          helper_2(list* obj, string_t, char const* arg) noexcept
          {
            hyperdex_ds_list_append_string(obj->list_, arg, strlen(arg), &obj->status_);
          }

          helper_2(list* obj, string_t, blob_pair const& val) noexcept
          {
            hyperdex_ds_list_append_string(obj->list_, val.first, val.second, &obj->status_);
          }

          template<typename T,
            typename = std::enable_if_t<!std::is_constructible<helper_2_string, list*, string_t, T&&>::value>>
          helper_2(list* obj, string_t, T&& arg) noexcept
          {
            static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                       PrimitiveTraits<std::decay_t<T>>::is_string,
                       "Type of inserting to hyperdex::list value must be a string");


            char const* str;
            size_t size;

            if (~PrimitiveTraits<std::decay_t<T>>
                  ::serialize(obj->arenas_, obj->status_, str, size, std::forward<T>(arg)))
              hyperdex_ds_list_append_string(obj->list_, str, size, &obj->status_);
          }

          template<typename T, typename ... TT,
                   typename = std::enable_if_t<!std::is_constructible<helper_2_string, list*,
                                       string_t, T&&, TT&&...>::value>>
          helper_2(list* obj, string_t, T&& arg, TT&& ... args) noexcept
          {
            helper_2 { obj, string_t{}, std::forward<T>(arg) };
            if (obj->status_ != HYPERDEX_DS_SUCCESS)
            {
              log_ds_status(obj->status_);
              return;
            }

            helper_2 { obj, string_t{}, std::forward<TT>(args)... };
            if (obj->status_ != HYPERDEX_DS_SUCCESS) log_ds_status(obj->status_);
          }

          template<typename T>
          helper_2(list* obj, non_string_t, T&& arg) noexcept
          {
            static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<T>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::list values");

            PrimitiveTraits<std::decay_t<DataType>>
                     ::list_append(obj->arenas_, obj->status_, obj->list_, std::forward<T>(arg));
          }

          template<typename T, typename ... TT>
          helper_2(list* obj, non_string_t, T&& arg, TT&& ... args) noexcept
          {
            helper_2 { obj, non_string_t{}, std::forward<T>(arg)};
            if (obj->status_ != HYPERDEX_DS_SUCCESS)
            {
              log_ds_status(obj->status_);
              return;
            }

            helper_2 { obj, non_string_t{}, std::forward<TT>(args)...};
            if (obj->status_ != HYPERDEX_DS_SUCCESS)
              log_ds_status(obj->status_);
          }

      }; // helper_2

    public:

      FOREST_NONCOPYABLE_CLASS(list);
      FOREST_NONMOVABLE_CLASS(list);
      FOREST_NONALLOCATABLE_CLASS(list);

      static constexpr bool const is_list = true;
      static constexpr hyperdatatype const type
        = std::is_same<DataType, hyperdex_blob_data>::value ? HYPERDATATYPE_LIST_STRING :
             PrimitiveTraits<std::decay_t<DataType>>::list_type;

      explicit operator bool () const noexcept { return status_ == HYPERDEX_DS_SUCCESS; }

      hyperdex_ds_returncode status() const noexcept { return status_; }

      list() noexcept
      {
        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          return;
        }
        arenas_.emplace_back(arena, arena_deleter{});

        if (!(list_ = hyperdex_ds_allocate_list(arena)))
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          arenas_.pop_back();
        }
      }

      list(char const* str, size_t size) noexcept // construct from serialized string
      {
        init_str_  = str;
        init_size_ = size;
      }

      list(an_attribute const& attr) noexcept
      : list { attr.a.value, attr.a.value_sz }
      {
        assert(attr.a.datatype == type);
      }

      list(an_attribute const* attr) noexcept
      : list { attr->a.value, attr->a.value_sz }
      {
        assert(attr->a.datatype == type);
      }

      list(hyperdex_client_attribute const& attr) noexcept
      : list { attr.value, attr.value_sz }
      {
        assert(attr.datatype == type);
      }

      list(hyperdex_client_attribute const* attr) noexcept
      : list { attr->value, attr->value_sz }
      {
        assert(attr->datatype == type);
      }

      struct helpern
      {
        helpern(an_attribute const&);
      };

      list(std::initializer_list<blob_pair> args) noexcept
      : list {}
      {
        static_assert(type == HYPERDATATYPE_LIST_STRING,
                      "Constructor list(std::initializer_list<std::pair<char const*, size_t>> args) "
                      "is available only for HYPERDATATYPE_LIST_STRING hyperdex::list type.");

        if (list_)
        {
          for (auto& arg: args)
          {
            if (!~hyperdex_ds_list_append_string(list_, arg.first, arg.second, &status_))
            {
              log_ds_status(status_);
              return;
            }
          } // for (auto& arg: args)
        } // if (list_)
      }

      template<typename ...TT, typename = std::enable_if_t<!std::is_constructible<helper_1, TT&&...>::value>>
      list(TT&& ... args) noexcept
      : list {}
      {

        static_assert(!std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                      "Constructor template<typename ...T> list(T&& ... args) "
                      "is not available for hyperdex::list<hyperdex_blob_data> type.");

        constexpr auto const selector
          = std::integral_constant<bool, PrimitiveTraits<std::decay_t<DataType>>::is_string>{};

        if (list_) helper_2 { this, selector, std::forward<TT>(args)... };
      }

      list& operator = (hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == type);
        clear_();
        init_str_  = attr.value;
        init_size_ = attr.value_sz;
        return *this;
      }

      list& operator = (hyperdex_client_attribute const* attr) noexcept
      {
        assert(attr->datatype == type);
        return operator = (*attr);
      }

      list& operator = (an_attribute const& attr) noexcept
      {
        assert(attr.a.datatype == type);
        return operator = (attr.a);
      }

      list& operator = (an_attribute const* attr) noexcept
      {
        assert(attr->a.datatype == type);
        return operator = (attr->a);
      }

      hyperdex_int_ret append(char const* str, size_t size) noexcept
      {
        static_assert(type == HYPERDATATYPE_LIST_STRING,
                      "Member function  hyperdex_int_ret append(char const* str, size_t size) "
                      "is available only for HYPERDATATYPE_LIST_STRING hyperdex::list type.");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!list_ && !init_str_to_list_()) return -1;

        if (!~hyperdex_ds_list_append_string(list_, str, size, &status_))
        {
          log_ds_status(status_);
          return -1;
        }

        return 0;
      }

      template<typename T>
      hyperdex_int_ret append(T&& arg) noexcept
      {

        static_assert(!std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                      "Method template<typename T> hyperdex_int_ret append(T&& arg) "
                      "is not available for hyperdex::list<hyperdex_blob_data> type.");

        static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                       PrimitiveTraits<std::decay_t<T>>::is_string,
                       "Type of inserting to hyperdex::list value must be a string");

        static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<T>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::list values");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!list_ && !init_str_to_list_()) return -1;

        constexpr auto const selector
          = std::integral_constant<bool, PrimitiveTraits<std::decay_t<DataType>>::is_string>{};

        helper_2 { this, selector, std::forward<T>(arg) };

        if (status_ != HYPERDEX_DS_SUCCESS) return -1;

        return 0;
      }

      hyperdex_int_ret finalize(char const*& res_str,
                                size_t& res_size)
                                const noexcept
      {
        if (list_)
        {
          hyperdatatype datatype;
          status_  = HYPERDEX_DS_SUCCESS;
          if (!~hyperdex_ds_list_finalize(list_, &status_, &res_str, &res_size, &datatype))
          {
            log_ds_status(status_);
            return -1;
          }
          return 0;
        }
        res_str  = init_str_;
        res_size = init_size_;
        status_  = HYPERDEX_DS_SUCCESS;
        return 0;
      }

      class iterator
      {

          template<typename T>
          void iterator_helper_(wrapper<T>) noexcept
          {
            status_ = PrimitiveTraits<typename
                        std::conditional_t<
                          PrimitiveTraits<std::decay_t<DataType>>::nondeserializable,
                          forest::string,
                          std::decay_t<DataType>
                        >
                      >::iterate_list(iter_, val_);
          }

          void iterator_helper_(wrapper<hyperdex_blob_data>) noexcept
          {
            status_ = hyperdex_ds_iterate_list_string_next(&iter_, &val_.first, &val_.second);
          }

        public:
          iterator(bool pos, char const* init_str = nullptr, size_t init_size = 0) noexcept
          : status_ { pos ? 1 : 0 }
          {
            if (status_ == 1)
            {
              hyperdex_ds_iterator_init(&iter_, type, init_str, init_size);
              iterator_helper_(wrapper<std::decay_t<DataType>>{});
            }
          }

          bool operator!=(const iterator& other) const noexcept
          {
            return (!~status_ ? 0 : status_) != (!~other.status_ ? 0 : other.status_);
          }

          auto operator* () const noexcept
          {
            return val_;
          }

          iterator const& operator++() noexcept
          {
            if (status_ == 1) iterator_helper_(wrapper<std::decay_t<DataType>>{});

            return *this;
          }

          hyperdex_int_ret status() const noexcept { return status_; }

        private:
          hyperdex_ds_iterator  iter_ {};
          hyperdex_int_ret status_ { 1 };

          std::conditional_t<
             std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
             blob_pair,
             std::conditional_t<
                PrimitiveTraits<std::decay_t<DataType>>::nondeserializable,
                forest::string,
                std::decay_t<DataType>
             >
          > val_ {};

      }; // class iterator

      iterator begin() const noexcept
      {
        if (list_)
        {
          char const* str;
          size_t sz;
          if (!~finalize(str, sz)) return { false };
          return { true, str, sz };
        }
        return { true, init_str_, init_size_ };
      }

      iterator end() const noexcept
      {
        return { false };
      }

    private:

      template<typename T>
      bool init_str_to_list_helper_(wrapper<T>) noexcept
      {
        hyperdex_ds_iterator iter;
        hyperdex_ds_iterator_init(&iter, type, init_str_, init_size_);

        hyperdex_int_ret res;

        std::decay_t<DataType> val;

        while ((res = PrimitiveTraits<std::decay_t<DataType>>::iterate_list(iter, val)) == 1)
        {
          if (!~res || !~append(val))
          {
            list_ = nullptr;
            // arenas_.pop_back();
            return false;
          }
        }

        init_str_  = nullptr;
        init_size_ = 0;

        return true;
      }

      bool init_str_to_list_helper_(wrapper<hyperdex_blob_data>) noexcept
      {
        hyperdex_ds_iterator iter;
        hyperdex_ds_iterator_init(&iter, type, init_str_, init_size_);

        blob_pair val {};
        hyperdex_int_ret res;

        while ((res = hyperdex_ds_iterate_list_string_next(&iter, &val.first, &val.second)) == 1)
        {
          if (!~res || !~hyperdex_ds_list_append_string(list_, val.first, val.second, &status_))
          {
            log_ds_status(status_);
            list_ = nullptr;
            arenas_.pop_back();
            return false;
          }
        }

        init_str_  = nullptr;
        init_size_ = 0;

        return true;
      }

      bool init_str_to_list_helper_(wrapper<hyperdex_int>) noexcept
      {
        hyperdex_ds_iterator iter;
        hyperdex_ds_iterator_init(&iter, type, init_str_, init_size_);

        hyperdex_int val {};
        hyperdex_int_ret res;

        while ((res = hyperdex_ds_iterate_list_int_next(&iter, &val)) == 1)
        {
          if (!~res || !~hyperdex_ds_list_append_int(list_, val, &status_))
          {
            log_ds_status(status_);
            list_ = nullptr;
            arenas_.pop_back();
            return false;
          }
        }

        init_str_  = nullptr;
        init_size_ = 0;

        return true;
      }

      bool init_str_to_list_helper_(wrapper<hyperdex_float>) noexcept
      {
        hyperdex_ds_iterator iter;
        hyperdex_ds_iterator_init(&iter, type, init_str_, init_size_);

        hyperdex_float val {};
        hyperdex_int_ret res;

        while ((res = hyperdex_ds_iterate_list_float_next(&iter, &val)) == 1)
        {
          if (!~res || !~hyperdex_ds_list_append_float(list_, val, &status_))
          {
            log_ds_status(status_);
            list_ = nullptr;
            arenas_.pop_back();
            return false;
          }
        }

        init_str_  = nullptr;
        init_size_ = 0;

        return true;
      }

      bool init_str_to_list_() noexcept
      {
        assert(!list_);

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          return false;
        }
        arenas_.emplace_back(arena, arena_deleter{});

        if (!(list_ = hyperdex_ds_allocate_list(arena)))
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          arenas_.pop_back();
          return false;
        }

        return init_str_to_list_helper_(wrapper<std::conditional_t<
                                                   type == HYPERDATATYPE_LIST_STRING,
                                                   hyperdex_blob_data,
                                                   std::conditional_t<
                                                     type == HYPERDATATYPE_LIST_INT64,
                                                     hyperdex_int,
                                                     std::conditional_t<
                                                       type == HYPERDATATYPE_LIST_FLOAT,
                                                       hyperdex_float,
                                                       std::decay_t<DataType>
                                                     >
                                                   >
                                                 >>{});

      }

      void clear_() noexcept
      {
          arenas_  = arena_vector {};
           status_ = HYPERDEX_DS_SUCCESS;
             list_ = nullptr;
         init_str_ = nullptr;
        init_size_ = 0;
      }

      mutable arena_vector arenas_ {};
      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };
      hyperdex_ds_list*       list_  { nullptr };
      mutable char const* init_str_  { nullptr };
      mutable size_t     init_size_  { 0 };

  }; // template class list

  template<template<typename, typename = void> typename>
  class basic_document;
  template <typename, typename> class c_string_traits;
  using document = basic_document<c_string_traits>;

  template<hyperdatatype T, template<typename, typename = void> typename>
  class timestamp;

  template <typename, typename> class complex_data_traits;

  using timestamp_second = timestamp<HYPERDATATYPE_TIMESTAMP_SECOND, complex_data_traits>;
  using timestamp_minute = timestamp<HYPERDATATYPE_TIMESTAMP_MINUTE, complex_data_traits>;
  using timestamp_hour   = timestamp<HYPERDATATYPE_TIMESTAMP_HOUR, complex_data_traits>;
  using timestamp_day    = timestamp<HYPERDATATYPE_TIMESTAMP_DAY, complex_data_traits>;
  using timestamp_week   = timestamp<HYPERDATATYPE_TIMESTAMP_WEEK, complex_data_traits>;
  using timestamp_month  = timestamp<HYPERDATATYPE_TIMESTAMP_MONTH, complex_data_traits>;
  using full_timestamp   = timestamp<HYPERDATATYPE_STRING, complex_data_traits>;


  using        blob_list = list<hyperdex_blob_data>;
  using      string_list = list<forest::string>;
  using         int_list = list<hyperdex_int>;
  using       float_list = list<hyperdex_float>;
  using      int128_list = list<hyperdex_int128>;
  using long_double_list = list<long double>;

  using    document_list = list<document>;

  using timestamp_second_list = list<timestamp_second>;
  using timestamp_minute_list = list<timestamp_minute>;
  using timestamp_hour_list   = list<timestamp_hour>;
  using timestamp_day_list    = list<timestamp_day>;
  using timestamp_week_list   = list<timestamp_week>;
  using timestamp_month_list  = list<timestamp_month>;
  using full_timestamp_list   = list<full_timestamp>;

} // namespace hyperdex

#endif // HYPERDEX_LIST_HPP_INCLUDED
