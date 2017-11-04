#ifndef FOREST_ENDIAN_HPP_INCLUDED
#define FOREST_ENDIAN_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: endian.hpp
// First created: 2015-09-27-10.49
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifdef BOOST_ENDIAN_CONVERSION_HPP
#error <forest/endian.hpp> must be included before <boost/endian/conversion.hpp>
#endif // BOOST_ENDIAN_CONVERSION_HPP

// forest
#include <forest/alternatives.hpp>
#include <forest/int128.hpp>

// C++
#include <type_traits>

namespace boost
{
  namespace endian
  {
    inline  forest::int128_t endian_reverse( forest::int128_t x) noexcept;
    inline forest::uint128_t endian_reverse(forest::uint128_t x) noexcept;
  } // namespace endian
} // namespace boost

// boost
#include <boost/endian/conversion.hpp>

namespace boost
{
  namespace endian
  {

    #pragma GCC diagnostic push

    #pragma GCC diagnostic ignored "-Wstrict-aliasing"

    inline  forest::int128_t endian_reverse(forest::int128_t x) noexcept
    {
      uint64_t tmp = endian_reverse(*reinterpret_cast<uint64_t*>(&x));
      *reinterpret_cast<uint64_t*>(&x) = endian_reverse(*(reinterpret_cast<uint64_t*>(&x) + 1));
      *(reinterpret_cast<uint64_t*>(&x) + 1) = tmp;

      return x;
    }

    inline forest::uint128_t endian_reverse(forest::uint128_t x) noexcept
    {
      uint64_t tmp = endian_reverse(*reinterpret_cast<uint64_t*>(&x));
      *reinterpret_cast<uint64_t*>(&x) = endian_reverse(*(reinterpret_cast<uint64_t*>(&x) + 1));
      *(reinterpret_cast<uint64_t*>(&x) + 1) = tmp;

      return x;
    }

    #pragma GCC diagnostic pop

  } // namespace endian
} // namespace boost

namespace forest
{

  inline constexpr bool is_big_endian()
  {
    constexpr int const i = 1;
    return !(i << (sizeof(int) - sizeof(char)));
  }

  inline constexpr bool is_little_endian()
  {
    constexpr int const i = 1;
    return i << (sizeof(int) - sizeof(char));
  }

  inline void swap_bytes(void* buf, size_t size)
  {
    register unsigned char *start, *end;
    register unsigned char swap;

    for (start = reinterpret_cast<unsigned char*>(buf), end = start + size - 1; start < end; ++start, --end)
    {
      swap = *start;
      *start = *end;
      *end = swap;
    }
  }

  inline void swap_bytes_if_le(void* buf, size_t size)
  {
    if (is_little_endian()) swap_bytes(buf, size);
  }

  inline void swap_bytes_if_be(void* buf, size_t size)
  {
    if (is_big_endian()) swap_bytes(buf, size);
  }

  /*
  template<typename T>
  inline void swap_bytes_if_le_inplace(T& t)
  {
    if (is_little_endian()) swap_bytes(&t, sizeof(T));
  }

  template<typename T>
  inline void swap_bytes_if_be_inplace(T& t)
  {
    if (is_big_endian()) swap_bytes(&t, sizeof(T));
  }

  template<typename T>
  inline T swap_bytes_if_le(T const& t)
  {
    T res {t};
    if (is_little_endian()) swap_bytes(&res, sizeof(T));
    return res;
  }

  template<typename T>
  inline T swap_bytes_if_be(T const& t)
  {
    T res {t};
    if (is_big_endian()) swap_bytes(&res, sizeof(T));
    return res;
  }
  */

  using namespace boost::endian;

} // namespace forest

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::is_big_endian;
using forest::is_little_endian;
using forest::swap_bytes;
using forest::swap_bytes_if_le;
using forest::swap_bytes_if_be;

// using forest::swap_bytes_if_le_inplace;
// using forest::swap_bytes_if_be_inplace;

using namespace boost::endian;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // ENDIAN_HPP_INCLUDED
