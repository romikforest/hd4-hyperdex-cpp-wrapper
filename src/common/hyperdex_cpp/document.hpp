#ifndef HYPERDEX_DOCUMENT_HPP_INCLUDED
#define HYPERDEX_DOCUMENT_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: document.hpp
// First created: 2015-10-18-07.24
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

// forest
#include <forest/string.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>

namespace hyperdex
{
  using forest::string;

  template <typename, typename> class c_string_traits;

  template<template<typename, typename = void> typename CStringTraits = c_string_traits>
  class basic_document
  {
    public:

      static constexpr bool const is_document = true;

      basic_document() noexcept: str_ { "{}" } {}

      basic_document(char const* str, size_t size)
      : str_ { str, size } {}

      basic_document(char const* str)
      : str_ { str } {}

      basic_document(string const& str)
      : str_ { str } {}

      basic_document(string&& str)
      : str_ { std::move(str) } {}


      template<typename CString>
      basic_document(CString&& c_string)
      {
        arena_vector arenas {};
        char const* str;

        if (~CStringTraits<std::decay_t<CString>>
            ::serialize(arenas, status_, str, std::forward<CString>(c_string)))
            str_ = str;

      } // basic_document(CString&& c_string)

      string  to_string() const noexcept { return str_; }
      char const* c_str() const noexcept { return str_.c_str(); }

      explicit operator bool ()    { return status_ == HYPERDEX_DS_SUCCESS; }

      size_t size() const noexcept { return str_.size();  }
      bool  empty() const noexcept { return str_.empty(); }

    private:
      hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };
      string str_;

  }; // class basic_document

  using document = basic_document<>;

} // namespace hyperdex

#endif // HYPERDEX_DOCUMENT_HPP_INCLUDED
