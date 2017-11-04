#ifndef FOREST_TOOLS_HPP_INCLUDED
#define FOREST_TOOLS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: tools.hpp
// First created: 2015-09-03-12.49
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <cassert>

#include <new>
#include <forest/move.hpp>

// ignore unused
#include <boost/core/ignore_unused.hpp>
using boost::ignore_unused;

// statement debug macro
#ifdef NDEBUG
#define SDBG if (0)
#else
#define SDBG
#endif // NDEBUG

#define FOREST_NONCOPYABLE_CLASS(class_name) \
    class_name(class_name const&) = delete; \
    class_name& operator = (class_name const&) = delete

#define FOREST_NONMOVABLE_CLASS(class_name) \
    class_name(class_name&&) = delete; \
    class_name& operator = (class_name&&) = delete

#define FOREST_NONALLOCATABLE_CLASS(class_name) \
    void* operator new (std::size_t) = delete; \
    void* operator new (std::size_t, std::nothrow_t const&) = delete; \
    void* operator new (std::size_t, void*) = delete; \
    void* operator new[](std::size_t) = delete; \
    void* operator new[](std::size_t, std::nothrow_t const&) = delete; \
    void* operator new[](std::size_t, void* ptr) = delete; \
    void operator delete  (void*) = delete; \
    void operator delete[](void*) = delete; \
    void operator delete  (void*, std::nothrow_t const&) = delete; \
    void operator delete[](void*, std::nothrow_t const&) = delete; \
    void operator delete  (void*, std::size_t) = delete; \
    void operator delete[](void*, std::size_t) = delete; \
    void operator delete  (void*, std::size_t, std::nothrow_t const&) = delete; \
    void operator delete[](void*, std::size_t, std::nothrow_t const&) = delete

#define FOREST_DEFAULT_COPYABLE_CLASS(class_name) \
    class_name(class_name const&) = default; \
    class_name& operator = (class_name const&) = default

#define FOREST_DEFAULT_MOVABLE_CLASS(class_name) \
    class_name(class_name&&) = default; \
    class_name& operator = (class_name&&) = default

#define FOREST_DEFAULT_CONSTRUCTABLE_CLASS(class_name) \
    class_name() = default;

#define FOREST_DEFAULT_DESTRUCTABLE_CLASS(class_name) \
    ~class_name() = default;

#define FOREST_DEFAULT_CONSTRUCTABLE_AND_DESTRUCTABLE_CLASS() \
     class_name() = default; \
    ~class_name() = default;

#endif // FOREST_TOOLS_HPP_INCLUDED
