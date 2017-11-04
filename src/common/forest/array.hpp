#ifndef FOREST_ARRAY_HPP_INCLUDED
#define FOREST_ARRAY_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: array.hpp
// First created: 2015-09-12-00.07
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>
#include <type_traits>

#if FOREST_ALTERNATIVE_FOR_ARRAY == FAV_STD

#include <array>
#include <functional>

namespace forest
{
  using std::array;
  using std::tuple_size;
  using std::tuple_element;
  using std::get;
  using std::swap;
  using std::ref;
  using std::cref;
}

#elif FOREST_ALTERNATIVE_FOR_ARRAY == FAV_BOOST

#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace forest
{
  using boost::array;
  using boost::tuples::get;
  using boost::tuples::swap;
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


#elif FOREST_ALTERNATIVE_FOR_ARRAY == FAV_STD_TR1

#include <tr1/array>
#include <tr1/functional>

namespace forest
{
  using std::tr1::array;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::get;
  using std::tr1::swap;
  using std::tr1::ref;
  using std::tr1::cref;
}

#elif FOREST_ALTERNATIVE_FOR_ARRAY == FAV_BOOST_TR1

#include <boost/tr1/array.hpp>
#include <boost/tr1/functional.hpp>

namespace forest
{
  using std::tr1::array;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::get;
  using std::tr1::swap;
  using std::tr1::ref;
  using std::tr1::cref;
}

#elif FOREST_ALTERNATIVE_FOR_ARRAY == FAV_MICROSOFT_TR1

#include <array>
#include <functional>

namespace forest
{
  using std::tr1::array;
  using std::tr1::tuple_size;
  using std::tr1::tuple_element;
  using std::tr1::get;
  using std::tr1::swap;
  using std::tr1::ref;
  using std::tr1::cref;
}

#else

#error "Wrong alternative for array class!"

#endif // FOREST_ALTERNATIVE_FOR_ARRAY

namespace forest
{
  template<typename ArrayType>
  class is_forest_array
  {
      struct helper
      {
        template<typename T, size_t size>
        helper(array<T, size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, ArrayType>::value;
  };

}


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::array;
using forest::tuple_size;
using forest::tuple_element;
using forest::get;
using forest::swap;
using forest::ref;
using forest::cref;

using forest::is_forest_array;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_ARRAY_HPP_INCLUDED
