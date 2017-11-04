#ifndef TEST25_HPP_INCLUDED
#define TEST25_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test25.hpp
// First created: 2015-10-16-10.32
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <iostream>
#include <typeinfo>
template<typename T>
struct id { typedef T type; };

struct C1 {
    char const* str;
    template <typename T> operator T() const
    { std::cout << "Convert to " << typeid(T).name() << std::endl;; return {}; }
};

struct C2 {
    struct D
    {
      D(std::initializer_list<std::pair<char const*, int>>){}
    };
    C2(C1) { std::cout << "C2 Constructed from C1" << std::endl;; }
//    template<typename T = std::initializer_list<std::pair<char const*, int>>>
//    C2(typename id<T>::type) { std::cout << "C2 constructed from initializer list" << std::endl; }
    C2(D) { std::cout << "C2 constructed from initializer list" << std::endl; }
};

int main() {
    C1 c1{};
//    C2 c2{c1};
    C2 c21({c1});
//    C2 c22{{c1}};
//    C2 c23{ {"bbb", 3}, {"aaa", 3} };
}

=========================================================


#include <iostream>
#include <string>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <hyperdex_cpp/client.hpp>
#include <hyperdex_cpp/client_data.hpp>
#include <hyperdex_cpp/admin.hpp>
#include <hyperdex_cpp/admin_data.hpp>
#include <limits>

#include <forest/rsu.hpp>
#include <set>
int main()
{
  forest::rsu<uint32_t> rsu { 0, 0 };

  std::set<unsigned> set;

  for (int i = 0; i < 256; ++i)
  {
    uint64_t resude = (uint64_t)rsu[i];
    std::cout << resude << " " << (uint64_t)rsu.to_index(resude) << std::endl;
  }
//    set.insert((unsigned)rsu[i]);
//  for (auto x: set)
//    std::cout << x << std::endl;

//  std::cout << "Count: " << set.size() << std::endl;

  std::cout << date::year::min() << std::endl;
  std::cout << date::year::max() << std::endl;
  hyperdex::string_list {"A", "B", "C", "D", "E"};
  hyperdex::int_list { 1, 2, 3, 4, 5 };
  hyperdex::string_list {};
  hyperdex::int_list {};

  hyperdex::string_list {string("A")};
  hyperdex::int_list {2.5};

  hyperdex::an_attribute a1{};
//  hyperdex_client_attribute a1{};
//  hyperdex::string_list {a1};
  hyperdex::string_list { {"a1", 2}, {"a2", 2} };
//  int i = a1;
//  auto const& ra1 = a1;
//  hyperdex::string_list {ra1};

  return 0;
#if 0
  hyperdex::admin adm{};
  adm.add_space_sync("space kd "
    "key k "
    "attributes v "
  "create 8 partitions "
  "tolerate 2 failures");

  char const* stringlist;
  adm.list_spaces(stringlist);
  adm.loop_all();
  std::cout << stringlist;
 // print_c_string_list(std::cout, stringlist);
  return 0;

  using hyperdex::operator<<;

  //int i = 10;
  array<int, 5> ia;
  array<hyperdex_client_attribute, 5> ai;
  array<hyperdex_client_attribute, 5> oi;

  std::cout << hyperdex::is_array_of_attributes<int>::value << std::endl;
  std::cout << hyperdex::is_array_of_attributes<decltype(ia)>::value << std::endl;
  std::cout << hyperdex::is_array_of_attributes<decltype(ai)>::value << std::endl;
  std::cout << hyperdex::is_array_of_attributes<decltype(oi)>::value << std::endl;

  //std::tuple<hyperdex_client_attribute, hyperdex_client_attribute, hyperdex_client_attribute> tup
  hyperdex::attribute_tuple_3 tup
  {
    { "Name 1", "Hen", 3, HYPERDATATYPE_STRING },
    { "Name 2", "Con", 3, HYPERDATATYPE_STRING },
    { "Name 3", "Val", 3, HYPERDATATYPE_STRING }
  };

  hyperdex::an_attribute_tuple_3 a_tup
  {
    { "Name 1", "Hen", 3, HYPERDATATYPE_STRING },
    { "Name 2", "Con", 3, HYPERDATATYPE_STRING },
    { "Name 3", "Val", 3, HYPERDATATYPE_STRING }
  };

  std::cout << "Tuple: " << tup << std::get<0>(tup) << std::endl;

  hyperdex::attributes ft {tup};

  std::cout << "From Tuple: " << ft << std::endl;

  std::tuple<hyperdex_client_attribute, hyperdex_client_attribute, hyperdex_client_attribute> const& tto
    = ft;

//  std::tuple<hyperdex_client_attribute, hyperdex_client_attribute, hyperdex_client_attribute> const tto2 = tto;

  std::cout << "To Tuple: " << tto << std::endl;

  //============
  string s1, s2, s3, s4;
//  int ei;

  char const* na[] = { "Name 1", "Name 2", "Name 3" }; // na, 3,

  auto ti = tie(s1, s2, s3, s4);

  ft.by_names(ti, na, 3);

  std::cout << "Strings:" << std::endl;
  std::cout << "s1: " << s1 << std::endl;
  std::cout << "s2: " << s2 << std::endl;
  std::cout << "s3: " << s3 << std::endl;
  std::cout << "s4: " << s4 << std::endl;
//  std::cout << "ei: " << ei << std::endl;

  //============

  std::tuple<char const*, char const*, char const*> nm
  {
    "Name 1", "Name 2", "Name 3"
  };

  std::tuple<char const*, char const*, char const*> nm1
  {
    "Name 1", "Name 1", "Name 1"
  };

  std::cout << "Nm: " << nm << std::endl;
  std::cout << "It: " << std::get<0>(nm) << std::endl;

  hyperdex::name_array<3> arr { "Name 1", "Name 2", "Name 3" };
  std::cout << arr;

  hyperdex::name_vector vec { "Name 1", "Name 2", "Name 3" };
  std::cout << vec;

  char const* const arrc[]{ "Name 1", "Name 2", "Name 3" };
  std::cout << arrc << std::endl << hyperdex::is_c_array_of_names<decltype(arrc)>::value << std::endl;

  hyperdex::an_attribute_array<3> a_arr {{
    { nullptr, "Val1", 3, HYPERDATATYPE_STRING },
    { nullptr, "Val", 3, HYPERDATATYPE_STRING },
    { nullptr, "Val", 3, HYPERDATATYPE_STRING }
  }};

  hyperdex::an_attribute_vector const v_arr {{
    { nullptr, "Val", 3, HYPERDATATYPE_STRING },
    { nullptr, "Con", 3, HYPERDATATYPE_STRING },
    { nullptr, "Val", 3, HYPERDATATYPE_STRING }
  }};

  ignore_unused(a_arr);

  hyperdex::an_attribute c_arr[] {
    { nullptr, "Val", 3, HYPERDATATYPE_STRING },
    { nullptr, "Con", 3, HYPERDATATYPE_STRING },
    { nullptr, "Val", 3, HYPERDATATYPE_STRING }
  };

  std::cout << "C ARR: " << c_arr;

 // int* iptr = nullptr;
//  hyperdex::fill_names(iptr, 3, &(arr[0]), 3);
//   hyperdex::fill_names(&(a_arr[0]), 3, &(arr[0]), 3);
//   hyperdex::fill_names(&(c_arr[0]), 3, nm);
   auto& ps = a_arr;
//   vector<int> v;
  hyperdex::fill_names(ps, nm);
  //fill_names(a_arr, vec);

  std::cout << "After fill: " << ps;

  hyperdex::attributes m {a_arr};

  //hyperdex::fill_names(tup, nm);

  std::cout << m[1] << std::endl;
  std::cout << m["Name 2"] << std::endl;
  std::cout << "Search: " << hyperdex::attribute_linear_search(&a_arr[0], 3, string("Name 3")) << std::endl;
  std::cout << "Search T: " << hyperdex::attribute_linear_search(tup, string("Name 2") ) << std::endl;
  std::cout << "Search: " << hyperdex::attribute_linear_search(a_arr, string("Name 2") ) << std::endl;
  std::cout << "Search: " << hyperdex::attribute_linear_search(m, string("Name 2") ) << std::endl;
  std::cout << "MSearch: " << m.linear_search(string("Name 2")) << std::endl;

/*
  std::array<hyperdex::an_attribute, 2> arr
    {{
      { "par1", "val1", 4, HYPERDATATYPE_STRING },
      { "par2", "val2", 4, HYPERDATATYPE_STRING }
    }};
  hyperdex::attributes attrs2 { arr };

  std::array<hyperdex::an_attribute, 2> const& arr2 = attrs2;
  const_cast<hyperdex::an_attribute&>(arr2[0]).a.attr = "sic!";
 // hyperdex_client_attribute const* p = attrs2;

  std::cout << "Array:" << std::endl;
  for (auto const& arg: arr) std::cout << arg << std::endl;

  std::cout << "Attributes:" << std::endl;
  for (auto const& arg: attrs2) std::cout << arg << std::endl;

  std::cout << "Array2:" << std::endl;
  for (auto const& arg: arr2) std::cout << arg << std::endl;
*/

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return 1; }


  float f = 5.4;
  hyperdex::string_list ls { "Oggr", string("Hoccc"), string("aa\0bb", 5) };

  std::cout << "LS: " << std::hex << ls << std::endl;

  hyperdex::attributes attrs {
                              {"v", "Hello, Piter!"},
                              //{"i", 10.5}
                              //{ "f", f }
 //                             { "s", _ftp },
                              { "ls", ls },
 //                             { "tsec", _tp },
 //                             { "mss", mss },
 //                             { "mif", mif },
                               { "f", f }
                             };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return 1; }

  hyperdex::client_answer ans = client.put("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "put error"; return 1; }
  else                      LOG(info)  << "put done";

  ans = client.get("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "get error"; return 1; }
  else                      LOG(info)  << "get done";
  std::cout << "Element ls: " << attrs["ls"] << std::endl;

  ls = attrs["ls"];

  std::cout << "LS: " << std::endl;
  for (const auto& val: ls)
  {
//    for (const auto& vval: val)
    std::cout << val << std::endl;
  }

  float mf = 5;
  hyperdex::string_list mls;
  string mv;

//  char const* marr[2] = {"f", "v"};

// attrs.by_names(tie(mf,mv), marr);
//   attrs.by_names(tie(mf,mv), {"f", "v"});

//  std::cout << "f: " << mf << std::endl;
 // std::cout << "ls: " << mls << std::endl;
//  std::cout << "v: " << mv << std::endl;

//  char const* marr[3] = {"f", "ls", "v"};

//  attrs.by_names(tie(mf,mls,mv), marr);
  attrs.by_names(tie(mf,mls,mv), {"f", "ls", "v"});

  std::cout << "f: " << mf << std::endl;
  std::cout << "ls: " << mls << std::endl;
  std::cout << "v: " << mv << std::endl;
#endif
}

#if 0

#include <type_traits>
#include <cstddef>
#include <iostream>
using namespace std;

struct null_helper { static constexpr int const value = 0; };

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
class constructible_from_order
{
    using PrevType = conditional_t<(order < 1), null_helper,
                                           constructible_from_order<Helper, order-1, TT&&...>>;
    static constexpr int const prev = PrevType::value;
    static constexpr bool const is_this_constructible = is_constructible<Helper<order>, TT&&...>::value;
  public:
    static constexpr int const value = prev ? prev : is_this_constructible ? order : 0;

}; // template class constructible_from_order

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order = enable_if<(constructible_from_order<Helper, order, TT&&...>::value == order)>;

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order_t = typename enable_in_order<Helper, order, TT&&...>::type;

using blob_data = pair<char const*, size_t>;

class C {

    template<unsigned order>
    class helper
    {
      public:
        helper(char const*, size_t) {} // 1

        helper(blob_data const&, blob_data const&) {} // 1

        template<typename T, typename = enable_if_t<(order == 2) && sizeof(T)>>
        helper(blob_data const&, T&&) {} // 2

        template<typename T, typename = enable_if_t<(order == 3) && sizeof(T)>>
        helper(T&&, blob_data const&) {} // 3

        template <class... Ts, typename = enable_if_t<(order == 4) && sizeof...(Ts)>>
        helper(Ts&&... ) {} // 4

    }; // template class helper

  public: // constructors:

    // order 1
    C(char const*, size_t) { cout << "1" << endl; }

    // order 1
    C(blob_data const&, blob_data const&) { cout << "1" << endl; }

    // order 2
    template<typename T, typename = enable_in_order_t<helper, 2, blob_data const&, T&&>>
    C(blob_data const&, T&&) { cout << "2" << endl; }

    // order 3
    template<typename T, typename = enable_in_order_t<helper, 3, T&&, blob_data const&>>
    C(T&&, blob_data const&) { cout << "3" << endl; }

    // order 4
    template <class... Ts, typename = enable_in_order_t<helper, 4, Ts&&... >>
    C(Ts&&... ) { cout << "4" << endl;}

  public: // member functions:

    // order 1
    void fun(char const*, size_t) { cout << "1" << endl; }

    // order 1
    void fun(blob_data const&, blob_data const&) { cout << "1" << endl; }

    // order 2
    template<typename T, typename = enable_in_order_t<helper, 2, blob_data const&, T&&>>
    void fun(blob_data const&, T&&) { cout << "2" << endl; }

    // order 3
    template<typename T, typename = enable_in_order_t<helper, 3, T&&, blob_data const&>>
    void fun(T&&, blob_data const&) { cout << "3" << endl; }

    // order 4
    template <class... Ts, typename = enable_in_order_t<helper, 4, Ts&&... >>
    void fun(Ts&&... ) { cout << "4" << endl;}

}; // class C


int main() {

  char const* str = "aaa";

  // constructors:
  cout << "Constructors: " << endl;
  cout << "1: "; C  c1   {        str,  size_t{5} };
  cout << "1: "; C  cx   { { str, 5 }, { str, 5 } };
  cout << "2: "; C  c2   { { str, 5 },        str };
  cout << "3: "; C  c3   {        str, { str, 5 } };
  cout << "4: "; C  c4   {        str,        str };
  cout << endl;

  // functions:
  cout << "Functions: " << endl;
  cout << "1: "; c1.fun(        str,  size_t{5} );
  cout << "1: "; c1.fun( { str, 5 }, { str, 5 } );
  cout << "2: "; c1.fun( { str, 5 },        str );
  cout << "3: "; c1.fun(        str, { str, 5 } );
  cout << "4: "; c1.fun(        str,        str );
  cout << endl;

} // main

#endif

#if 0

#include <type_traits>
#include <cstddef>
#include <iostream>
using namespace std;

struct null_helper { static constexpr int const value = 0; };

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
class constructible_from_order
{
    using PrevType = typename conditional_t<(order < 1), null_helper,
                                           constructible_from_order<Helper, order-1, TT&&...>>;
    static constexpr int const prev = PrevType::value;
    static constexpr bool const is_this_constructible = is_constructible<Helper<order>, TT&&...>::value;
  public:
    static constexpr int const value = prev ? prev : is_this_constructible ? order : 0;

}; // template class constructible_from_order

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order = enable_if<(constructible_from_order<Helper, order, TT&&...>::value == order)>;

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order_t = typename enable_in_order<Helper, order, TT&&...>::type;

using blob_data = pair<char const*, size_t>;

class C {

    template<unsigned order>
    class helper
    {
      public:
        helper(char const*, size_t) {} // 1

        helper(blob_data const&, blob_data const&) {} // 1

        template<typename T, typename = enable_if_t<(order == 2) && sizeof(T)>>
        helper(blob_data const&, T&&) {} // 2

        template<typename T, typename = enable_if_t<(order == 3) && sizeof(T)>>
        helper(T&&, blob_data const&) {} // 3

        template <class... Ts, typename = enable_if_t<(order == 4) && sizeof...(Ts)>>
        helper(Ts&&... ) {} // 4

    }; // template class helper

  public: // constructors:

    // order 1
    C(char const*, size_t) { cout << "1" << endl; }

    // order 1
    C(blob_data const&, blob_data const&) { cout << "1" << endl; }

    // order 2
    template<typename T, typename = enable_in_order_t<helper, 2, blob_data const&, T&&>>
    C(blob_data const&, T&&) { cout << "2" << endl; }

    // order 3
    template<typename T, typename = enable_in_order_t<helper, 3, T&&, blob_data const&>>
    C(T&&, blob_data const&) { cout << "3" << endl; }

    // order 4
    template <class... Ts, typename = enable_in_order_t<helper, 4, Ts&&... >>
    C(Ts&&... ) { cout << "4" << endl;}

  public: // member functions:

    // order 1
    void fun(char const*, size_t) { cout << "1" << endl; }

    // order 1
    void fun(blob_data const&, blob_data const&) { cout << "1" << endl; }

    // order 2
    template<typename T, typename = enable_in_order_t<helper, 2, blob_data const&, T&&>>
    void fun(blob_data const&, T&&) { cout << "2" << endl; }

    // order 3
    template<typename T, typename = enable_in_order_t<helper, 3, T&&, blob_data const&>>
    void fun(T&&, blob_data const&) { cout << "3" << endl; }

    // order 4
    template <class... Ts, typename = enable_in_order_t<helper, 4, Ts&&... >>
    void fun(Ts&&... ) { cout << "4" << endl;}

}; // class C


int main() {

  char const* str = "aaa";

  // constructors:
  cout << "Constructors: " << endl;
  cout << "1: "; C  c1   {        str,  size_t{5} };
  cout << "1: "; C  cx   { { str, 5 }, { str, 5 } };
  cout << "2: "; C  c2   { { str, 5 },        str };
  cout << "3: "; C  c3   {        str, { str, 5 } };
  cout << "4: "; C  c4   {        str,        str };
  cout << endl;

  // functions:
  cout << "Functions: " << endl;
  cout << "1: "; c1.fun(        str,  size_t{5} );
  cout << "1: "; c1.fun( { str, 5 }, { str, 5 } );
  cout << "2: "; c1.fun( { str, 5 },        str );
  cout << "3: "; c1.fun(        str, { str, 5 } );
  cout << "4: "; c1.fun(        str,        str );
  cout << endl;

} // main

#endif

#if 0

#include <type_traits>
#include <cstddef>
#include <iostream>

template<template<class...>class Z, class T>
struct is_template:std::false_type{};
template<template<class...>class Z, class...Ts>
struct is_template<Z, Z<Ts...>>:std::true_type{};

struct foo {
private:
  template<class T> struct tag{ explicit tag(int) {} };
public:
  foo( tag<std::true_type>, char const*, size_t );
  template<class...Ts>
  foo( tag<std::false_type>, Ts&&...ts );

public:
  foo() = default; // or whatever
  template<class T0, class...Ts,
    std::enable_if_t<!is_template<tag, std::decay_t<T0>>{},int> =0>
  foo(T0&&t0, Ts&&...ts):
    foo( tag<typename std::is_constructible< foo, tag<std::true_type>, T0&&, Ts&&... >::type>{0}, std::forward<T0>(t0), std::forward<Ts>(ts)... )
  {}
};

int main()
{

}
#endif
#if 0

#include <type_traits>
#include <cstddef>
#include <iostream>
using namespace std;

struct null_helper { static constexpr int const value = 0; };

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
class constructible_from_order
{
    using PrevType = typename conditional_t<(order < 1),
                                           null_helper,
                                           constructible_from_order<Helper, order-1, TT&&...>>;

    static constexpr int const prev = PrevType::value;

    static constexpr bool const is_this_constructible = is_constructible<Helper<order>, TT&&...>::value;

  public:

    static constexpr int const value = prev ? prev : is_this_constructible ? order : 0;
};

template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order = enable_if<(constructible_from_order<Helper, order, TT&&...>::value == order)>;


/*
template<template<unsigned> typename Helper, unsigned order, typename ...TT,
         typename = enable_if<(constructible_from_order<Helper, order, TT&&...>::value == order)> >
struct enable_in_order
{
  static constexpr bool const type = void;
};

template<template<unsigned> typename Helper, unsigned order, typename ...TT,
         typename = enable_if<(constructible_from_order<Helper, order, TT&&...>::value != order)> >
struct enable_in_order {};
*/
template<template<unsigned> typename Helper, unsigned order, typename ...TT>
using enable_in_order_t = typename enable_in_order<Helper, order, TT&&...>::type;

struct S { int field; };

using F = S;  // or
//using F = size_t;

template<unsigned order>
class helper
{
  public:
    helper(char const*, size_t) {} // 1

    template<typename T, typename = enable_if_t<(order == 2) && sizeof(T)>>
    helper(F const&, T&&) {} // 2

    template<typename T, typename = enable_if_t<(order == 3) && sizeof(T)>>
    helper(T&&, F const&) {} // 3

    template <class... Ts, typename = enable_if_t<(order == 4) && sizeof...(Ts)>>
    helper(Ts&&... ) {} // 4
};

class C {

  public:

    C(char const*, size_t) { cout << "1" << endl; } // 1

    template<typename T, typename = enable_in_order_t<helper, 2, F const&, T&&>>
    C(F const&, T&&) { cout << "2" << endl; } // 2

    template<typename T, typename = enable_in_order_t<helper, 3, T&&, F const&>>
    C(T&&, F const&) { cout << "3" << endl; } // 3


    template <class... Ts, typename = enable_in_order_t<helper, 4, Ts&&... >>
    C(Ts&&... ) { cout << "4" << endl;} // 4
};

int main() {
  char const* str = "aaa";
  F f {5};
  int c = 'a';

  cout << "1: "; C c1(str, size_t{5});
  cout << "2: "; C c2(f, c);
  cout << "3: "; C c3(c, f);
  cout << "4: "; C c4(str, str);

//  int a = 0;
//  C x{a};
//  cout << "From int? :" << is_constructible<C, int&>{} << endl;
}
#endif

#if 0

#include <type_traits>
#include <cstddef>
#include <iostream>
using namespace std;

struct S { int field; };

using F = S;  // or
//using F = size_t;

class C {

    class null_helper
    {
    public:
      static constexpr bool const constructible_this = false;
      static constexpr int  const constructible_from = 0;
    };

    template<int i, typename ...TT>
    class helper
    {
        using PrevType = typename conditional_t<(i < 1), null_helper, helper<i-1, TT&&...>>;
        static constexpr int const prev = PrevType::constructible_from;
        static constexpr bool const is_this_constructible = is_constructible<helper, TT&&...>::value;

      public:

        static constexpr int const constructible_from
          = prev ? prev : is_this_constructible ? i : 0;

        helper(char const*, size_t) {} // 1

        template<typename T, typename = enable_if_t<(i == 2) && sizeof(T)  // && !is_constructible<helper<1>, F const&, T&&>::value
        >>
        helper(F const&, T&&) {} // 2

        template<typename T, typename = enable_if_t<(i == 3) && sizeof(T) // && !is_constructible<helper<2>, T&&, F const&>::value
        >>
        helper(T&&, F const&) { cout << "3" << endl; } // 3

        template <class... Ts, typename = enable_if_t<(i == 4) && sizeof...(Ts) // && !is_constructible<helper<3>, Ts&&...>::value
        >>
        helper(Ts&&... ) { cout << "4" << endl;} // 4
    };

  public:

    C(char const*, size_t) { cout << "1" << endl; } // 1

    template<typename T, typename = enable_if_t<helper<2, F const&, T&&>::constructible_from == 2>>
    C(F const&, T&&) { cout << "2" << endl; } // 2

    template<typename T, typename = enable_if_t<helper<3, T&&, F const&>::constructible_from == 3>>
    C(T&&, F const&) { cout << "3" << endl; } // 3


    template <class... Ts, typename = enable_if_t<helper<4, Ts&&... >::constructible_from == 4>>
    C(Ts&&... ) { cout << "4" << endl;} // 4
};

int main() {
  char const* str = "aaa";
  F f {5};
  int c = 'a';

  cout << "1: "; C c1(str, size_t{5});
  cout << "2: "; C c2(f, c);
  cout << "3: "; C c3(c, f);
  cout << "4: "; C c4(str, str);

//  int a = 0;
//  C x{a};
//  cout << "From int? :" << is_constructible<C, int&>{} << endl;
}
#endif
#if 0

#include <iostream>
#include <string>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <hyperdex_cpp/client.hpp>
#include <hyperdex_cpp/client_data.hpp>
#include <limits>

int main()
{
/*
  std::array<hyperdex::an_attribute, 2> arr
    {{
      { "par1", "val1", 4, HYPERDATATYPE_STRING },
      { "par2", "val2", 4, HYPERDATATYPE_STRING }
    }};
  hyperdex::attributes attrs2 { arr };

  std::array<hyperdex::an_attribute, 2> const& arr2 = attrs2;
  const_cast<hyperdex::an_attribute&>(arr2[0]).a.attr = "sic!";
  hyperdex_client_attribute const* p = attrs2;

  std::cout << "Array:" << std::endl;
  for (auto const& arg: arr) std::cout << arg << std::endl;

  std::cout << "Attributes:" << std::endl;
  for (auto const& arg: attrs2) std::cout << arg << std::endl;

  std::cout << "Array2:" << std::endl;
  for (auto const& arg: arr2) std::cout << arg << std::endl;
*/
  string eee = "Wow!";

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return 1; }

  /*
  hyperdex::attributes a { {"ggg", "eoo", 3},
                           {"Here", "mmm", 3},
                           {string("Look"), "abnr", 4},
                           {eee, "aa", 2},
                           {"test1", eee},
                           {"test2", string("Moo!")},
                           {"test3", "just a test"},
                           {string("test4"), string("Moo!")},
                         };
  */

  // string f { "Ser test 2" };
  //std::vector<std::string> mv {"Loo lou loy", "Meu", "Me"};
  //hyperdex::list<std::vector<std::string>> ls(mv);
  float f = 5.4;
  //wchar_t const* f  { L"Nmmm!!!" };
//   hyperdex::blob_list ls { {"Lololo", 6}, { "Yum", 3 } };
//    hyperdex::hyperdex_int128  big = 125665585LL;
//  ignore_unused(big);
//  long double bigf = 256.75;
//    hyperdex::timestamp_second _tp{2018, 2, 4, 14, 30, 29, 28000};
//  hyperdex::full_timestamp _ftp{3389, 5, 4, 12, 00, 35, 94};
//  hyperdex::int128_list ls { big, big, bigf };
//  hyperdex::full_timestamp_list ls { _ftp };
//  hyperdex::int_list ls { 10, 20 };
//  if (!ls) std::clog << "Error!";
//  hyperdex::timestamp_second_list ls { };
  //hyperdex::list<string> ls { "Aff", "Hax", string("Lol"), std::pair<char const*,int>("aaa", 3) };
  hyperdex::string_list ls { "Oggr", string("Hoccc"), string("aa\0bb", 5) };
//  ls.append("Google!");
  //hyperdex::blob_list ls { "", 0 };
  //hyperdex::string_list ls { "ttt", string("hhh"), std::make_pair("yyy", 2), "sss" };
  //hyperdex::string_set ss{};
  //hyperdex::int_list ls { "", 0 };

  // hyperdex::an_attribute au{nullptr, nullptr, 0, HYPERDATATYPE_LIST_STRING};
  //const hyperdex_client_attribute au{nullptr, nullptr, 0, HYPERDATATYPE_LIST_STRING};
  //hyperdex::blob_list ls{au};
//  ls.append("chrty", 5);
//  ls.append("ggg1");
//  ls.append(string("ggg2"));
//  ls.append(9.5);
//  ls.append(int(20));
//  ls.append(big);
//  ls.append(bigf);
//  ls.append(_tp);

  std::cout << "LS: " << std::hex << ls << std::endl;
//  for (const auto& val: ls)
//  {
//    for (const auto& vval: val)
//    std::cout << val << std::endl;
//  }

//  time_t _tt =  2589354125; //std::numeric_limits<time_t>::max();

//  hyperdex::full_timestamp _ftp{};
//  _ftp.from_time_t(_tt);
//  hyperdex::hyperdex_int128  big = 125665585LL;
//  ignore_unused(big);
//  long double bigf = 256.75;
//  hyperdex::timestamp_second _tp{2018, 2, 4, 14, 30, 29, 28000};
//  std::cout << "_ftp is: " << _ftp << std::endl;
//  std::cout << "_tp is: " << _tp << std::endl;
//  _tp.from_hyperdex(_tp.to_hyperdex());
  //_tp.from_time_t(_tt);
//  std::cout << (_tp.is_in_hyperdex_limits()?"true":"false") << " !!!Ho\n";
//  std::cout << (_tp.lost_precision_to_hyperdex()?"true":"false") << " !!!Ho\n";

//  hyperdex::string_string_map mss { {"k1", "v1"}, {"k2", "v2"} };
  // hyperdex::map<char const*, string> mss {{ "yy", { "v2\0ggg", 6 } }, { "gg", big }  };
//  hyperdex::map<char const*, hyperdex::full_timestamp> mss { { "gg", _ftp }  };
//  mss.insert("test time", _tp);
//  mss.insert("test ins", big);
//  mss.insert("test bf", bigf);
//  mss.insert("k1", "v1");
//  mss.insert("k2", string("v2"));
//  mss.insert(std::pair<const char*, size_t>("kk", 2), "dd");
//  mss.insert(std::make_pair("Nn", 2), std::make_pair("Lo", 2));
//  mss.insert("k3", std::make_pair("Lo", 2));
//  const std::pair<char const*, size_t> p {"Eh", 2};
//  mss.insert("k", p);
//  mss.insert("k4", {"Le", 2});
//  mss.insert({"k5", 2}, {"Li", 2});
//  mss.insert({"Le", 2}, string("mnm"));
//  mss.insert("Moo", {"TT", 2});
//  mss.insert("A", {"FF", 2});
//  mss.insert("k4", 5);
//mss.insert(5, 5);

//  std::cout << "MSS: " << std::endl;
//  for (const auto& val: mss)
//  {
//    std::cout << val << std::endl;
//  }

//  hyperdex::map<hyperdex::timestamp_second, hyperdex::hyperdex_float> mif { {_tp, bigf} };
//  _tp += std::chrono::seconds(1000);
//  mif.insert(_tp, 25 );
//  mif.insert(9, bigf);
  //hyperdex::int_float_map mif { {1, big} };
//  mif.insert(2, big);
//  mif.insert(5, 2);
//  mif.insert(6, 1.5);
//  mif.insert({ "dd", 2}, 1);

//  std::cout << "MIF: " << std::endl;
//  for (const auto& val: mif)
//  {
//    std::cout << val << std::endl;
//  }

  hyperdex::attributes attrs {
                              {"v", "Hello, Piter!"},
                              //{"i", 10.5}
                              //{ "f", f }
 //                             { "s", _ftp },
                              { "ls", ls },
 //                             { "tsec", _tp },
 //                             { "mss", mss },
 //                             { "mif", mif },
                               { "f", f }
                             };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return 1; }

  hyperdex::client_answer ans = client.put("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "put error"; return 1; }
  else                      LOG(info)  << "put done";

  ans = client.get("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "get error"; return 1; }
  else                      LOG(info)  << "get done";

  // std::cout << attrs;

  //std::cout << " Attrs are:" << std::endl << std::endl;
  //for (auto& a: attrs)
  //{
  //  std::cout << a << std::endl;
  //}

//  std::cout << "Element v: " << attrs["v"] << std::endl;
  std::cout << "Element ls: " << attrs["ls"] << std::endl;
//  std::cout << "Element mss: " << attrs["mss"] << std::endl;
//  std::cout << "Element mif: " << attrs["mif"] << std::endl;

  ls = attrs["ls"];
 // ls.append(33);
  //ls.append("Hoi!");
//  mss = attrs["mss"];
//  mss.insert("x", "ddt");
//  mif = attrs["mif"];
//  mif.insert(5, 3.2);

  std::cout << "LS: " << std::endl;
  for (const auto& val: ls)
  {
//    for (const auto& vval: val)
    std::cout << val << std::endl;
  }

//  std::cout << "MSS: " << std::endl;
//  for (const auto& val: mss)
//  {
//    std::cout << val << std::endl;
//  }

//  std::cout << "MIF: " << std::endl;
//  for (const auto& val: mif)
//  {
//    std::cout << val << std::endl;
//  }

  // hyperdex::an_attribute a { "AName", "AValue", 6, HYPERDATATYPE_STRING };



  //hyperdex::timestamp_second tp { 3000, 1, 1, 12, 11, 22, 15 };

//  time_t tt =  std::numeric_limits<time_t>::max(); // 2589354125;

//  std::cout << "time_t out: " << tt << std::endl;
  // std::cout << "time_t out: " << asctime(gmtime(&tt)) << std::endl;

//  hyperdex::timestamp_second tp {};
  //tp.from_time_t(tt);
//  *reinterpret_cast<int128_t*>(&tp) = int128_t(~uint128_t(0) >> 1);

  //tp += std::chrono::nanoseconds(35);

//  std::cout << "Time: " << tp << std::endl;

  // time_t tt2 = tp.to_time_t();

  // std::cout << "time_t2 out: " << asctime(gmtime(&tt2)) << std::endl;

//  hyperdex::attribute_serializer as;

//  hyperdex::an_attribute a = as.serialize(string("Hoi"), tp);

//  string s = a;

//  std::cout << s << std::endl << std::endl;

//  std::cout << a << std::endl;

//  hyperdex::full_timestamp out { a };

//  std::cout << "Out: " << out << std::endl;

//  std::cout << "Size of unsigned: " << sizeof(unsigned) << std::endl;

}
#endif

#if 0

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <hyperdex_cpp/client.hpp>
#include <hyperdex_cpp/client_data.hpp>
#include <limits>

int main()
{

  string eee = "Wow!";

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return 1; }

  /*
  hyperdex::attributes a { {"ggg", "eoo", 3},
                           {"Here", "mmm", 3},
                           {string("Look"), "abnr", 4},
                           {eee, "aa", 2},
                           {"test1", eee},
                           {"test2", string("Moo!")},
                           {"test3", "just a test"},
                           {string("test4"), string("Moo!")},
                         };
  */

  // string f { "Ser test 2" };
  std::vector<std::string> f {"Loo lou loy"};
  //wchar_t const* f  { L"Nmmm!!!" };
  //hyperdex::blob_list ls { {"Lololo", 6}, { "Yum", 3 } };
  hyperdex::string_list ls { "Aff", "Hax", "De" };

  std::cout << "LS: " << std::endl;
  for (const auto& val: ls)
  {
    std::cout << val << std::endl;
  }

  time_t _tt =  2589354125; //std::numeric_limits<time_t>::max();

  hyperdex::full_timestamp _ftp{};
  _ftp.from_time_t(_tt);

  hyperdex::timestamp_second _tp{2018, 2, 4, 14, 30, 29, 28000};
  //_tp.from_time_t(_tt);
  std::cout << (_tp.is_in_hyperdex_limits()?"true":"false") << " !!!Ho\n";
  std::cout << (_tp.lost_precision_to_hyperdex()?"true":"false") << " !!!Ho\n";

  hyperdex::attributes attrs {
                              {"v", "Hello, Piter!"}
                              // {"i", 10.5}
                              // { "f", f }
 //                             { "s", _ftp },
 //                             { "ls", ls },
 //                             { "tsec", _tp }
                             };

  if (!attrs) { LOG(error) << "Error creating attributes!"; return 1; }

  hyperdex::client_answer ans = client.put("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "put error"; return 1; }
  else                      LOG(info)  << "put done";

  ans = client.get("kv", "some_key", attrs);

  if (!client.loop_all()) { LOG(error) << "get error"; return 1; }
  else                      LOG(info)  << "get done";

  // std::cout << attrs;

  //std::cout << " Attrs are:" << std::endl << std::endl;
  //for (auto& a: attrs)
  //{
  //  std::cout << a << std::endl;
  //}

  std::cout << "Element v: " << attrs["v"] << std::endl;
  std::cout << "Element ls: " << attrs["ls"] << std::endl;

  ls = attrs["ls"];

  std::cout << "LS: " << std::endl;
  for (const auto& val: ls)
  {
    std::cout << val << std::endl;
  }

  // hyperdex::an_attribute a { "AName", "AValue", 6, HYPERDATATYPE_STRING };


  //hyperdex::timestamp_second tp { 3000, 1, 1, 12, 11, 22, 15 };

  time_t tt =  std::numeric_limits<time_t>::max(); // 2589354125;

  std::cout << "time_t out: " << tt << std::endl;
  // std::cout << "time_t out: " << asctime(gmtime(&tt)) << std::endl;

  hyperdex::timestamp_second tp {};
  //tp.from_time_t(tt);
  *reinterpret_cast<int128_t*>(&tp) = int128_t(~uint128_t(0) >> 1);

  //tp += std::chrono::nanoseconds(35);

  std::cout << "Time: " << tp << std::endl;

  // time_t tt2 = tp.to_time_t();

  // std::cout << "time_t2 out: " << asctime(gmtime(&tt2)) << std::endl;

  hyperdex::attribute_serializer as;

  hyperdex::an_attribute a = as.serialize(string("Hoi"), tp);

  string s = a;

  std::cout << s << std::endl << std::endl;

  std::cout << a << std::endl;

  hyperdex::full_timestamp out { a };

  std::cout << "Out: " << out << std::endl;

  std::cout << "Size of unsigned: " << sizeof(unsigned) << std::endl;


}

#endif

/*
#include <hyperdex_cpp/client.hpp>
#include <hyperdex_cpp/client_data.hpp>

int main()
{
  string eee = "Wow!";

  hyperdex::client client;
  if (!client) { LOG(error) << "Error creating hyperdex client!"; return 1; }


//  hyperdex::attributes a { {"ggg", "eoo", 3},
//                           {"Here", "mmm", 3},
//                           {string("Look"), "abnr", 4},
//                           {eee, "aa", 2},
//                           {"test1", eee},
//                           {"test2", string("Moo!")},
//                           {"test3", "just a test"},
//                           {string("test4"), string("Moo!")},
//                         };

  hyperdex::attributes attrs { {"v", string("Hello, Piter!")} };
  if (!attrs) { LOG(error) << "Error creating attributes!"; return 1; }

  // hyperdex::client_answer ans = client.put("kv", "some_key", 8, attrs);
  hyperdex::client_answer ans = client.put(string("kv"), string("some_key"), attrs);

  std::cout << ans;

  if (ans) ans = client.loop();

  if (!ans) { LOG(error) << "put error"; return 1; }
  else       LOG(info)  << "put done";

  ans = client.get("kv", "some_key", 8, attrs);
  // ans = client.get("kv", "some_key", attrs);

  if (ans) ans = client.loop();

  if (!ans) { LOG(error) << "get error"; return 1; }
  else       LOG(info)  << "get done";

  std::cout << attrs;

}
*/

/*
#include <forest/forest.hpp>
#include <forest/tools.hpp>
#include <forest/app_info.hpp>
#include <forest/log.hpp>

#include <hyperdex_cpp/hyperdex.hpp>

#include "version.h"

int main( int argc, char const** argv )
{
  ignore_unused( argc );
  ignore_unused( argv );

  SHOW_APP_INFO("Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)");

  // LOG(trace) << "A trace severity message";
  // LOG(debug) << "A debug severity message";
  // LOG(info) << "An informational severity message";
  // LOG(warning) << "A warning severity message";
  // LOG(error) << "An error severity message";
  // LOG(fatal) << "A fatal severity message";

  hyperdex::client client();

  return 0;
}
*/

#if 0

#include <iostream>
#include <type_traits>

struct S {};

class C
{
private:
  int field;

  public:

    class helper
    {
    public:
      constexpr helper(C* obj, S const&) {
        obj->field = 5;
        // std::cout << " fun(S const& ) " << std::endl;
        }

      template<typename ... T, typename = std::enable_if_t<!std::is_constructible<helper, T&&...>::value> >
      constexpr helper(C* obj, T&&...) {
        // std::cout << " fun(T&&...) " << std::endl;
        obj->field = 3;
        }
    };

  template<typename ... T>
  void fun(T&&... args) { helper { this, std::forward<T>(args)... }; }


};


int main ()
{
	S s{};
	C c{};

	c.fun(s);
	c.fun(5);

}

#endif




#endif // TEST25_HPP_INCLUDED
