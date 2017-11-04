#ifndef HYPERDEX_ATTRIBUTES_HPP_INCLUDED
#define HYPERDEX_ATTRIBUTES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: attributes.hpp
// First created: 2015-10-18-11.56
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
#include <forest/vector.hpp>
#include <forest/tools.hpp>
#include <forest/optional.hpp>
#include <forest/string.hpp>
#include <forest/tuple.hpp>
#include <forest/string.hpp>

// C++
#include <unordered_map>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <tuple>
#include <algorithm>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/iterators_and_traits.hpp>
#include <hyperdex_cpp/attribute_status.hpp>

namespace hyperdex
{
  using forest::tuple;
  using forest::tuple_size;
  using forest::get;

  namespace helper
  {
    template<typename T1, typename T2>
    inline
    std::enable_if_t<std::is_constructible<T1, T2>::value, void>
    assign_helper(T1& arg1, T2 const& arg2)
    {
      arg1 = arg2;
    }

    template<typename T1, typename T2>
    inline
    std::enable_if_t<!std::is_constructible<T1, T2>::value, void>
    assign_helper(T1& arg1, T2 const& arg2)
    {
      arg1 = arg2.operator T1();
    }

  } // namespace helper

  template<typename, typename> class c_string_traits;
  template<typename, typename> class complex_data_traits;

  template<template<typename, typename = void> typename CStringTraits = c_string_traits,
           template<typename, typename = void> typename ComplexTraits = complex_data_traits>
  class basic_attributes final
  {
      template<template<typename, typename = void> typename,
               template<typename, typename = void> typename>
      friend class basic_client;

      class attribute_definition final
      {
          friend class basic_attributes;

        public:
          FOREST_NONCOPYABLE_CLASS(attribute_definition);
          FOREST_NONMOVABLE_CLASS(attribute_definition);
          FOREST_NONALLOCATABLE_CLASS(attribute_definition);

          attribute_definition(hyperdex_client_attribute const& attr) noexcept : attr_ { attr } {}
          attribute_definition(an_attribute const& attr) noexcept : attr_ { attr.a } {}

          template<typename AttrName>
          attribute_definition(AttrName&& name,
                               char const* value,
                               size_t sz,
                               hyperdatatype datatype = HYPERDATATYPE_STRING)
          noexcept
            : attr_ { nullptr, value, sz, datatype }
          {
            CStringTraits<std::decay_t<AttrName>>
              ::serialize(arenas_, status_, attr_.attr, std::forward<AttrName>(name));
          }

          template<typename AttrName, typename AttrData>
          attribute_definition(AttrName&& name, AttrData&& data) noexcept
            : attr_ { nullptr, nullptr, 0, ComplexTraits<std::decay_t<AttrData>>::type }
          {
            if (~ComplexTraits<std::decay_t<AttrData>>::serialize(arenas_,
                                                                           status_,
                                                                           attr_.value, attr_.value_sz,
                                                                           std::forward<AttrData>(data)))
            {
              CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                                        status_,
                                                                        attr_.attr,
                                                                        std::forward<AttrName>(name));
            }
          }

        private:

          mutable hyperdex_ds_returncode status_ {HYPERDEX_DS_SUCCESS};
          mutable arena_vector arenas_ {};
          hyperdex_client_attribute attr_;

      }; // class attribute_definition

      template<typename T>
      void init_from_container_(T const& t)
      {
        auto const sz = container_size(t);

        if (sz > 0)
        {
          auto arena = hyperdex_ds_arena_create();

          if (!arena)
          {
            log_ds_status(status_ = HYPERDEX_DS_NOMEM);
            return;
          }

          arenas_.emplace_back(arena, arena_deleter{});

          attrs_ = hyperdex_ds_allocate_attribute(arena, sz);

          if (!attrs_)
          {
            log_ds_status(status_ = HYPERDEX_DS_NOMEM);
            arenas_.pop_back();
            return;
          }

          size_ = sz;
          container_copy(t, attrs_,
                         hyperdex::begin(t),
                         hyperdex::end(t),
                         const_cast<hyperdex_client_attribute*>(attrs_),
                         sz);
        }

      } // init_from_container_

      void init_from_list_(std::initializer_list<attribute_definition> args) noexcept
      {
        if (args.size() > 0)
        {
          auto arena = hyperdex_ds_arena_create();

          if (!arena)
          {
            log_ds_status(status_ = HYPERDEX_DS_NOMEM);
            return;
          }

          arenas_.emplace_back(arena, arena_deleter{});

          attrs_ = hyperdex_ds_allocate_attribute(arena, args.size());

          if (!attrs_)
          {
            log_ds_status(status_ = HYPERDEX_DS_NOMEM);
            arenas_.pop_back();
            return;
          }

          size_ = args.size();

          size_t i = arenas_.size();
          for (auto& arg : args) i += arg.arenas_.size();
          arenas_.reserve(i);

          i = 0;
          for (auto& arg : args)
          {
            if (arg.status_ != HYPERDEX_DS_SUCCESS)
            {
              log_ds_status(status_ = arg.status_);
              return;
            }

            const_cast<hyperdex_client_attribute*>(attrs_)[i] = arg.attr_;

            arenas_.insert(arenas_.end(),
                           std::make_move_iterator(arg.arenas_.begin()),
                           std::make_move_iterator(arg.arenas_.end()));

            ++i;

          } // for (auto& arg : args)

        } // if (args.size() > 0)

      } // init_from_list_

    public:

      FOREST_NONCOPYABLE_CLASS(basic_attributes);
      FOREST_NONMOVABLE_CLASS(basic_attributes);
      FOREST_NONALLOCATABLE_CLASS(basic_attributes);

      basic_attributes() noexcept
      : attrs_ { nullptr }, size_ { 0 } {}

      basic_attributes(hyperdex_client_attribute const* attr, size_t size) noexcept
      : attrs_ { attr }, size_ { size } {}

      basic_attributes(an_attribute const* attr, size_t size) noexcept
      : attrs_ { &(attr->a) }, size_{ size } {}

      template<typename T>
      basic_attributes(T const& t,
                       std::enable_if_t<is_linear_safe_attribute_container<T>::value>* = nullptr) noexcept
      : attrs_ { attribute_filter(low_address(t)) }, size_ { container_size(t) }
      {}

      template<typename T>
      basic_attributes(T const& t,
                       std::enable_if_t<is_attribute_container<T>::value &&
                                       !is_linear_safe_attribute_container<T>::value>* = nullptr) noexcept
      {
        init_from_container_(t);
      }

      basic_attributes(std::initializer_list<attribute_definition> args) noexcept
      {
        init_from_list_(args);
      }

      template<typename T>
      std::enable_if_t<is_linear_safe_attribute_container<T>::value, basic_attributes&>
      operator = (T const& t) noexcept
      {
        clear();
        attrs_ = attribute_filter(low_address(t));
        size_  = container_size(t);
        return *this;
      }

      template<typename T>
      std::enable_if_t<is_attribute_container<T>::value && !is_linear_safe_attribute_container<T>::value,
      basic_attributes&>
      operator = (T const& t) noexcept
      {
        clear();
        init_from_container_(t);
        return *this;
      }

      ~basic_attributes() noexcept
      {
        if (destroy_f_) hyperdex_client_destroy_attrs(attrs_, size_);
      }

      bool set(std::initializer_list<attribute_definition> args)
      {
        clear();
        init_from_list_(args);
        return status_ == HYPERDEX_DS_SUCCESS;
      }

      explicit operator bool () const noexcept { return status_ == HYPERDEX_DS_SUCCESS; }

      hyperdex_ds_returncode status() const noexcept { return status_; }

      operator hyperdex_client_attribute const* () const noexcept { return attrs_; }

      operator an_attribute const* () const noexcept { return attrs_; }

      template<size_t N, typename = std::enable_if_t<(N > 0)>>
      operator attribute_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<attribute_array<N> const*>(attrs_);
      }

      template<size_t N, typename = std::enable_if_t<(N > 0)>>
      operator an_attribute_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<an_attribute_array<N> const*>(attrs_);
      }

      template<size_t N>
      operator attribute_c_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<attribute_c_array<N> const*>(attrs_);
      }

      template<size_t N>
      operator an_attribute_c_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<an_attribute_c_array<N> const*>(attrs_);
      }

      operator attribute_vector () const noexcept
      {
        return attribute_vector {attrs_, attrs_ + size_};
      }

      operator an_attribute_vector () const noexcept
      {
        return an_attribute_vector {attrs_, attrs_ + size_};
      }

      template<typename... TT, typename = std::enable_if_t<is_tuple_of_attributes<tuple<TT...>>::value>>
      operator tuple<TT...> const& () const noexcept
      {
        assert(tuple_size<tuple<TT...>>::value <= size_);
        return *reinterpret_cast<tuple<TT...> const*>(attrs_);
      }

      template<typename T, typename = std::enable_if_t<is_attribute_container<T>::value &&
                                                       std::is_default_constructible<T>::value &&
                                                      !is_array_of_attributes<T>::value &&
                                                      !is_c_array_of_attributes<T>::value &&
                                                      !is_vector_of_attributes<T>::value &&
                                                      !is_tuple_of_attributes<T>::value>>
      operator T () const
      {
        T t {};
        container_assign(attrs_, t,
                         attrs_, attrs_ + size_,
                         size_);
        return t;
      }

    private:

      struct stop_fill
      {
        template<typename I, typename... TT>
        static void fill(basic_attributes const*,
                         tuple<TT...>const&,
                         I&, size_t
                         ) noexcept
                         {}

        template<typename I, typename... TT>
        static void fill(basic_attributes const*,
                         tuple<TT...>const&,
                         I&, size_t,
                         attribute_status_vector const&
                         ) noexcept
                         {}
      };

      template<size_t i>
      struct fill_tuple
      {
        template<typename I, typename... TT>
        static void fill(basic_attributes const* obj,
                         tuple<TT...>const& t,
                         I& names, size_t sz)
                         noexcept
        {
          if (i < sz)
          {
            helper::assign_helper(get<i>(t), an_attribute { (*obj)[*names] });
            using Next = std::conditional_t<
              (i < tuple_size<tuple<TT...>>::value - 1),
              fill_tuple<i+1>,
              stop_fill
            >;
            Next::fill(obj, t, ++names, sz);
          }
        }

        template<typename I, typename... TT>
        static void fill(basic_attributes const* obj,
                         tuple<TT...>const& t,
                         I& names, size_t sz,
                         attribute_status_vector& statuses)
                         noexcept
        {
          if (i < sz)
          {
            an_attribute a { (*obj)[*names] };
            helper::assign_helper(get<i>(t), a);
            if (!*names || !*names[0]) statuses.push_back(attribute_status::bad_request);
            else if (!a.a.attr) statuses.push_back(attribute_status::not_found);
            else if (!a.a.value_sz) statuses.push_back(attribute_status::empty);
            else statuses.push_back(attribute_status::ok);
          }
          else
          {
            statuses.push_back(attribute_status::not_changed);
          }
          using Next = std::conditional_t<
            (i < tuple_size<tuple<TT...>>::value - 1),
            fill_tuple<i+1>,
            stop_fill
          >;
          Next::fill(obj, t, ++names, sz, statuses);
        }
      };

    public:

      template<typename... TT>
      void by_names(tuple<TT...> const& t, char const* const* names, size_t names_size) const noexcept
      {
        if (names && names_size)
        {
          using type = std::conditional_t<
              sizeof...(TT),
              fill_tuple<0>,
              stop_fill
          >;
          type::fill(this, t, names, std::min(names_size, tuple_size<tuple<TT...>>::value));
        }
      }

      template<size_t N, typename... TT>
      void by_names(tuple<TT...>const& t, name_c_array<N> const& names) const noexcept
      {
        by_names(t, names, N);
      }

      template<typename NameContainer, typename... TT>
      std::enable_if_t<is_name_container<NameContainer>::value && !is_c_array<NameContainer>::value>
      by_names(tuple<TT...>const& t, NameContainer const& names) const noexcept
      {
        auto const names_size = container_size(names);
        if (names_size)
        {
          using type = std::conditional_t<
              sizeof...(TT),
              fill_tuple<0>,
              stop_fill
          >;
          type::fill(this, t, begin(names), std::min(names_size, tuple_size<tuple<TT...>>::value));
        }
      }

      template<typename... TT>
      void by_names(tuple<TT...> const& t,
                    char const* const* names, size_t names_size,
                    attribute_status_vector& statuses) const noexcept
      {
        if (statuses.size()) statuses.clear();
        if (!names) names_size = 0;
        auto const sz = std::min(names_size, tuple_size<tuple<TT...>>::value);
        statuses.reserve(sz);
        using type = std::conditional_t<
            sizeof...(TT),
            fill_tuple<0>,
            stop_fill
        >;
        type::fill(this, t, names, sz, statuses);
      }

      template<size_t N, typename... TT>
      void by_names(tuple<TT...>const& t,
                    name_c_array<N> const& names,
                    attribute_status_vector& statuses) const noexcept
      {
        by_names(t, names, N, statuses);
      }

      template<typename NameContainer, typename... TT>
      std::enable_if_t<is_name_container<NameContainer>::value && !is_c_array<NameContainer>::value>
      by_names(tuple<TT...>const& t, NameContainer const& names,
               attribute_status_vector& statuses) const noexcept
      {
        auto const names_size = container_size(names);
        if (statuses.size()) statuses.clear();
        auto const sz = std::min(names_size, tuple_size<tuple<TT...>>::value);
        statuses.reserve(sz);
        using type = std::conditional_t<
            sizeof...(TT),
            fill_tuple<0>,
            stop_fill
        >;
        type::fill(this, t, begin(names), sz, statuses);
      }

      size_t linear_search(char const* name) const noexcept
      {
        auto res = size_;
        if (name)
        {
          register auto ptr = attrs_;
          register auto end = ptr + res;
          for (; ptr < end; ++ptr)
          {
            if (ptr->attr && !strcmp(ptr->attr, name)) { res -= end - ptr; break; }
          }
        }
        return res;
      }

      template<typename CString>
      size_t linear_search(CString const& name) const noexcept
      {
        arena_vector arenas {};
        hyperdex_ds_returncode status;

        char const* str;
        auto res = size_;

        if (!~CStringTraits<std::decay_t<CString>>
          ::serialize(arenas, status, str, name)) return res;

        if (str)
        {
          register auto ptr = attrs_;
          register auto end = ptr + res;
          for (; ptr < end; ++ptr)
          {
            if (ptr->attr && !strcmp(ptr->attr, str)) { res -= end - ptr; break; }
          }
        }
        return res;
      }

      using              size_type = size_t;
      using             value_type = an_attribute;
      using        difference_type = std::ptrdiff_t;
      using              reference = value_type&;
      using        const_reference = value_type const&;
      using                pointer = value_type*;
      using          const_pointer = value_type const*;
      using               iterator = pointer;
      using         const_iterator = const_pointer;
      using       reverse_iterator = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      // forward array's methods:

      const_reference at(size_type pos) const
      {
        _M_range_check(pos);
        return *reinterpret_cast<an_attribute const*>(&attrs_[pos]);
      }

      const_reference at(char const* str) const
      {
        if (!map_) map_attributes();
        return *reinterpret_cast<an_attribute const*>(&attrs_[map_->at(str)]);
      }

      template<typename AttrName>
      const_reference at(AttrName&& name) const
      {
        char const* str;
        if (!~CStringTraits<std::decay_t<AttrName>>
            ::serialize(arenas_, status_, str, std::forward<AttrName>(name)))
              throw std::out_of_range("basic_attributes::at on name serialization");
        return at(str);
      }

      const_reference operator[](size_type pos) const noexcept
      {
        return *reinterpret_cast<an_attribute const*>(&attrs_[pos]);
      }

      const_reference operator[](char const* str) const noexcept
      {
        if (!map_) map_attributes();
        auto it = map_->find(str);
        if (it == map_->end()) return bad_attribute_element;
        return *reinterpret_cast<an_attribute const*>(&attrs_[it->second]);
      }

      template<typename AttrName,
               typename = std::enable_if_t<!std::is_convertible<AttrName, size_type>::value &&
                                           !std::is_convertible<AttrName, char const*>::value>>
      const_reference operator[](AttrName&& name) const noexcept
      {
        arena_vector arenas {};
        hyperdex_ds_returncode status;

        char const* str;
        if (~CStringTraits<std::decay_t<AttrName>>
            ::serialize(arenas, status, str, std::forward<AttrName>(name))) return operator[](str);
        return bad_attribute_element;
      }

      const_reference front() const noexcept
      {
        return *reinterpret_cast<an_attribute const*>(&attrs_[0]);
      }

      const_reference back() const noexcept
      {
        return *reinterpret_cast<an_attribute const*>(&attrs_[size_ - 1]);
      }

      const_pointer data() const noexcept
      {
        return reinterpret_cast<an_attribute const*>(attrs_);
      }

      const_iterator cbegin() const noexcept { return begin(); }

      const_iterator begin() const noexcept
      {
        return reinterpret_cast<an_attribute const*>(attrs_);
      }

      const_iterator cend() const noexcept { return end(); }

      const_iterator end() const noexcept
      {
        return reinterpret_cast<an_attribute const*>(attrs_ + size_);
      }

      const_reverse_iterator crbegin() const noexcept { return rbegin(); }

      const_reverse_iterator rbegin() const noexcept
      {
        return std::reverse_iterator<const_iterator>(end());
      }

      const_reverse_iterator crend() const noexcept { return rend(); }

      const_reverse_iterator rend() const noexcept
      {
        return std::reverse_iterator<const_iterator>(begin());
      }

      bool         empty() const noexcept { return !size_; }
      size_type     size() const noexcept { return  size_; }
      size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); };

      void clear() noexcept
      {
        if (destroy_f_) hyperdex_client_destroy_attrs(attrs_, size_);
        map_ = forest::optional<std::unordered_map<forest::string, size_type>> {};
        arenas_ = arena_vector {};
        status_ = HYPERDEX_DS_SUCCESS;
        destroy_f_ = false;
        size_ = 0;
        attrs_ = nullptr;
      }

    private:

      void _M_range_check(size_type pos) const
      {
        if (pos < size_) throw std::out_of_range("hyperdex::basic_attributes::_M_range_check");
        return;
      }

      void map_attributes() const noexcept
      {
        map_ = std::unordered_map<forest::string, size_type> {};
        for (size_type i = 0; i < size_; ++i)
          if (attrs_[i].attr) map_->emplace(attrs_[i].attr, i);
      }

    private:

      hyperdex_client_attribute const* attrs_ { nullptr };
      size_type size_ { 0 };
      bool destroy_f_ { false };

      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };

      arena_vector arenas_ {};

      mutable forest::optional<std::unordered_map<forest::string, size_type>> map_ {};

  }; // class basic_attributes

  using attributes = basic_attributes<>;

} // namespace hyperdex

#endif // HYPERDEX_ATTRIBUTES_HPP_INCLUDED
