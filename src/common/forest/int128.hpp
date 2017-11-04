#ifndef INT128_HPP_INCLUDED
#define INT128_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: int128.hpp
// First created: 2015-09-27-10.57
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

// forest
#include <forest/alternatives.hpp>
#include <forest/string.hpp>

// C++
#include <cstddef>
#include <stdint.h>
#include <iostream>
#include <limits>

#ifdef __STRICT_ANSI__
#warning __STRICT_ANSI__ is defined. Numeric limits for 128 bit integrals will not defined.
#endif

#ifndef _GLIBCXX_USE_INT128
#warning _GLIBCXX_USE_INT128 is not defined. You can not use 128 bit integrals.
#endif


namespace forest
{
  using  int128_t =  __int128_t;
  using uint128_t = __uint128_t;

} // namespace forest

namespace std
{

inline std::ostream& operator << (std::ostream& lhs, __int128_t rhs)
{
  string out;

  char const nums[] = "0123456789ABCDEF";

  bool sign;
  __uint128_t tmp;

  if (rhs < 0 && (lhs.flags() & std::ios::dec))
  {
    sign = true;
    tmp = - rhs;
  }
  else
  {
    sign = false;
    tmp = rhs;
  }

  if (lhs.flags() & std::ios::hex)
  {
    do
    {
      out = nums[tmp % 16] + out;
      tmp /= 16;
    }
    while (tmp != 0);

    if (lhs.flags() & std::ios::showbase)
      out = "0x" + out;
  }
  else if (lhs.flags() & std::ios::oct)
  {
    do
    {
      out = nums[tmp % 8] + out;
      tmp /= 8;
    }
    while (tmp != 0);

    if (lhs.flags() & std::ios::showbase)
      out = '0' + out;
  }
  else
  {
    do
    {
      out = nums[tmp % 10] + out;
      tmp /= 10;
    }
    while (tmp != 0);
  }

  if (sign)
    out = "-" + out;

  return lhs << out;
}

inline std::ostream& operator << (std::ostream& lhs, __uint128_t rhs)
{
  string out;

  char const nums[] = "0123456789ABCDEF";

  if (lhs.flags() & std::ios::hex)
  {
    do
    {
      out = nums[rhs % 16] + out;
      rhs /= 16;
    }
    while (rhs != 0);

    if (lhs.flags() & std::ios::showbase)
      out = "0x" + out;
  }
  else if (lhs.flags() & std::ios::oct)
  {
    do
    {
      out = nums[rhs % 8] + out;
      rhs /= 8;
    }
    while (rhs != 0);

    if (lhs.flags() & std::ios::showbase)
      out = '0' + out;
  }
  else
  {
    do
    {
      out = nums[rhs % 10] + out;
      rhs /= 10;
    }
    while (rhs != 0);
  }

  return lhs << out;
}

}

namespace std
{

  #ifdef __STRICT_ANSI__
  forest::int128_t abs(forest::int128_t arg)
  {
    return arg < 0 ? -arg : arg;
  }
  #endif
}

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::int128_t;
using forest::uint128_t;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // INT128_HPP_INCLUDED
