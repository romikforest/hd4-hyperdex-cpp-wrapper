#ifndef HYPERDEX_MAP_ATTRIBUTES_HPP_INCLUDED
#define HYPERDEX_MAP_ATTRIBUTES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: map_attributes.hpp
// First created: 2015-10-18-11.57
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
  class basic_map_attributes final
  {
      template<template<typename, typename = void> typename,
               template<typename, typename = void> typename>
      friend class basic_client;

      class map_attribute_definition final
      {
          friend class basic_map_attributes;

        public:
          FOREST_NONCOPYABLE_CLASS(map_attribute_definition);
          FOREST_NONMOVABLE_CLASS(map_attribute_definition);
          FOREST_NONALLOCATABLE_CLASS(map_attribute_definition);

          map_attribute_definition(hyperdex_client_map_attribute const& attr) noexcept : attr_ { attr } {}

          template<typename AttrName>
          map_attribute_definition(AttrName&& name,
                                         char const* map_key,
                                         size_t map_key_sz,
                                         hyperdatatype map_key_datatype,
                                         char const* value,
                                         size_t value_sz,
                                         hyperdatatype value_datatype)
          noexcept
            : attr_ { nullptr, map_key, map_key_sz, map_key_datatype, value, value_sz, value_datatype }
          {
            CStringTraits<std::decay_t<AttrName>>
              ::serialize(arenas_, status_, attr_.attr, std::forward<AttrName>(name));
          }

          template<typename AttrName, typename KeyData>
          map_attribute_definition(AttrName&& name,
                                         KeyData&& data,
                                         char const* value,
                                         size_t value_sz,
                                         hyperdatatype value_datatype)
                                         noexcept
          : attr_ { nullptr,
                    nullptr, 0, ComplexTraits<std::decay_t<KeyData>>::type,
                    value, value_sz, value_datatype }
          {
            if (~ComplexTraits<std::decay_t<KeyData>>::serialize(arenas_,
                                                                          status_,
                                                                          attr_.map_key, attr_.map_key_sz,
                                                                          std::forward<KeyData>(data)))
            {
              CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                                        status_,
                                                                        attr_.attr,
                                                                        std::forward<AttrName>(name));
            }
          }

          template<typename AttrName, typename ValueData>
          map_attribute_definition(AttrName&& name,
                                         char const* map_key,
                                         size_t map_key_sz,
                                         hyperdatatype map_key_datatype,
                                         ValueData&& data)
                                         noexcept
          : attr_ { nullptr, map_key, map_key_sz, map_key_datatype,
                             nullptr, 0, ComplexTraits<std::decay_t<ValueData>>::type }
          {
            if (~ComplexTraits<std::decay_t<ValueData>>::serialize(arenas_,
                                                                          status_,
                                                                          attr_.value, attr_.value_sz,
                                                                          std::forward<ValueData>(data)))
            {
              CStringTraits<std::decay_t<AttrName>>::serialize(arenas_,
                                                                        status_,
                                                                        attr_.attr,
                                                                        std::forward<AttrName>(name));
            }
          }

          template<typename AttrName, typename KeyData, typename ValueData>
          map_attribute_definition(AttrName&& name, KeyData&& k, ValueData&& v) noexcept
          : attr_ { nullptr,
                    nullptr, 0, ComplexTraits<std::decay_t<KeyData>>::type,
                    nullptr, 0, ComplexTraits<std::decay_t<ValueData>>::type }
          {
            if (~ComplexTraits<std::decay_t<KeyData>>::serialize(arenas_,
                                                                          status_,
                                                                          attr_.map_key, attr_.map_key_sz,
                                                                          std::forward<KeyData>(k))
                &&
                ~ComplexTraits<std::decay_t<ValueData>>::serialize(arenas_,
                                                                            status_,
                                                                            attr_.value, attr_.value_sz,
                                                                            std::forward<ValueData>(v))
                )
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
          hyperdex_client_map_attribute attr_;

      }; // class map_attribute_definition

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

          attrs_ = hyperdex_ds_allocate_map_attribute(arena, sz);

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
                         const_cast<hyperdex_client_map_attribute*>(attrs_),
                         sz);
        }

      } // init_from_container_

      void init_from_list_(std::initializer_list<map_attribute_definition> args) noexcept
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

          attrs_ = hyperdex_ds_allocate_map_attribute(arena, args.size());

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

            const_cast<hyperdex_client_map_attribute*>(attrs_)[i] = arg.attr_;

            arenas_.insert(arenas_.end(),
                           std::make_move_iterator(arg.arenas_.begin()),
                           std::make_move_iterator(arg.arenas_.end()));

            ++i;

          } // for (auto& arg : args)

        } // if (args.size() > 0)

      } // init_from_list_

    public:

      FOREST_NONCOPYABLE_CLASS(basic_map_attributes);
      FOREST_NONMOVABLE_CLASS(basic_map_attributes);
      FOREST_NONALLOCATABLE_CLASS(basic_map_attributes);

      basic_map_attributes() noexcept
      : attrs_ { nullptr }, size_ { 0 } {}

      basic_map_attributes(hyperdex_client_map_attribute const* attr, size_t size) noexcept
      : attrs_ { attr }, size_ { size } {}

      template<typename T>
      basic_map_attributes(T const& t,
                       std::enable_if_t<is_linear_safe_map_attribute_container<T>::value>* = nullptr) noexcept
      : attrs_ { low_address(t) }, size_ { container_size(t) }
      {}

      template<typename T>
      basic_map_attributes(T const& t,
                       std::enable_if_t<is_map_attribute_container<T>::value &&
                                       !is_linear_safe_map_attribute_container<T>::value>* = nullptr) noexcept
      {
        init_from_container_(t);
      }

      basic_map_attributes(std::initializer_list<map_attribute_definition> args) noexcept
      {
        init_from_list_(args);
      }

      template<typename T>
      std::enable_if_t<is_linear_safe_map_attribute_container<T>::value, basic_map_attributes&>
      operator = (T const& t) noexcept
      {
        clear();
        attrs_ = low_address(t);
        size_  = container_size(t);
        return *this;
      }

      template<typename T>
      std::enable_if_t<is_map_attribute_container<T>::value && !is_linear_safe_map_attribute_container<T>::value,
      basic_map_attributes&>
      operator = (T const& t) noexcept
      {
        clear();
        init_from_container_(t);
        return *this;
      }

      bool set(std::initializer_list<map_attribute_definition> args)
      {
        clear();
        init_from_list_(args);
        return status_ == HYPERDEX_DS_SUCCESS;
      }

      explicit operator bool () const noexcept { return status_ == HYPERDEX_DS_SUCCESS; }

      hyperdex_ds_returncode status() const noexcept { return status_; }

      operator hyperdex_client_map_attribute const* () const noexcept { return attrs_; }

      template<size_t N, typename = std::enable_if_t<(N > 0)>>
      operator map_attribute_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<map_attribute_array<N> const*>(attrs_);
      }

      template<size_t N>
      operator map_attribute_c_array<N> const& () const noexcept
      {
        assert(N == size_);

        return *reinterpret_cast<map_attribute_c_array<N> const*>(attrs_);
      }

      operator map_attribute_vector () const noexcept
      {
        return map_attribute_vector {attrs_, attrs_ + size_};
      }

      template<typename... TT, typename = std::enable_if_t<is_tuple_of_map_attributes<tuple<TT...>>::value>>
      operator tuple<TT...> const& () const noexcept
      {
        assert(tuple_size<tuple<TT...>>::value <= size_);
        return *reinterpret_cast<tuple<TT...> const*>(attrs_);
      }

      template<typename T, typename = std::enable_if_t<is_map_attribute_container<T>::value &&
                                                       std::is_default_constructible<T>::value &&
                                                      !is_array_of_map_attributes<T>::value &&
                                                      !is_c_array_of_map_attributes<T>::value &&
                                                      !is_vector_of_map_attributes<T>::value &&
                                                      !is_tuple_of_map_attributes<T>::value>>
      operator T () const
      {
        T t {};
        container_assign(attrs_, t,
                         attrs_, attrs_ + size_,
                         size_);
        return t;
      }

      using              size_type = size_t;
      using             value_type = hyperdex_client_map_attribute;
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
        if (pos < size_) throw std::out_of_range("hyperdex::basic_map_attributes::_M_range_check");
        return;
      }

    private:

      hyperdex_client_map_attribute const* attrs_ { nullptr };
      size_type size_ { 0 };

      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };

      arena_vector arenas_ {};

  }; // class basic_map_attributes

  using map_attributes = basic_map_attributes<>;

} // namespace hyperdex

#endif // HYPERDEX_MAP_ATTRIBUTES_HPP_INCLUDED
