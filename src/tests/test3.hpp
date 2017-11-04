#ifndef TEST3_HPP_INCLUDED
#define TEST3_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test3.hpp
// First created: 2015-10-16-10.24
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

#include <hyperdex_cpp/hyperdex.hpp>

using forest::string;

void doit(hyperdex::attributes& attrs)
{
  #pragma GCC diagnostic ignored "-Weffc++"
  struct out_data
  {
    string s, v;
    int64_t i;
    double f;
  } out;

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return; }

    auto ans = client.put_sync("kv", "some_key", attrs);

  if (!ans) { LOG(error) << "put error"; return; }

  ans = client.get_sync("kv", "some_key", attrs);

  if (!ans) { LOG(error) << "get error"; return; }

  attrs.by_names(tie(out.v, out.s, out.i, out.f), {"v","s","i","f"});

  std::cout << "v: " << out.v << std::endl
            << "s: " << out.s << std::endl
            << "i: " << out.i << std::endl
            << "f: " << out.f << std::endl;
}

void test3()
{

  std::cout << "Test 3: Simple types" << std::endl;

  struct in_data1
  {
    char const* s = "My text";
    string v = "Some test";
    int i = 10;
    float f = 5.5;
  } in1;

  hyperdex::attributes attrs {
                              { "v", in1.v },
                              { "s", in1.s },
                              { "i", in1.i },
                              { "f", in1.f }
                             };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return; }

  doit(attrs);

  std::cout << "===================================" << std::endl;

  wchar_t wc = 95;
  double f = 35.5;

  attrs.set({{"i", wc}, {"f", f}});
  doit(attrs);

  std::cout << "===================================" << std::endl;

  int fi = 10;
  //unsigned char c = 254;
  signed char c = -126;

  attrs.set({{"i", c}, {"f", fi}});
  doit(attrs);

  std::cout << "===================================" << std::endl;

 // unsigned long  fl = 5000, il = 18446744073709551611LL;
 // long  fl = 5000, il = 9223372036854775804LL;
  long  fl = 5000, il = -9223372036854775804LL;

  attrs.set({{"i", il}, {"f", fl}});
  doit(attrs);

  long iol = attrs["i"];
  std::cout << " iol: " << iol << std::endl;

//  std::cout << "===================================" << std::endl;
//
//  unsigned  fu = 5, iu = 10;
//
//  attrs.set({{"i", iu}, {"f", fu}});
//  doit(attrs);
//
  std::cout << "===================================" << std::endl;

  forest::uint128_t  fu128 = 500000, iu128 = ~forest::uint128_t{0} - 4;

  attrs.set({{"v", iu128}, {"s", fu128}});
  doit(attrs);

  forest::uint128_t fuo128 = attrs["s"], iuo128 = attrs["v"];

  std::cout << "fuo128: " << fuo128 << " iuo128: " << iuo128 << std::endl;

//  std::cout << "===================================" << std::endl;
//
//  forest::int128_t  f128 = -500000, i128 = -1000000;
//
//  attrs.set({{"v", i128}, {"s", f128}});
//  doit(attrs);
//
//  forest::int128_t fo128 = attrs["s"], io128 = attrs["v"];
//
//  std::cout << "fo128: " << fo128 << " io128: " << io128 << std::endl;
//
  std::cout << "===================================" << std::endl;

  long double  fld = -32568.2541;
  short is = 235;

  attrs.set({{"i", is}, {"s", fld}});
  doit(attrs);

  long double fold = attrs["s"];
  short ios = attrs["i"];

  std::cout << "fold: " << fold << " ios: " << ios << std::endl;
//
//  std::cout << "===================================" << std::endl;
//
//  long double  fld2 = 32568.2541;
//  unsigned long iul = -1005000;
//
//  attrs.set({{"i", iul}, {"s", fld2}});
//  doit(attrs);
//
//  long double fold2 = attrs["s"];
//  long ioul = attrs["i"];
//
//  std::cout << "fold2: " << fold2 << " ioul: " << ioul << std::endl;
//
//  std::cout << "===================================" << std::endl;
//
//  unsigned short ius = 2568;
//
//  attrs.set({{"i", ius}});
//  doit(attrs);
//
//  long ious = attrs["i"];
//
//  std::cout << " ious: " << ious << std::endl;
//
//  std::cout << "===================================" << std::endl;

} // test3

#endif // TEST3_HPP_INCLUDED
