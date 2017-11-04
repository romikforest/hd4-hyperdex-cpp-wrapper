#ifndef FOREST_TYPE_NAMES_HPP_INCLUDED
#define FOREST_TYPE_NAMES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: type_names.hpp
// First created: 2015-10-22-08.36
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <boost/type_index.hpp>
#include <iostream>
#include <forest/string.hpp>

namespace forest
{

  template<typename T>
  inline void print_type_name(T&&)
  {
    std::cout << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
  }

  template<typename T>
  inline forest::string type_name(T&&)
  {
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
  }

  template<typename T>
  inline void print_type_name()
  {
    std::cout << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
  }

  template<typename T>
  inline forest::string type_name()
  {
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
  }

} // namespace forest

#endif // FOREST_TYPE_NAMES_HPP_INCLUDED
