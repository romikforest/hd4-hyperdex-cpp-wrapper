#ifndef DATE_HPP_INCLUDED
#define DATE_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: date.hpp
// First created: 2015-09-28-09.45
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

// uses date.h from http://howardhinnant.github.io/date_v2.html
// modified by Koptev R.V. 2015
#include <forest/date.h>

namespace std
{
  namespace chrono
  {
    using date::operator<<;

  } // namespace chrono

} // namespace std


#endif // DATE_HPP_INCLUDED
