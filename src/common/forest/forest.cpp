///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: forest.cpp
// First created: 2015-09-03-11.46
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_MEMORY_MANAGER == FAV_NONE
#elif FOREST_MEMORY_MANAGER == FAV_STD
#elif FOREST_MEMORY_MANAGER == FAV_LTALLOC

#include <forest/ltalloc/ltalloc.cc>

#else

#error "Wrong alternative for memory manager"

#endif // FOREST_MEMORY_MANAGER


#include <forest/locale.hpp>

namespace
{
  class ForestApp final
  {
  public:
    ForestApp()
    {
      #if FOREST_ALTERNATIVE_FOR_LOCALE == FAV_BOOST
      boost::locale::generator gen;
      std::locale lc = gen( "" );
      std::locale::global( lc );
      std::cout.imbue( lc );
      std::cin.imbue( lc );
      std::cerr.imbue( lc );
      std::clog.imbue( lc );
      #endif // FOREST_ALTERNATIVE_FOR_LOCALE
    }
  };

  static ForestApp app;

}
