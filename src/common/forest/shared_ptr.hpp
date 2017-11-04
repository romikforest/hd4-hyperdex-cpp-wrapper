#ifndef FOREST_SHARED_PTR_HPP_INCLUDED
#define FOREST_SHARED_PTR_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: shared_ptr.hpp
// First created: 2015-09-11-20.41
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_SHARED_PTR == FAV_STD

#include <memory>
#include <utility>

namespace forest
{

  using std::shared_ptr;
  using std::make_shared;
  using std::allocate_shared;

}

#elif FOREST_ALTERNATIVE_FOR_SHARED_PTR == FAV_BOOST

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace forest
{

  using boost::shared_ptr;
  using boost::make_shared;
  using boost::allocate_shared;

}

#elif FOREST_ALTERNATIVE_FOR_SHARED_PTR == FAV_STD_TR1

#include <tr1/memory>

namespace forest
{

  using std::tr1::shared_ptr;

  /*
  template <typename T, typename... Args>
  inline shared_ptr<T> make_shared(Args&&... args)
  {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template< class T, class Alloc, class... Args >
  inline shared_ptr<T> allocate_shared( Alloc const & a, Args && ... args )
  {
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
  }
  */

}

#elif FOREST_ALTERNATIVE_FOR_SHARED_PTR == FAV_BOOST_TR1

#include <boost/tr1/memory.hpp>

namespace forest
{

  using std::tr1::shared_ptr;

}

#elif FOREST_ALTERNATIVE_FOR_SHARED_PTR == FAV_MICROSOFT_TR1

#include <memory>

namespace forest
{

  using std::tr1::shared_ptr;

}

#else

#error "Wrong alternative for shared_ptr class!"

#endif // FOREST_ALTERNATIVE_FOR_SHARED_PTR


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::shared_ptr;

#if FOREST_ALTERNATIVE_FOR_SHARED_PTR != FAV_STD_TR1 && \
    FOREST_ALTERNATIVE_FOR_SHARED_PTR != FAV_BOOST_TR1 && \
    FOREST_ALTERNATIVE_FOR_SHARED_PTR != FAV_MICROSOFT_TR1

using forest::make_shared;
using forest::allocate_shared;

#endif // FOREST_ALTERNATIVE_FOR_SHARED_PTR

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_SHARED_PTR_HPP_INCLUDED
