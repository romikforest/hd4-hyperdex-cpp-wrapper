#ifndef FOREST_EXT_OPTIONAL_HPP_INCLUDED
#define FOREST_EXT_OPTIONAL_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: ext_optional.hpp
// First created: 2015-09-10-16.45
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_EXT_OPTIONAL == FAV_FOREST

#include <forest/optional.hpp>

// C++
#include <utility>

namespace forest
{
  template<typename status_container, typename T = nullopt_t>
  class ext_optional
  {
  public:
    ext_optional(const status_container& s): sc(s), opt() {}
    ext_optional(status_container&& s): sc(std::move(s)), opt() {}

    ext_optional(const ext_optional& other)
    {
      sc  = other.sc;
      opt = other.opt;
    }

    ext_optional(ext_optional&& other)
    {
      sc  = std::move(other.sc);
      opt = std::move(other.opt);
    }

    template<typename ... U> ext_optional(const status_container& s, U&& ... args)
      : sc(s), opt(std::forward(args)...)
    {}

    template<typename ... U> ext_optional(status_container&& s, U&& ... args)
      : sc(std::move(s)), opt(std::forward(args)...)
    {}

    ~ext_optional()
    {
      sc.destroy(&opt);
    }

    ext_optional& operator=( const ext_optional& other )
    {
      if (this != &other)
      {
        sc  = other.sc;
        opt = other.opt;
      }
      return *this;
    }

    ext_optional& operator=( ext_optional&& other )
    {
      if (this != &other)
      {
        sc  = std::move(other.sc);
        opt = std::move(other.opt);
      }
      return *this;
    }

    constexpr const T* operator->() const { return opt.operator->(); }

    constexpr T* operator->() { return opt.operator->(); }

    constexpr const T& operator*() const& { return opt.operator*(); }

    constexpr T& operator*() & { return opt->operator*(); }

    constexpr const T&& operator*() const&& { return std::move(&opt)->operator*(); }

    constexpr T&& operator*() && { return std::move(&opt)->operator*(); }

    constexpr explicit operator bool() const { return sc.is_ok() && opt; }

    constexpr T& value() & { return opt.value(); }

    constexpr const T & value() const & { return opt.value(); }

    constexpr T&& value() && { return std::move(&opt)->value; }

    constexpr const T&& value() const && { return std::move(&opt)->value; }

    template< class U > constexpr T value_or( U&& default_value ) const&
    {
      return opt.value_or(std::forward(default_value));
    }

    template< class U > T value_or( U&& default_value ) &&
    {
      return std::move(&opt)->value_or(std::forward(default_value));
    }

    void swap( ext_optional& other )
    {
      swap(sc, other.sc);
      opt.swap(other);
    }

    template< class... Args > void emplace( const status_container& c, Args&&... args )
    {
      sc = c;
      opt.emplace(std::forward(args)...);
    }

    template< class... Args > void emplace( status_container&& c, Args&&... args )
    {
      sc = std::move(c);
      opt.emplace(std::forward(args)...);
    }

  private:
    status_container sc;
    optional<T> opt;

  }; // class ext_optional

  template <class T>
  void swap(ext_optional<T>& x, ext_optional<T>& y) noexcept(noexcept(x.swap(y)))
  {
    x.swap(y);
  }

} // namespace forest

namespace std
{
  template <typename T>
  struct hash<forest::ext_optional<T>>
  {
    using result_type   = typename hash<T>::result_type;
    using argument_type = forest::ext_optional<T>;

    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };

  template <typename T>
  struct hash<forest::ext_optional<T&>>
  {
    using result_type   = typename hash<T>::result_type;
    using argument_type = forest::ext_optional<T&>;

    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
}

#else

#error "Wrong alternative for ext_optional class!"

#endif // FOREST_ALTERNATIVE_FOR_EXT_OPTIONAL


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::ext_optional;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_EXT_OPTIONAL_HPP_INCLUDED
