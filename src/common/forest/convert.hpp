#ifndef FOREST_CONVERT_HPP_INCLUDED
#define FOREST_CONVERT_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: convert.hpp
// First created: 2015-09-11-19.43
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifndef CONVERT_FRAMEWORK_GUARD
#define CONVERT_FRAMEWORK_GUARD

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_CONVERT == FAV_BOOST

#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>

namespace forest
{

  using boost::convert;

} // namespace forest

struct boost::cnv::by_default : public boost::cnv::lexical_cast {};

#else

#error "Wrong alternative for convert framework!"

#endif // FOREST_ALTERNATIVE_FOR_CONVERT


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::convert;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // CONVERT_FRAMEWORK_GUARD

#endif // FOREST_CONVERT_HPP_INCLUDED
