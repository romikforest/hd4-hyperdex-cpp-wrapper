#ifndef FOREST_LOG_HPP_INCLUDED
#define FOREST_LOG_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: log.hpp
// First created: 2015-09-03-15.11
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_LOG == FAV_NONE
#elif FOREST_ALTERNATIVE_FOR_LOG == FAV_LOG_BOOST_TRIVIAL

#ifndef LOG
#include <boost/log/trivial.hpp>
#define LOG(x) BOOST_LOG_TRIVIAL(x)
#endif // LOG

#else

#error "Wrong alternative for LOG!"

#endif // FOREST_ALTERNATIVE_FOR_LOG


#endif // FOREST_LOG_HPP_INCLUDED
