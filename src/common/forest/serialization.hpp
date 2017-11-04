#ifndef FOREST_SERIALIZATION_HPP_INCLUDED
#define FOREST_SERIALIZATION_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: serialization.hpp
// First created: 2015-09-28-13.11
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_SERIALIZATION == FAV_BOOST

// need -lboost_serialization -lboost_system
// include headers for standard classes, if you want to serialize them,
// manually in your source files

#if FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE == FAV_DS_BIN

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

namespace forest
{
  using oarchive = boost::archive::binary_oarchive;
  using iarchive = boost::archive::binary_iarchive;
}

#elif FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE == FAV_DS_TEXT

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace forest
{
  using oarchive = boost::archive::text_oarchive;
  using iarchive = boost::archive::text_iarchive;
}

#elif FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE == FAV_DS_TEXT_WIDE

#include <boost/archive/text_woarchive.hpp>
#include <boost/archive/text_wiarchive.hpp>

namespace forest
{
  using oarchive = boost::archive::text_woarchive;
  using iarchive = boost::archive::text_wiarchive;
}

#elif FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE == FAV_DS_XML

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

namespace forest
{
  using oarchive = boost::archive::xml_oarchive;
  using iarchive = boost::archive::xml_iarchive;
}

#elif FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE == FAV_DS_XML_WIDE

#include <boost/archive/xml_woarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>

namespace forest
{
  using oarchive = boost::archive::xml_woarchive;
  using iarchive = boost::archive::xml_wiarchive;
}

#else

#error "Wrong alternative for serialization archive type!"

#endif // FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE

#else

#error "Wrong alternative for serialization functionality!"

#endif // FOREST_ALTERNATIVE_FOR_SERIALIZATION

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL
#ifdef FOREST_USE_SERIALIZATION

using oarchive = forest::oarchive;
using iarchive = forest::iarchive;

#endif // FOREST_USE_SERIALIZATION
#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // FOREST_SERIALIZATION_HPP_INCLUDED
