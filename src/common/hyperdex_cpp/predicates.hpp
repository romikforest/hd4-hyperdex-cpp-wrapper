#ifndef HYPERDEX_PREDICATES_HPP_INCLUDED
#define HYPERDEX_PREDICATES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: predicates.hpp
// First created: 2015-10-18-11.55
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

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/iterators_and_traits.hpp>

// C++
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include <tuple>

namespace hyperdex
{
  template<typename, typename> class c_string_traits;
  template<typename, typename> class complex_data_traits;

  template<template<typename, typename = void> typename CStringTraits = c_string_traits,
           template<typename, typename = void> typename ComplexTraits = complex_data_traits>
  class basic_predicates final
  {
      template<template<typename, typename = void> typename,
               template<typename, typename = void> typename>
      friend class basic_client;

      class predicate_definition final
      {
          friend class basic_predicates;

        public:
          FOREST_NONCOPYABLE_CLASS(predicate_definition);
          FOREST_NONMOVABLE_CLASS(predicate_definition);
          FOREST_NONALLOCATABLE_CLASS(predicate_definition);

          predicate_definition(hyperdex_client_attribute_check const& attr) noexcept : attr_ { attr } {}

          template<typename AttrName>
          predicate_definition(AttrName&& name,
                               char const* value,
                               size_t sz,
                               hyperdatatype datatype,
                               hyperpredicate pred)
          noexcept
            : attr_ { nullptr, value, sz, datatype, pred }
          {
            CStringTraits<std::decay_t<AttrName>>
              ::serialize(arenas_, status_, attr_.attr, std::forward<AttrName>(name));
          }

          template<typename AttrName, typename AttrData>
          predicate_definition(AttrName&& name, AttrData&& data, hyperpredicate pred) noexcept
            : attr_ { nullptr, nullptr, 0, ComplexTraits<std::decay_t<AttrData>>::type, pred }
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
          hyperdex_client_attribute_check attr_;

      }; // class predicate_definition

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

          attrs_ = hyperdex_ds_allocate_attribute_check(arena, sz);

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
                         const_cast<hyperdex_client_attribute_check*>(attrs_),
                         sz);
        }

      } // init_from_container_

      void init_from_list_(std::initializer_list<predicate_definition> args) noexcept
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

          attrs_ = hyperdex_ds_allocate_attribute_check(arena, args.size());

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

            const_cast<hyperdex_client_attribute_check*>(attrs_)[i] = arg.attr_;

            arenas_.insert(arenas_.end(),
                           std::make_move_iterator(arg.arenas_.begin()),
                           std::make_move_iterator(arg.arenas_.end()));

            ++i;

          } // for (auto& arg : args)

        } // if (args.size() > 0)

      } // init_from_list_

    public:

      FOREST_NONCOPYABLE_CLASS(basic_predicates);
      FOREST_NONMOVABLE_CLASS(basic_predicates);
      FOREST_NONALLOCATABLE_CLASS(basic_predicates);

      basic_predicates() noexcept
      : attrs_ { nullptr }, size_ { 0 } {}

      basic_predicates(hyperdex_client_attribute_check const* attr, size_t size) noexcept
      : attrs_ { attr }, size_ { size } {}

      template<typename T>
      basic_predicates(T const& t,
                       std::enable_if_t<is_linear_safe_predicate_container<T>::value>* = nullptr) noexcept
      : attrs_ { low_address(t) }, size_ { container_size(t) }
      {}

      template<typename T>
      basic_predicates(T const& t,
                       std::enable_if_t<is_predicate_container<T>::value &&
                                       !is_linear_safe_predicate_container<T>::value>* = nullptr) noexcept
      {
        init_from_container_(t);
      }

      basic_predicates(std::initializer_list<predicate_definition> args) noexcept
      {
        init_from_list_(args);
      }

      template<typename T>
      std::enable_if_t<is_linear_safe_predicate_container<T>::value, basic_predicates&>
      operator = (T const& t) noexcept
      {
        clear();
        attrs_ = low_address(t);
        size_  = container_size(t);
        return *this;
      }

      template<typename T>
      std::enable_if_t<is_predicate_container<T>::value && !is_linear_safe_predicate_container<T>::value,
      basic_predicates&>
      operator = (T const& t) noexcept
      {
        clear();
        init_from_container_(t);
        return *this;
      }

      bool set(std::initializer_list<predicate_definition> args)
      {
        clear();
        init_from_list_(args);
        return status_ == HYPERDEX_DS_SUCCESS;
      }

      explicit operator bool () const noexcept { return status_ == HYPERDEX_DS_SUCCESS; }

      hyperdex_ds_returncode status() const noexcept { return status_; }

      operator hyperdex_client_attribute_check const* () const noexcept { return attrs_; }

      template<size_t N, typename = std::enable_if_t<(N > 0)>>
      operator predicate_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<predicate_array<N> const*>(attrs_);
      }

      template<size_t N>
      operator predicate_c_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<predicate_c_array<N> const*>(attrs_);
      }

      operator predicate_vector () const noexcept
      {
        return predicate_vector {attrs_, attrs_ + size_};
      }

      template<typename... TT, typename = std::enable_if_t<is_tuple_of_predicates<tuple<TT...>>::value>>
      operator tuple<TT...> const& () const noexcept
      {
        assert(tuple_size<tuple<TT...>>::value <= size_);
        return *reinterpret_cast<tuple<TT...> const*>(attrs_);
      }

      template<typename T, typename = std::enable_if_t<is_predicate_container<T>::value &&
                                                       std::is_default_constructible<T>::value &&
                                                      !is_array_of_predicates<T>::value &&
                                                      !is_c_array_of_predicates<T>::value &&
                                                      !is_vector_of_predicates<T>::value &&
                                                      !is_tuple_of_predicates<T>::value>>
      operator T () const
      {
        T t {};
        container_assign(attrs_, t,
                         attrs_, attrs_ + size_,
                         size_);
        return t;
      }

      using              size_type = size_t;
      using             value_type = hyperdex_client_attribute_check;
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
        return attrs_[pos];
      }

      const_reference operator[](size_type pos) const noexcept
      {
        return attrs_[pos];
      }

      const_reference front() const noexcept
      {
        return attrs_[0];
      }

      const_reference back() const noexcept
      {
        return attrs_[size_ - 1];
      }

      const_pointer data() const noexcept
      {
        return attrs_;
      }

      const_iterator cbegin() const noexcept { return begin(); }

      const_iterator begin() const noexcept
      {
        return attrs_;
      }

      const_iterator cend() const noexcept { return end(); }

      const_iterator end() const noexcept
      {
        return attrs_ + size_;
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
        arenas_ = arena_vector {};
        status_ = HYPERDEX_DS_SUCCESS;
        size_ = 0;
        attrs_ = nullptr;
      }

    private:

      void _M_range_check(size_type pos) const
      {
        if (pos < size_) throw std::out_of_range("hyperdex::basic_predicates::_M_range_check");
        return;
      }

    private:

      hyperdex_client_attribute_check const* attrs_ { nullptr };
      size_type size_ { 0 };

      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };

      arena_vector arenas_ {};

  }; // class basic_predicates

  using predicates = basic_predicates<>;

} // namespace hyperdex

#endif // HYPERDEX_PREDICATES_HPP_INCLUDED
