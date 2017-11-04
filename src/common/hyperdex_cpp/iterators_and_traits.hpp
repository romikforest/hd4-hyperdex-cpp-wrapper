#ifndef HYPERDEX_TYPE_ITERATORS_AND_TRAITS_HPP_INCLUDED
#define HYPERDEX_TYPE_ITERATORS_AND_TRAITS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: iterators_and_traits.hpp
// First created: 2015-10-18-05.43
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

// C++
#include <type_traits>
#include <iterator>

// forest
#include <forest/tuple.hpp>
#include <forest/array.hpp>
#include <forest/vector.hpp>

// hyperdex
#include <hyperdex_cpp/base_types.hpp>

namespace hyperdex
{

  using forest::tuple;
  using forest::tuple_size;
  using forest::tuple_element;

  using forest::array;
  using forest::vector;

  using std::begin;
  using std::end;
  using std::distance;
  using std::reverse_iterator;

  template<typename T>
  class is_tuple
  {
      struct helper
      {
        template<typename ... TT>
        explicit helper(tuple<TT...> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, T>::value;
  };

  template<typename T, typename = void>
  struct is_homogeneous_tuple
  {
    static constexpr bool const value = false;
  };

  template<typename T, typename...TT>
  struct is_homogeneous_tuple<tuple<T, TT...>, std::enable_if_t<is_homogeneous_tuple<tuple<TT...>>::value &&
                              std::is_same<typename is_homogeneous_tuple<tuple<TT...>>::type,T>::value>>
  {
    static constexpr bool const value = true;
    using type = T;
  };

  template<typename T>
  struct is_homogeneous_tuple<tuple<T>>
  {
    static constexpr bool const value = true;
    using type = T;
  };

  // Trait helpers:

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_attributes;

  template<typename Attributes>
  class is_attributes_class
  {
      struct helper
      {
        template<template<typename, typename = void> typename CStringTraits,
                 template<typename, typename = void> typename ComplexTraits>
        explicit helper(basic_attributes<CStringTraits, ComplexTraits> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Attributes>::value;
  };

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_predicates;

  template<typename Predicates>
  class is_predicates_class
  {
      struct helper
      {
        template<template<typename, typename = void> typename CStringTraits,
                 template<typename, typename = void> typename ComplexTraits>
        explicit helper(basic_predicates<CStringTraits, ComplexTraits> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Predicates>::value;
  };

  template<template<typename, typename = void> typename,
           template<typename, typename = void> typename>
  class basic_map_attributes;

  template<typename MapAttributes>
  class is_map_attributes_class
  {
      struct helper
      {
        template<template<typename, typename = void> typename CStringTraits,
                 template<typename, typename = void> typename ComplexTraits>
        explicit helper(basic_map_attributes<CStringTraits, ComplexTraits> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, MapAttributes>::value;
  };

  template<typename Attribute>
  class is_attribute
  {
      struct helper
      {
        explicit helper(hyperdex_client_attribute const&);
        explicit helper(an_attribute const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Attribute>::value;
  };

  template<typename Attribute>
  class is_attribute_pointer
  {
      struct helper
      {
        explicit helper(hyperdex_client_attribute const*);
        explicit helper(an_attribute const*);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Attribute>::value;
  };

  template<typename Predicate>
  class is_predicate
  {
      struct helper
      {
        explicit helper(hyperdex_client_attribute_check const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Predicate>::value;
  };

  template<typename Predicate>
  class is_predicate_pointer
  {
      struct helper
      {
        explicit helper(hyperdex_client_attribute_check const*);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Predicate>::value;
  };

  template<typename MapAttribute>
  class is_map_attribute
  {
      struct helper
      {
        explicit helper(hyperdex_client_map_attribute const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, MapAttribute>::value;
  };

  template<typename MapAttribute>
  class is_map_attribute_pointer
  {
      struct helper
      {
        explicit helper(hyperdex_client_map_attribute const*);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, MapAttribute>::value;
  };

  template<typename T>
  struct is_tuple_of_attributes
  {
    static constexpr bool const value = false;
  };

  template<typename T, typename...TT>
  struct is_tuple_of_attributes<tuple<T, TT...>>
  {
    static constexpr bool const value = is_tuple_of_attributes<tuple<T>>::value &&
                                        is_tuple_of_attributes<tuple<TT...>>::value;
  };

  template<typename T>
  struct is_tuple_of_attributes<tuple<T>>
  {
    static constexpr bool const value = false;
  };

  template<>
  struct is_tuple_of_attributes<tuple<hyperdex_client_attribute>>
  {
    static constexpr bool const value = true;
  };

  template<>
  struct is_tuple_of_attributes<tuple<an_attribute>>
  {
    static constexpr bool const value = true;
  };

  template<typename T>
  struct is_tuple_of_predicates
  {
    static constexpr bool const value = false;
  };

  template<typename T, typename...TT>
  struct is_tuple_of_predicates<tuple<T, TT...>>
  {
    static constexpr bool const value = is_tuple_of_predicates<tuple<T>>::value &&
                                        is_tuple_of_predicates<tuple<TT...>>::value;
  };

  template<typename T>
  struct is_tuple_of_predicates<tuple<T>>
  {
    static constexpr bool const value = false;
  };

  template<>
  struct is_tuple_of_predicates<tuple<hyperdex_client_attribute_check>>
  {
    static constexpr bool const value = true;
  };

  template<typename T>
  struct is_tuple_of_map_attributes
  {
    static constexpr bool const value = false;
  };

  template<typename T, typename...TT>
  struct is_tuple_of_map_attributes<tuple<T, TT...>>
  {
    static constexpr bool const value = is_tuple_of_map_attributes<tuple<T>>::value &&
                                        is_tuple_of_map_attributes<tuple<TT...>>::value;
  };

  template<typename U>
  struct is_tuple_of_map_attributes<tuple<U>>
  {
    static constexpr bool const value = false;
  };

  template<>
  struct is_tuple_of_map_attributes<tuple<hyperdex_client_map_attribute>>
  {
    static constexpr bool const value = true;
  };

  template<typename T>
  struct is_tuple_of_names
  {
    static constexpr bool const value = false;
  };

  template<typename T, typename...TT>
  struct is_tuple_of_names<tuple<T, TT...>>
  {
    static constexpr bool const value = is_tuple_of_names<tuple<T>>::value &&
                                        is_tuple_of_names<tuple<TT...>>::value;
  };

  template<typename T>
  struct is_tuple_of_names<tuple<T>>
  {
    static constexpr bool const value = false;
  };

  template<>
  struct is_tuple_of_names<tuple<char const*>>
  {
    static constexpr bool const value = true;
  };

  template<typename Container>
  class is_c_array
  {
      struct helper
      {
        template<typename T, size_t size, typename = std::enable_if_t<(size>0)>>
        explicit helper(c_array<T, size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_cpp_array
  {
      struct helper
      {
        template<typename T, size_t size, typename = std::enable_if_t<(size>0)>>
        explicit helper(array<T, size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_array_of_attributes
  {
      struct helper
      {
        template<size_t size>
        explicit helper(array<hyperdex_client_attribute, size> const&);

        template<size_t size>
        explicit helper(array<an_attribute, size> const&);
      };


    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_array_of_predicates
  {
      struct helper
      {
        template<size_t size>
        explicit helper(array<hyperdex_client_attribute_check, size> const&);
      };


    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_array_of_map_attributes
  {
      struct helper
      {
        template<size_t size>
        explicit helper(array<hyperdex_client_map_attribute, size> const&);
      };


    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_array_of_names
  {
      struct helper
      {
        template<size_t size>
        explicit helper(array<char const*, size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_c_array_of_attributes
  {
      struct helper
      {
        template<size_t size>
        explicit helper(an_attribute_c_array<size> const&);

        template<size_t size>
        explicit helper(attribute_c_array<size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_c_array_of_predicates
  {
      struct helper
      {
        template<size_t size>
        explicit helper(predicate_c_array<size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_c_array_of_map_attributes
  {
      struct helper
      {
        template<size_t size>
        explicit helper(map_attribute_c_array<size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_c_array_of_names
  {
      struct helper
      {
        template<size_t size>
        explicit helper(name_c_array<size> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_vector
  {
      struct helper
      {
        template <typename T>
        explicit helper(vector<T> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_vector_of_attributes
  {
      struct helper
      {
        explicit helper(vector<an_attribute> const&);
        explicit helper(vector<hyperdex_client_attribute> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_vector_of_predicates
  {
      struct helper
      {
        explicit helper(vector<hyperdex_client_attribute_check> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_vector_of_map_attributes
  {
      struct helper
      {
        explicit helper(vector<hyperdex_client_map_attribute> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename Container>
  class is_vector_of_names
  {
      struct helper
      {
        explicit helper(vector<char const*> const&);
      };

    public:
      static constexpr bool const value = std::is_constructible<helper, Container>::value;
  };

  template<typename T, typename = std::enable_if_t<is_homogeneous_tuple<T>::value>>
  inline auto begin(T& cont)
  {
    using type = typename tuple_element<0, T>::type;
    return reverse_iterator<type*>(reinterpret_cast<type*>(&cont) + tuple_size<T>::value);
  }

  template<typename T, typename = std::enable_if_t<is_homogeneous_tuple<T>::value>>
  inline auto begin(T const& cont)
  {
    using type = typename tuple_element<0, T>::type;
    return reverse_iterator<type const*>(reinterpret_cast<type const*>(&cont) + tuple_size<T>::value);
  }

  template<typename T, typename = std::enable_if_t<is_homogeneous_tuple<T>::value>>
  inline auto end(T& cont)
  {
    using type = typename tuple_element<0, T>::type;
    return reverse_iterator<type*>(reinterpret_cast<type*>(&cont));
  }

  template<typename T, typename = std::enable_if_t<is_homogeneous_tuple<T>::value>>
  inline auto end(T const& cont)
  {
    using type = typename tuple_element<0, T>::type;
    return reverse_iterator<type const*>(reinterpret_cast<type const*>(&cont));
  }

  template<typename T, typename = void>
  struct has_size_method: std::false_type {};

  template<typename T>
  struct has_size_method<T, std::enable_if_t<std::is_same<decltype(std::declval<T>().size()), size_t>::value>>
  : std::true_type {};

  template<typename T, typename = void>
  struct has_data_method: std::false_type {};

  template<typename T>
  struct has_data_method<T, std::enable_if_t<std::is_pointer<decltype(std::declval<T>().data())>::value>>
  : std::true_type{};

  template<typename T, typename = void>
  struct has_begin_method: std::false_type {};

  template<typename T>
  struct has_begin_method<T, std::enable_if_t<!std::is_same<decltype(std::declval<T>().begin()), void>::value>>
  : std::true_type{};

  template<typename T, typename = void>
  struct has_end_method: std::false_type {};

  template<typename T>
  struct has_end_method<T, std::enable_if_t<!std::is_same<decltype(std::declval<T>().end()), void>::value>>
  : std::true_type{};

  template<typename T, typename = void>
  struct has_reserve_method: std::false_type {};

  template<typename T>
  struct has_reserve_method<T,
    std::enable_if_t<std::is_same<decltype(std::declval<T>().reserve(std::declval<size_t>())), void>::value>>
  : std::true_type{};

  template<typename T>
  inline
  std::enable_if_t<std::is_same<T, an_attribute>::value, hyperdex_client_attribute*>
  attribute_filter(T* p) noexcept { return reinterpret_cast<hyperdex_client_attribute*>(p); }

  template<typename T>
  inline
  std::enable_if_t<std::is_same<T, an_attribute>::value, hyperdex_client_attribute const*>
  attribute_filter(T const* p) noexcept { return reinterpret_cast<hyperdex_client_attribute const*>(p); }

  template<typename T>
  inline
  std::enable_if_t<!std::is_same<T, an_attribute>::value, T*>
  attribute_filter(T* p) noexcept { return p; }

  template<typename T>
  inline
  std::enable_if_t<!std::is_same<T, an_attribute>::value, T const*>
  attribute_filter(T const* p) noexcept { return p; }

  template<typename T, size_t N>
  inline constexpr size_t
  sizeof_c_array(c_array<T,N> const&) { return N; }

  template<typename T, size_t N>
  inline constexpr size_t
  sizeof_cpp_array(cpp_array<T,N> const&) { return N; }

  template<size_t N, typename T>
  inline constexpr
  size_t
  container_size(c_array<T,N> const&)
  {
    return N;
  }

  template<size_t N, typename T>
  inline constexpr
  size_t
  container_size(cpp_array<T,N> const&)
  {
    return N;
  }

  template<typename T>
  inline constexpr
  std::enable_if_t<is_homogeneous_tuple<T>::value, size_t>
  container_size(T const&)
  {
    return tuple_size<T>::value;
  }

  template<typename T>
  inline std::enable_if_t< !is_homogeneous_tuple<T>::value
                        && !std::is_array<T>::value
                        && !is_cpp_array<T>::value
                        && has_size_method<T>::value, size_t>
  container_size(T const& cont)
  {
    return cont.size();
  }

  template<typename T>
  inline std::enable_if_t<!is_homogeneous_tuple<T>::value
                        && !std::is_array<T>::value
                        && !is_cpp_array<T>::value
                        && !has_size_method<T>::value,
  size_t>
  container_size(T const& cont)
  {
    return distance(begin(cont), end(cont));
  }

  template <typename T>
  inline
  std::enable_if_t<!has_reserve_method<T>::value>
  container_reserve(T&, size_t) {}

  template <typename T>
  inline
  std::enable_if_t<has_reserve_method<T>::value>
  container_reserve(T& t, size_t n) { t.reserve(n); }

  template<typename Source, typename Dest, typename InputIterator, typename OutputIterator>
  inline
  OutputIterator
  container_copy(Source const&,
                 Dest const& dest,
                 InputIterator first, InputIterator last,
                 OutputIterator result)
  {
    container_reserve(dest, std::distance(first, last));
    return std::copy(first,last,result);
  }

  template<typename Source, typename Dest, typename InputIterator, typename OutputIterator>
  inline
  OutputIterator
  container_copy(Source const&,
                 Dest const& dest,
                 InputIterator first, InputIterator last,
                 OutputIterator result,
                 size_t sz)
  {
    container_reserve(dest, sz);
    return std::copy(first,last,result);
  }

  template<typename Source, typename Dest, typename InputIterator, typename OutputIterator>
  inline
  OutputIterator
  container_assign(Source const&,
                   Dest const& dest,
                   InputIterator first, InputIterator last)
  {
    container_reserve(dest, std::distance(first, last));
    return std::copy(first,last,hyperdex::begin(dest));
  }

  template<typename Source, typename Dest, typename InputIterator, typename OutputIterator>
  inline
  OutputIterator
  container_assign(Source const&,
                   Dest const& dest,
                   InputIterator first, InputIterator last,
                   size_t sz)
  {
    container_reserve(dest, sz);
    return std::copy(first,last,hyperdex::begin(dest));
  }

  template<typename T>
  struct is_non_cv: std::integral_constant<bool, std::is_same<T, std::remove_cv_t<T>>::value> {};

  namespace helper
  {
    template<typename T>
    std::enable_if_t<!is_non_cv<T>::value,T> const& tref();

    template<typename T>
    std::enable_if_t<is_non_cv<T>::value,T> & tref();
  }

  template<typename T, typename = void>
  struct container_element { using type = void; };

  template<typename T>
  struct container_element<T, std::enable_if_t<!std::is_same<decltype(*begin(helper::tref<T>())), void>::value>>
  {
    using type = std::remove_reference_t<decltype(*begin(helper::tref<T>()))>;
  };

  template<typename T>
  using container_element_t = typename container_element<T>::type;

  template<typename T, typename = void>
  struct is_container_of_non_cv: std::false_type {};

  template<typename T>
  struct is_container_of_non_cv<T, std::enable_if_t<!std::is_same<container_element_t<T>, void>::value>>
  : std::integral_constant<bool,
                           std::is_same<std::remove_cv_t<container_element_t<T>>, container_element_t<T>
                           >::value>
  {};

  template<typename T, typename = void>
  struct iterator_type { using type = void; };

  template<typename T>
  struct iterator_type<T, std::enable_if_t<!std::is_same<decltype(begin(helper::tref<T>())), void>::value>>
  {
    using type = decltype(begin(helper::tref<T>()));
  };

  template<typename T>
  using iterator_type_t = typename iterator_type<T>::type;

  template<typename Iter>
  struct is_reverse_iterator : std::false_type { };

  template<typename Iter>
  struct is_reverse_iterator<std::reverse_iterator<Iter>>
  : std::integral_constant<bool, !is_reverse_iterator<Iter>::value>
  { };

  template<typename T>
  inline
  std::enable_if_t<!is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type*>
  low_address(T& cont)
  {
    return &*begin(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type*>
  low_address(T& cont)
  {
    return &*--end(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<!is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type const*>
  low_address(T const& cont)
  {
    return &*begin(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type const*>
  low_address(T const& cont)
  {
    return &*--end(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<!is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type*>
  up_address(T& cont)
  {
    return &*end(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type*>
  up_address(T& cont)
  {
    return &*--begin(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<!is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type const*>
  up_address(T const& cont)
  {
    return &*end(cont);
  }

  template<typename T>
  inline
  std::enable_if_t<is_reverse_iterator<typename iterator_type<T>::type>::value,
                   typename container_element<T>::type const*>
  up_address(T const& cont)
  {
    return &*--begin(cont);
  }

  template<typename>
  class is_attribute_container;

  template<typename>
  class is_predicate_container;

  template<typename>
  class is_map_attribute_container;

  // traits for safe containers:

  template<typename T, typename = void>
  struct is_linear_safe_container: std::false_type {};

  template<typename T> // c arrays
  struct is_linear_safe_container<T, std::enable_if_t<std::is_array<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_cpp_array<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_vector<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_homogeneous_tuple<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_attributes_class<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_predicates_class<T>::value>>: std::true_type {};

  template<typename T>
  struct is_linear_safe_container<T, std::enable_if_t<is_map_attributes_class<T>::value>>: std::true_type {};

  // Test containers
  template<typename Container>
  class is_attribute_container
  {
    public:
      static constexpr bool const value = (std::is_array<Container>::value ||
                                           has_begin_method<Container>::value  ||
                                           is_homogeneous_tuple<Container>::value) &&
                                          (std::is_same<std::remove_cv_t<container_element_t<Container>>,
                                                        hyperdex_client_attribute>::value ||
                                           std::is_same<std::remove_cv_t<container_element_t<Container>>,
                                                        an_attribute>::value);
  };

  template<typename Container>
  class is_linear_safe_attribute_container
  {
    public:
      static constexpr bool const value = is_linear_safe_container<Container>::value &&
                                          is_attribute_container<Container>::value;
  };

  template<typename Container>
  class is_predicate_container
  {
    public:
      static constexpr bool const value = (std::is_array<Container>::value ||
                                           has_begin_method<Container>::value  ||
                                           is_homogeneous_tuple<Container>::value) &&
                                           std::is_same<std::remove_cv_t<container_element_t<Container>>,
                                                        hyperdex_client_attribute_check>::value;
  };

  template<typename Container>
  class is_linear_safe_predicate_container
  {
    public:
      static constexpr bool const value = is_linear_safe_container<Container>::value &&
                                          is_predicate_container<Container>::value;
  };

  template<typename Container>
  class is_map_attribute_container
  {
    public:
      static constexpr bool const value = (std::is_array<Container>::value ||
                                           has_begin_method<Container>::value  ||
                                           is_homogeneous_tuple<Container>::value) &&
                                           std::is_same<std::remove_cv_t<container_element_t<Container>>,
                                                        hyperdex_client_map_attribute>::value;
  };

  template<typename Container>
  class is_linear_safe_map_attribute_container
  {
    public:
      static constexpr bool const value = is_linear_safe_container<Container>::value &&
                                          is_map_attribute_container<Container>::value;
  };

  template<typename Container>
  class is_name_container
  {
    public:
      static constexpr bool const value = (std::is_array<Container>::value ||
                                           has_begin_method<Container>::value  ||
                                           is_homogeneous_tuple<Container>::value) &&
                                           std::is_same<std::remove_cv_t<container_element_t<Container>>,
                                                        char const*>::value;
  };

  template<typename Container>
  class is_linear_safe_name_container
  {
    public:
      static constexpr bool const value = is_linear_safe_container<Container>::value &&
                                          is_name_container<Container>::value;
  };

} // namespace hyperdex

#endif // HYPERDEX_TYPE_ITERATORS_AND_TRAITS_HPP_INCLUDED
