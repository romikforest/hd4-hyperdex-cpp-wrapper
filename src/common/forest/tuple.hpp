#ifndef FOREST_TUPLE_HPP_INCLUDED
#define FOREST_TUPLE_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: tuple.hpp
// First created: 2015-09-12-00.08
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_TUPLE == FAV_STD

#include <tuple>
#include <functional>

namespace forest
{
  using std::tuple;
  using std::tuple_size;
  using std::tuple_element;
  using std::make_tuple;
  using std::tie;
  using std::forward_as_tuple;
  using std::get;
  using std::tuple_cat;
  using std::ignore;
  using std::ref;
  using std::cref;
}

#elif FOREST_ALTERNATIVE_FOR_TUPLE == FAV_BOOST

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace forest
{
  using boost::tuples::tuple;
  using boost::make_tuple;
  using boost::tie;
  using boost::tuples::get;
  using boost::tuples::ignore;
  using boost::ref;
  using boost::cref;
}

#ifndef FOREST_BOOST_TUPLE_HELPER_INCLUDED
#define FOREST_BOOST_TUPLE_HELPER_INCLUDED

namespace forest
{
  template <class T>
  struct tuple_size
     : public boost::integral_constant
     < std::size_t, boost::tuples::length<T>::value>
  {};

  template < int I, class T>
  struct tuple_element
  {
     typedef typename boost::tuples::element<I,T>::type type;
  };
}

#endif // FOREST_BOOST_TUPLE_HELPER_INCLUDED

#elif FOREST_ALTERNATIVE_FOR_TUPLE == FAV_STD_TR1

#include <tr1/tuple>
#include <tr1/functional>

namespace forest
{
  using std::tr1::tuple;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::make_tuple;
  using std::tr1::tie;
  using std::tr1::get;
  using std::tr1::ignore;
  using std::tr1::ref;
  using std::tr1::cref;
}

#elif FOREST_ALTERNATIVE_FOR_TUPLE == FAV_BOOST_TR1

#include <boost/tr1/tuple.hpp>
#include <boost/tr1/functional.hpp>

namespace forest
{
  using std::tr1::tuple;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::make_tuple;
  using std::tr1::tie;
  using std::tr1::get;
  using std::tr1::ignore;
  using std::tr1::ref;
  using std::tr1::cref;
}

#elif FOREST_ALTERNATIVE_FOR_TUPLE == FAV_MICROSOFT_TR1

#include <tuple>
#include <functional>

namespace forest
{
  using std::tr1::tuple;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::make_tuple;
  using std::tr1::tie;
  using std::tr1::get;
  using std::tr1::ignore;
  using std::tr1::ref;
  using std::tr1::cref;
}

#else

#error "Wrong alternative for tuple class!"

#endif // FOREST_ALTERNATIVE_FOR_TUPLE


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::tuple;
using forest::make_tuple;
using forest::tie;
using forest::tuple_size;
using forest::tuple_element;
using forest::get;
using forest::ignore;
using forest::ref;
using forest::cref;

#if FOREST_ALTERNATIVE_FOR_TUPLE != FAV_STD_TR1 && FOREST_ALTERNATIVE_FOR_TUPLE != FAV_BOOST_TR1 \
  && FOREST_ALTERNATIVE_FOR_TUPLE != FAV_MICROSOFT_TR1 && FOREST_ALTERNATIVE_FOR_TUPLE != FAV_BOOST
using forest::forward_as_tuple;
using forest::tuple_cat;
#endif // FOREST_ALTERNATIVE_FOR_TUPLE

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_TUPLE_HPP_INCLUDED
