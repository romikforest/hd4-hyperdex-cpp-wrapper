#ifndef FOREST_LOCALE_HPP_INCLUDED
#define FOREST_LOCALE_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: locale.hpp
// First created: 2015-09-11-19.41
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_LOCALE == FAV_NONE
#elif FOREST_ALTERNATIVE_FOR_LOCALE == FAV_BOOST

#include <boost/locale.hpp>

namespace forest
{
  namespace locale = ::boost::locale;
}

#else

#error "Wrong alternative for locale framework!"

#endif // FOREST_ALTERNATIVE_FOR_LOCALE

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL
#ifdef FOREST_USE_LOCALE

namespace locale = forest::locale;

#endif // FOREST_USE_LOCALE
#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // FOREST_LOCALE_HPP_INCLUDED
