#ifndef FOREST_APP_INFO_HPP_INCLUDED
#define FOREST_APP_INFO_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: app_info.hpp
// First created: 2015-09-03-13.36
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <forest/locale.hpp>
#include <forest/string.hpp>
#include <forest/convert.hpp>

extern char * __progname;

#define prog_name __progname
// #define prog_name program_invocation_short_name

#ifndef SHOW_APP_INFO

#define SHOW_APP_INFO(x) \
{ \
    forest::locale::date_time app_gen_date \
        = forest::locale::period::year   (forest::convert<int>( AutoVersion::YEAR  ).value())  \
          + forest::locale::period::month(forest::convert<int>( AutoVersion::MONTH ).value())  \
          + forest::locale::period::day  (forest::convert<int>( AutoVersion::DATE  ).value()); \
    forest::string par = "" x; \
    std::cout  << prog_name \
               << " version " << AutoVersion::FULLVERSION_STRING \
               << " "  << AutoVersion::STATUS \
               << " (" << AutoVersion::UBUNTU_VERSION_STYLE \
               << " build " << AutoVersion::BUILDS_COUNT << ", " \
               << forest::locale::as::date << app_gen_date \
               << ") " \
               << std::endl; \
    if (!par.empty()) std::cout << par << std::endl; \
    std::cout << std::endl; \
}

#endif // SHOW_APP_INFO

#endif // FOREST_APP_INFO_HPP_INCLUDED
