#ifndef FOREST_ALTERNATIVES_HPP_INCLUDED
#define FOREST_ALTERNATIVES_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: alternatives.hpp
// First created: 2015-09-11-20.29
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives_values.hpp>

///////////////////////////////
// Visibility control:
///////////////////////////////
// comment to use only in namespace forest
// uncomment to use both - global and forest
//#define FOREST_ALTERNATIVES_DECLARE_GLOBAL

///////////////////////////////
// Memory management control
///////////////////////////////
// available:
// FAV_NONE
// FAV_STD
// FAV_LTALLOC
//
// If you use FAV_LTALLOC you must include file forest.cpp to your project
// and do not include ltalloc.cc manually.
// Only ltalloc allocator included as an option.
// Read documentation for ltalloc for project options or use files
// forest_compile_options.sh and forest_link_options.sh
// llallloc (lockless allocator) also included,
// but you must tune your project manually
// to work with it. Look README file for llalloc
#define FOREST_MEMORY_MANAGER FAV_NONE

///////////////////////////////
// move and forward alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_MOVE FAV_STD

///////////////////////////////
// locale framework alternatives
///////////////////////////////
// available:
// FAV_NONE
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_LOCALE FAV_NONE

// use wide strings and wchar_t characters
#define FOREST_USE_WSTRINGS

// defined if locale framework used
#if FOREST_ALTERNATIVE_FOR_LOCALE != FAV_NONE
#define FOREST_USE_LOCALE
#endif // FOREST_ALTERNATIVE_FOR_LOCALE

///////////////////////////////
// LOG alternatives
///////////////////////////////
// available:
// FAV_NONE
// FAV_LOG_BOOST_TRIVIAL
#define FOREST_ALTERNATIVE_FOR_LOG FAV_LOG_BOOST_TRIVIAL

// defined if log used
#if FOREST_ALTERNATIVE_FOR_LOG != FAV_NONE
#define FOREST_USE_LOG
#endif // FOREST_ALTERNATIVE_FOR_LOG

///////////////////////////////
// Serialization alternatives
///////////////////////////////
// available:
// FAV_NONE
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_SERIALIZATION FAV_BOOST

// Archive type
// available:
// FAV_DS_BIN
// FAV_DS_TEXT
// FAV_DS_TEXT_WIDE
// FAV_DS_XML
// FAV_DS_XML_WIDE
#define FOREST_ALTERNATIVE_FOR_SERIALIZATION_ARCHIVE FAV_DS_TEXT

// defined if serialization used
#if FOREST_ALTERNATIVE_FOR_SERIALIZATION != FAV_NONE
#define FOREST_USE_SERIALIZATION
#endif // FOREST_ALTERNATIVE_FOR_SERIALIZATION

///////////////////////////////
// convert alternatives
///////////////////////////////
// available:
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_CONVERT FAV_BOOST

///////////////////////////////
// unique_ptr alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
// FAV_INTERPROCESS
#define FOREST_ALTERNATIVE_FOR_UNIQUE_PTR FAV_STD

///////////////////////////////
// shared_ptr alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
// FAV_STD_TR1
// FAV_BOOST_TR1
// FAV_MICROSOFT_TR1
#define FOREST_ALTERNATIVE_FOR_SHARED_PTR FAV_STD

///////////////////////////////
// unordered_map alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
// FAV_STD_TR1
// FAV_BOOST_TR1
// FAV_MICROSOFT_TR1
// FAV_SPARSE_HASH
// FAV_DENSE_HASH
#define FOREST_ALTERNATIVE_FOR_UNORDERED_MAP FAV_SPARSE_HASH

///////////////////////////////
// unordered_set alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
// STD_TR1
// BOOST_TR1
// MICROSOFT_TR1
// SPARSE_HASH
// DENSE_HASH
#define FOREST_ALTERNATIVE_FOR_UNORDERED_SET FAV_SPARSE_HASH

///////////////////////////////
// map alternatives
///////////////////////////////
// available
// FAV_STD
// FAV_BTREE
// FAV_BOOST_FLAT
#define FOREST_ALTERNATIVE_FOR_MAP FAV_BTREE

///////////////////////////////
// set alternatives
///////////////////////////////
// available
// FAV_STD
// FAV_BTREE
// FAV_BOOST_FLAT
#define FOREST_ALTERNATIVE_FOR_SET FAV_BTREE

///////////////////////////////
// multi_map alternatives
///////////////////////////////
// available
// FAV_STD
// FAV_BTREE
// FAV_BOOST_FLAT
#define FOREST_ALTERNATIVE_FOR_MULTIMAP FAV_BTREE

///////////////////////////////
// multi_set alternatives
///////////////////////////////
// available
// FAV_STD
// FAV_BTREE
// FAV_BOOST_FLAT
#define FOREST_ALTERNATIVE_FOR_MULTISET FAV_BTREE

///////////////////////////////
// vector alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_VECTOR FAV_STD

///////////////////////////////
// string alternatives
///////////////////////////////
// available:
// FAV_STD
#define FOREST_ALTERNATIVE_FOR_STRING FAV_STD

// use string extensions:
// FAV_NONE
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_STRING_EXT FAV_BOOST

///////////////////////////////
// array alternatives
///////////////////////////////
// available:
// FAV_STD
// FAV_BOOST
// FAV_STD_TR1
// FAV_BOOST_TR1
// FAV_MICRISOFT_TR1
#define FOREST_ALTERNATIVE_FOR_ARRAY FAV_STD

///////////////////////////////
// tuple alternatives
///////////////////////////////
// FAV_STD
// FAV_BOOST
// FAV_STD_TR1
// FAV_BOOST_TR1
// FAV_MICRISOFT_TR1
#define FOREST_ALTERNATIVE_FOR_TUPLE FAV_STD

///////////////////////////////
// optional alternatives
///////////////////////////////
// available:
// FAV_EXPERIMENTAL
// FAV_BOOST
#define FOREST_ALTERNATIVE_FOR_OPTIONAL FAV_EXPERIMENTAL

///////////////////////////////
// ext_optional alternatives
///////////////////////////////
// available:
// FAV_FOREST
#define FOREST_ALTERNATIVE_FOR_EXT_OPTIONAL FAV_FOREST

///////////////////////////////
// expected alternatives
///////////////////////////////
// FAV_PROTO
#define FOREST_ALTERNATIVE_FOR_EXPECTED FAV_PROTO

///////////////////////////////
// Checks

#if (FOREST_ALTERNATIVE_FOR_ARRAY == FAV_STD         || \
     FOREST_ALTERNATIVE_FOR_ARRAY == FAV_BOOST       || \
     FOREST_ALTERNATIVE_FOR_ARRAY == FAV_STD_TR1     || \
     FOREST_ALTERNATIVE_FOR_ARRAY == FAV_BOOST_TR1   || \
     FOREST_ALTERNATIVE_FOR_ARRAY == FAV_MICROSOFT_TR1) \
       && \
    (FOREST_ALTERNATIVE_FOR_TUPLE == FAV_STD         || \
     FOREST_ALTERNATIVE_FOR_TUPLE == FAV_BOOST       || \
     FOREST_ALTERNATIVE_FOR_TUPLE == FAV_STD_TR1     || \
     FOREST_ALTERNATIVE_FOR_TUPLE == FAV_BOOST_TR1   || \
     FOREST_ALTERNATIVE_FOR_TUPLE == FAV_MICROSOFT_TR1) \
       && \
     FOREST_ALTERNATIVE_FOR_ARRAY != FOREST_ALTERNATIVE_FOR_TUPLE

#error "Tuple and array type alternatives must be compatible!"

#endif

#endif // FOREST_ALTERNATIVES_HPP_INCLUDED
