#ifndef HYPERDEX_CLIENT_HPP_INCLUDED
#define HYPERDEX_CLIENT_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: client.hpp
// First created: 2015-09-10-10.13
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifndef __cplusplus
#error HyperDex c++ wrapper needs c++ compiler
#endif

#if __cplusplus <= 201103L
#error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

static_assert(!~(-1), "Two's complement arithmetic is required in this code."
              " (or change all [!=] to [-1 ==], check and [~] operations)");

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/client_answer.hpp>
#include <hyperdex_cpp/attributes.hpp>
#include <hyperdex_cpp/predicates.hpp>
#include <hyperdex_cpp/map_attributes.hpp>
#include <hyperdex_cpp/log.hpp>

#ifdef HYPERDEX_USE_GOOGLE_MACAROONS
#include <hyperdex_cpp/macaroons.hpp>
#endif // HYPERDEX_USE_GOOGLE_MACAROONS

// forest
#include <forest/unique_ptr.hpp>
#include <forest/tools.hpp>

// boost
#include <boost/preprocessor/cat.hpp>

// C++
#include <utility>
#include <unordered_map>

#define __HYPERDEX_ANSWER_LOCK \
  client_answer ans {std::nothrow}; \
  if (!ans.a_) \
  { \
    log_error(ans); \
    return ans; \
  } \
  ans.a_->m.phase_ = 1; \
  if (lock_) { ans.a_->m.lock_ = true; return ans; } \
  seria_ = true; \
  /**/

#define __HYPERDEX_ANSWER_LOCK_S \
  client_answer ans {std::nothrow}; \
  if (!ans.a_) \
  { \
    log_error(ans); \
    return ans; \
  } \
  ans.a_->m.phase_ = 1; \
  \
  if(seria_ || lock_ || !ops.empty()) \
  { \
    ans.a_->m.is_completed_ = true; \
    ans.a_->m.last_op_ = true; \
    ans.a_->m.lock_ = lock_; \
    internal_error = " - sync. operation while seria was started or operations locked"; \
    log_error(ans); \
    return ans; \
  } \
  /**/

#define __HYPERDEX_SPACE_CHECK \
  if ( space.status != HYPERDEX_DS_SUCCESS ) \
  { \
    ans.a_->status_ = ds_to_client_status(space.status); \
    return ans; \
  } \
  /**/

#define __HYPERDEX_KEY_CHECK \
  if ( key.status != HYPERDEX_DS_SUCCESS ) \
  { \
    ans.a_->status_ = ds_to_client_status(key.status); \
    return ans; \
  } \
  /**/

#define __HYPERDEX_ANSWER_POST_CHECK \
  if (ans.a_->op_id_ < 0) \
  { \
    lock_ = true; \
    ans.a_->m.is_completed_ = true; \
    log_error(ans); \
  } \
  else \
  { \
    ops.emplace(ans.a_->op_id_, ans); \
  } \
  /**/

#define __HYPERDEX_ANSWER_POST_CHECK_S \
  ans.a_->m.is_completed_ = true; \
  ans.a_->m.last_op_ = true; \
  if (ans.a_->op_id_ < 1) \
    log_error(ans); \
  else \
  { \
    ans.a_->m.phase_ = 2; \
    hyperdex_client_returncode status; \
    hyperdex_opid loop_id = hyperdex_client_loop(client_.get(), -1, &status); \
    if (loop_id < 1) \
    { \
      ans.a_->status_ = status; \
      log_error(ans); \
    } \
    else if (ans.a_->op_id_ != loop_id) \
    { \
      ans.a_->status_ = HYPERDEX_CLIENT_INTERNAL; \
      internal_error = " - operation id is not equivalent for stadies of sync. operation"; \
      log_error(ans); \
    } \
  } \
  /**/

#define __HYPERDEX_FN_COND_GROUP_TEMPLATE(nm1,nm2,in_container,pre_macro,post_macro) \
      client_answer nm1(space_proxy  const& space, \
                        key_proxy    const& key, \
                        in_container const& attrs) noexcept \
      { \
        pre_macro; \
        __HYPERDEX_SPACE_CHECK; \
        __HYPERDEX_KEY_CHECK; \
\
        ans.a_->op_id_  = BOOST_PP_CAT(hyperdex_client_,nm2)(client_.get(), \
                                                             space.str, \
                                                             key.key, key.key_sz, \
                                                             attrs.attrs, attrs.size, \
                                                             &ans.a_->status_); \
\
        post_macro; \
\
        return ans; \
\
      } \
\
      client_answer BOOST_PP_CAT(cond_,nm1)(space_proxy      const& space, \
                                            key_proxy        const& key, \
                                            predicates_proxy const& preds, \
                                            in_container     const& attrs) noexcept \
      { \
        pre_macro; \
        __HYPERDEX_SPACE_CHECK; \
        __HYPERDEX_KEY_CHECK; \
\
        ans.a_->op_id_  = BOOST_PP_CAT(hyperdex_client_cond_,nm2)(client_.get(), \
                                                                  space.str, \
                                                                  key.key, key.key_sz, \
                                                                  preds.attrs, preds.size, \
                                                                  attrs.attrs, attrs.size, \
                                                                  &ans.a_->status_); \
\
        post_macro; \
\
        return ans; \
\
      } \
\
      client_answer BOOST_PP_CAT(group_,nm1)(space_proxy           const& space, \
                                             predicates_proxy      const& preds, \
                                             in_container          const& attrs, \
                                             hyperdex_group_count& count) noexcept \
      { \
        pre_macro; \
        __HYPERDEX_SPACE_CHECK; \
\
        ans.a_->op_id_  = BOOST_PP_CAT(hyperdex_client_group_,nm2)(client_.get(), \
                                                                   space.str, \
                                                                   preds.attrs, preds.size, \
                                                                   attrs.attrs, attrs.size, \
                                                                   &ans.a_->status_, \
                                                                   &count); \
\
        post_macro; \
\
        return ans; \
\
      } \
      /**/

#define __HYPERDEX_FN_COND_GROUP_ATTR(nm) \
  __HYPERDEX_FN_COND_GROUP_TEMPLATE(nm,nm,in_attributes_proxy,__HYPERDEX_ANSWER_LOCK,__HYPERDEX_ANSWER_POST_CHECK) \
/**/

#define __HYPERDEX_FN_COND_GROUP_MAP(nm) \
  __HYPERDEX_FN_COND_GROUP_TEMPLATE(nm,nm,map_attributes_proxy,__HYPERDEX_ANSWER_LOCK,__HYPERDEX_ANSWER_POST_CHECK) \
/**/

#define __HYPERDEX_FN_COND_GROUP_ATTR_SYNC(nm) \
  __HYPERDEX_FN_COND_GROUP_TEMPLATE(BOOST_PP_CAT(nm,_sync),nm,\
in_attributes_proxy,__HYPERDEX_ANSWER_LOCK_S,__HYPERDEX_ANSWER_POST_CHECK_S) \
/**/

#define __HYPERDEX_FN_COND_GROUP_MAP_SYNC(nm) \
  __HYPERDEX_FN_COND_GROUP_TEMPLATE(BOOST_PP_CAT(nm,_sync),nm,\
map_attributes_proxy,__HYPERDEX_ANSWER_LOCK_S,__HYPERDEX_ANSWER_POST_CHECK_S) \
/**/

#define __HYPERDEX_FN_COND_GROUP_ATTR_D(nm) \
  __HYPERDEX_FN_COND_GROUP_ATTR(nm) \
  __HYPERDEX_FN_COND_GROUP_ATTR_SYNC(nm) \
/**/

#define __HYPERDEX_FN_COND_GROUP_MAP_D(nm) \
  __HYPERDEX_FN_COND_GROUP_MAP(nm) \
  __HYPERDEX_FN_COND_GROUP_MAP_SYNC(nm) \
/**/

#define __HYPERDEX_UXACT_ATTR(nm) \
client_answer BOOST_PP_CAT(uxact_,nm) \
  (microtransaction m, in_attributes_proxy const& attrs) noexcept \
{ \
  __HYPERDEX_ANSWER_LOCK; \
  ans.a_->op_id_ = BOOST_PP_CAT(hyperdex_client_uxact_,nm)(client_.get(), m, attrs.attrs, attrs.size); \
  __HYPERDEX_ANSWER_POST_CHECK; \
  return ans; \
} \
client_answer BOOST_PP_CAT(uxact_,BOOST_PP_CAT(nm,_sync)) \
  (microtransaction m, in_attributes_proxy const& attrs) noexcept \
{ \
  __HYPERDEX_ANSWER_LOCK_S; \
  ans.a_->op_id_ = BOOST_PP_CAT(hyperdex_client_uxact_,nm)(client_.get(), m, attrs.attrs, attrs.size); \
  __HYPERDEX_ANSWER_POST_CHECK_S; \
  return ans; \
} \
/**/

namespace hyperdex
{
  using forest::string;
  using forest::to_string;
  using forest::unique_ptr;

  inline
  hyperdex_client_returncode
  ds_to_client_status(hyperdex_ds_returncode status) noexcept
  {
    hyperdex_client_returncode res;

    switch (status)
    {
      case HYPERDEX_DS_NOMEM           : res = HYPERDEX_CLIENT_NOMEM;     break;
      case HYPERDEX_DS_MIXED_TYPES     : res = HYPERDEX_CLIENT_WRONGTYPE; break;
      case HYPERDEX_DS_WRONG_STATE     : res = HYPERDEX_CLIENT_INTERNAL;  break;
      case HYPERDEX_DS_STRING_TOO_LONG : res = HYPERDEX_CLIENT_OVERFLOW;  break;
      case HYPERDEX_DS_SUCCESS         : res = HYPERDEX_CLIENT_SUCCESS;   break;
      default                          : res = HYPERDEX_CLIENT_INTERNAL;  break;
    }

    return res;

  } // ds_to_client_status


  struct connstr_t {};
  static constexpr connstr_t const connstr{};

  using microtransaction = hyperdex_client_microtransaction*;

  template<typename, typename> class c_string_traits;
  template<typename, typename> class primitive_data_traits;

  template<template<typename, typename = void> typename   CStringTraits = c_string_traits,
           template<typename, typename = void> typename PrimitiveTraits = primitive_data_traits>
  class basic_client final
  {

      struct space_proxy
      {
        space_proxy(char const* s) noexcept: str { s } {}

        template<typename SpaceName>
        space_proxy(SpaceName&& space_name) noexcept
        : str { nullptr }
        {
          arena_vector arenas {};

          CStringTraits<std::decay_t<SpaceName>>::serialize(arenas, status, str, space_name);
        }

        char const* str;
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};

      }; // struct space_proxy

      struct key_proxy
      {
        key_proxy(char const* a, size_t sz) noexcept
        : key { a }, key_sz { sz } {}

        key_proxy(blob_pair const& bp) noexcept
        : key { bp.first }, key_sz { bp.second } {}

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Weffc++"

        template<typename KeyName,
                 typename = std::enable_if_t<!std::is_convertible<KeyName, blob_pair const&>::value>>
        key_proxy(KeyName&& key_name) noexcept
        : key { nullptr }, key_sz { 0 }
        {
          arena_vector arenas {};

          PrimitiveTraits<std::decay_t<KeyName>>::serialize(arenas, status, key, key_sz, key_name);
        }

        #pragma GCC diagnostic pop

        char const* key;
        size_t key_sz;
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};

      }; // struct key_proxy

      struct in_attributes_proxy
      {
        in_attributes_proxy(hyperdex_client_attribute const* a, size_t sz) noexcept
        : attrs { a }, size { sz } {}

        in_attributes_proxy(an_attribute const* a, size_t sz) noexcept
        : attrs { a }, size { sz } {}

        in_attributes_proxy(std::pair<hyperdex_client_attribute const*, size_t> const& p) noexcept
        : attrs { p.first }, size { p.second } {}

        in_attributes_proxy(std::pair<an_attribute const*, size_t> const& p) noexcept
        : attrs { p.first }, size { p.second } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        in_attributes_proxy(basic_attributes<Traits1, Traits2> const& a) noexcept
        : attrs { a.attrs_ }, size { a.size_ } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        in_attributes_proxy(basic_attributes<Traits1, Traits2> const&& a) noexcept = delete;

        template<size_t N>
        in_attributes_proxy(attribute_c_array<N> const& a) noexcept
        : attrs { a }, size { N } {}

        template<typename T>
        in_attributes_proxy(T&& t,
                            std::enable_if_t<is_linear_safe_attribute_container<T>::value &&
                                         !is_c_array_of_attributes<T>::value &&
                                         !is_attributes_class<T>::value
                                         >* = nullptr) noexcept
        : attrs { attribute_filter(low_address(t)) }, size { container_size(t) }
        {
          static_assert(std::is_lvalue_reference<decltype(t)>::value,
                        "rvalue reference is not allowed as an argument for in_attributes_proxy constructor");
        }

        hyperdex_client_attribute const* attrs;
        size_t size;

      }; // struct in_attributes_proxy

      struct predicates_proxy
      {
        predicates_proxy(hyperdex_client_attribute_check const* a, size_t sz) noexcept
        : attrs { a }, size { sz } {}

        predicates_proxy(std::pair<hyperdex_client_attribute_check const*, size_t> const& p) noexcept
        : attrs { p.first }, size { p.second } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        predicates_proxy(basic_predicates<Traits1, Traits2> const& a) noexcept
        : attrs { a.attrs_ }, size { a.size_ } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        predicates_proxy(basic_predicates<Traits1, Traits2>&& a) noexcept = delete;

        template<size_t N>
        predicates_proxy(predicate_c_array<N> const& a) noexcept
        : attrs { a }, size { N } {}

        template<typename T>
        predicates_proxy(T&& t,
                         std::enable_if_t<is_linear_safe_predicate_container<T>::value &&
                                         !is_c_array_of_predicates<T>::value &&
                                         !is_predicates_class<T>::value
                                         >* = nullptr) noexcept
        : attrs { low_address(t) }, size { container_size(t) }
        {
          static_assert(std::is_lvalue_reference<decltype(t)>::value,
                        "rvalue reference is not allowed as an argument for predicates_proxy constructor");
        }

        hyperdex_client_attribute_check const* attrs;
        size_t size;

      }; // struct predicates_proxy

      struct map_attributes_proxy
      {
        map_attributes_proxy(hyperdex_client_map_attribute const* a, size_t sz) noexcept
        : attrs { a }, size { sz } {}

        map_attributes_proxy(std::pair<hyperdex_client_map_attribute const*, size_t> const& p) noexcept
        : attrs { p.first }, size { p.second } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        map_attributes_proxy(basic_map_attributes<Traits1, Traits2> const& a) noexcept
        : attrs { a.attrs_ }, size { a.size_ } {}

        template<template<typename, typename = void> typename Traits1,
                 template<typename, typename = void> typename Traits2>
        map_attributes_proxy(basic_map_attributes<Traits1, Traits2>&& a) noexcept = delete;

        template<size_t N>
        map_attributes_proxy(map_attribute_c_array<N> const& a) noexcept
        : attrs { a }, size { N } {}

        template<typename T>
        map_attributes_proxy(T&& t,
                             std::enable_if_t<is_linear_safe_map_attribute_container<T>::value &&
                                         !is_c_array_of_map_attributes<T>::value &&
                                         !is_map_attributes_class<T>::value
                                         >* = nullptr) noexcept
        : attrs { low_address(t) }, size { container_size(t) }
        {
          static_assert(std::is_lvalue_reference<decltype(t)>::value,
                        "rvalue reference is not allowed as an argument for map_attributes_proxy constructor");
        }

        hyperdex_client_map_attribute const* attrs;
        size_t size;

      }; // struct map_attributes_proxy

      struct names_proxy
      {
        names_proxy(char const* const* a, size_t sz) noexcept
        : names { const_cast<char const**>(a) }, size { sz } {}

        names_proxy(std::pair<char const* const*, size_t> const& p) noexcept
        : names { p.first }, size { p.second } {}

        template<size_t N>
        names_proxy(name_c_array<N> const& a) noexcept
        : names { const_cast<char const**>(a) }, size { N } {}

        template<typename T>
        names_proxy(T&& t,
                    std::enable_if_t<is_linear_safe_name_container<T>::value &&
                                    !is_c_array_of_names<T>::value
                                    >* = nullptr) noexcept
        : names { low_address(t) }, size { container_size(t) }
        {
          static_assert(std::is_lvalue_reference<decltype(t)>::value,
                        "rvalue reference is not allowed as an argument for names_proxy constructor");
        }

        char const** names;
        size_t size;

      }; // struct names_proxy

    public:

      basic_client(string coordinator = DEFAULT_HYPERDEX_COORDINATOR,
                   uint16_t port = DEFAULT_HYPERDEX_PORT) noexcept
        :  client_ { nullptr, Deleter{} }
      {

        try
        {
          client_ = std::move(
                      unique_ptr<hyperdex_client, Deleter>{
                        hyperdex_client_create(coordinator.c_str(), port),
                        Deleter{}
                      }
                    );
        }
        catch (...)
        {
          LOG(error) << "Error creating hyperdex client\n";
        }

      } // basic_client constructor(string coordinator, uint16_t port)

      basic_client(connstr_t, string conn_str = DEFAULT_HYPERDEX_COORDINATOR) noexcept
        :  client_ { nullptr, Deleter{} }
      {
        try
        {
          client_ = std::move(
                      unique_ptr<hyperdex_client, Deleter>{
                        hyperdex_client_create_conn_str(conn_str.c_str()),
                        Deleter{}
                      }
                    );
        }
        catch (...)
        {
          LOG(error) << "Error creating hyperdex client\n";
        }

      } // basic_client constructor(string conn_str)

      basic_client(hyperdex_client* cl) noexcept
        :  client_ { cl, Deleter{} } {}

      ~basic_client() noexcept = default;

      explicit operator bool() const { return client_.get(); }

      FOREST_NONCOPYABLE_CLASS(basic_client);
      FOREST_NONMOVABLE_CLASS(basic_client);
      FOREST_NONALLOCATABLE_CLASS(basic_client);

      bool is_locked()   const { return  lock_; }
      void    unlock()         { lock_ = false; }
      void  set_lock(bool val) { lock_ =   val; }

      __HYPERDEX_FN_COND_GROUP_ATTR_D(put)

      // use field HYPERDEX_ATTRIBUTE_SECRET ("__secret") to put a secret for macaroons

      client_answer put_if_not_exist(space_proxy         const& space,
                                     key_proxy           const& key,
                                     in_attributes_proxy const& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_put_if_not_exist(client_.get(),
                                                           space.str,
                                                           key.key, key.key_sz,
                                                           attrs.attrs, attrs.size,
                                                           &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // put_if_not_exist

      client_answer put_if_not_exist_sync(space_proxy         const& space,
                                          key_proxy           const& key,
                                          in_attributes_proxy const& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_put_if_not_exist(client_.get(),
                                                           space.str,
                                                           key.key, key.key_sz,
                                                           attrs.attrs, attrs.size,
                                                           &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // put_if_not_exist_sync

      client_answer del(space_proxy const& space,
                        key_proxy   const& key) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_del(client_.get(),
                                              space.str,
                                              key.key, key.key_sz,
                                              &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // del

      client_answer del_sync(space_proxy const& space,
                             key_proxy   const& key) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_del(client_.get(),
                                              space.str,
                                              key.key, key.key_sz,
                                              &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // del_sync

      client_answer cond_del(space_proxy      const& space,
                             key_proxy        const& key,
                             predicates_proxy const& preds) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_cond_del(client_.get(),
                                                   space.str,
                                                   key.key, key.key_sz,
                                                   preds.attrs, preds.size,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // cond_del

      client_answer cond_del_sync(space_proxy      const& space,
                                  key_proxy        const& key,
                                  predicates_proxy const& preds) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_  = hyperdex_client_cond_del(client_.get(),
                                                   space.str,
                                                   key.key, key.key_sz,
                                                   preds.attrs, preds.size,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // cond_del_sync

      client_answer group_del(space_proxy      const& space,
                              predicates_proxy const& preds,
                              hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_client_group_del(client_.get(),
                                                    space.str,
                                                    preds.attrs, preds.size,
                                                    &ans.a_->status_,
                                                    &count);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // group_del

      client_answer group_del_sync(space_proxy      const& space,
                                   predicates_proxy const& preds,
                                   hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_client_group_del(client_.get(),
                                                    space.str,
                                                    preds.attrs, preds.size,
                                                    &ans.a_->status_,
                                                    &count);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // group_del_sync

      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_add)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_sub)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_mul)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_div)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_mod)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_and)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_or)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_xor)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_min)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(atomic_max)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(string_prepend)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(string_append)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(list_lpush)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(list_rpush)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(set_add)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(set_remove)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(set_intersect)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(set_union)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(document_rename)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(document_unset)

      __HYPERDEX_FN_COND_GROUP_ATTR_D(string_ltrim)
      __HYPERDEX_FN_COND_GROUP_ATTR_D(string_rtrim)

      __HYPERDEX_FN_COND_GROUP_MAP_D(map_add)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_remove)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_add)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_sub)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_mul)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_div)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_mod)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_and)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_or)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_xor)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_string_prepend)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_string_append)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_min)
      __HYPERDEX_FN_COND_GROUP_MAP_D(map_atomic_max)


      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer get(space_proxy const& space,
                        key_proxy   const& key,
                        basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_get(client_.get(),
                                             space.str,
                                             key.key, key.key_sz,
                                             &ans.a_->status_,
                                             &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // get

      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer get_sync(space_proxy const& space,
                             key_proxy   const& key,
                             basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_get(client_.get(),
                                             space.str,
                                             key.key, key.key_sz,
                                             &ans.a_->status_,
                                             &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // get_sync

      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer get_partial(space_proxy const& space,
                                key_proxy   const& key,
                                names_proxy const& names,
                                basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_get_partial(client_.get(),
                                                     space.str,
                                                     key.key, key.key_sz,
                                                     names.names, names.size,
                                                     &ans.a_->status_,
                                                     &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // get_partial

      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer get_partial_sync(space_proxy const& space,
                                     key_proxy   const& key,
                                     names_proxy const& names,
                                     basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_get_partial(client_.get(),
                                                     space.str,
                                                     key.key, key.key_sz,
                                                     names.names, names.size,
                                                     &ans.a_->status_,
                                                     &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // get_partial_sync

      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer search(space_proxy const& space,
                           predicates_proxy const& preds,
                           basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_search(client_.get(),
                                                space.str,
                                                preds.attrs, preds.size,
                                                &ans.a_->status_,
                                                &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK;

        ans.a_->m.is_search_ = true;

        return ans;

      } // search

      template<template<typename, typename = void> typename Traits1,
               template<typename, typename = void> typename Traits2>
      client_answer sorted_search(space_proxy const& space,
                                  predicates_proxy const& preds,
                                  space_proxy const& sort_by,
                                  uint64_t const& limit,
                                  int maxmin,
                                  basic_attributes<Traits1, Traits2>& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        if ( sort_by.status != HYPERDEX_DS_SUCCESS ) \
        {
          ans.a_->status_ = ds_to_client_status(sort_by.status);
          return ans;
        }

        if (attrs.size_) attrs.clear();
        attrs.destroy_f_ = true;

        ans.a_->op_id_ = hyperdex_client_sorted_search(client_.get(),
                                                       space.str,
                                                       preds.attrs, preds.size,
                                                       sort_by.str,
                                                       limit,
                                                       maxmin,
                                                       &ans.a_->status_,
                                                       &attrs.attrs_, &attrs.size_);

        __HYPERDEX_ANSWER_POST_CHECK;

        ans.a_->m.is_search_ = true;

        return ans;

      } // sorted_search

      client_answer count(space_proxy           const& space,
                          predicates_proxy      const& preds,
                          hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_ = hyperdex_client_count(client_.get(),
                                               space.str,
                                               preds.attrs, preds.size,
                                               &ans.a_->status_,
                                               &count);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // count

      client_answer count_sync(space_proxy           const& space,
                               predicates_proxy      const& preds,
                               hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_ = hyperdex_client_count(client_.get(),
                                               space.str,
                                               preds.attrs, preds.size,
                                               &ans.a_->status_,
                                               &count);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // count_sync

      // Not documented yet in hyperdex documentation:
      //===============================================

      client_answer cond_put_or_create(space_proxy         const& space,
                                       key_proxy           const& key,
                                       predicates_proxy    const& preds,
                                       in_attributes_proxy const& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_cond_put_or_create(client_.get(),
                                                            space.str,
                                                            key.key, key.key_sz,
                                                            preds.attrs, preds.size,
                                                            attrs.attrs, attrs.size,
                                                            &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // cond_put_or_create

      client_answer cond_put_or_create_sync(space_proxy         const& space,
                                            key_proxy           const& key,
                                            predicates_proxy    const& preds,
                                            in_attributes_proxy const& attrs) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_cond_put_or_create(client_.get(),
                                                            space.str,
                                                            key.key, key.key_sz,
                                                            preds.attrs, preds.size,
                                                            attrs.attrs, attrs.size,
                                                            &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // cond_put_or_create_sync

      client_answer search_describe(space_proxy const& space,
                                    predicates_proxy const& preds,
                                    char const*& description) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_ = hyperdex_client_search_describe(client_.get(),
                                                         space.str,
                                                         preds.attrs, preds.size,
                                                         &ans.a_->status_,
                                                         &description);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // search_describe

      client_answer search_describe_sync(space_proxy const& space,
                                         predicates_proxy const& preds,
                                         char const*& description) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_ = hyperdex_client_search_describe(client_.get(),
                                                         space.str,
                                                         preds.attrs, preds.size,
                                                         &ans.a_->status_,
                                                         &description);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // search_describe_sync


      // Service:
      //==================

      client_answer loop(hyperdex_timeout timeout = -1) noexcept
      {
        client_answer ans {std::nothrow};
        if (!ans.a_)
        {
          log_error(ans);
          return ans;
        }

        if (ops.empty())
        {
          if (lock_)
          {
            lock_  = false;
            seria_ = false;
            ans.a_->m.last_op_ = true;
            ans.a_->m.lock_    = true;

            return ans;
          }
          ans.a_->m.is_completed_ = true;
          ans.a_->m.last_op_ = true;
          ans.a_->m.phase_ = 2;
          internal_error = " - loop call without operation call ";
          log_error(ans);
          seria_ = false;

          return ans;
        }

        hyperdex_client_returncode status;
        hyperdex_opid op  = hyperdex_client_loop(client_.get(), timeout, &status);

        if (~op)
        {
          if (ops.find(op) != ops.end())
          {
            ans = ops.at(op);
            if (!ans.a_->m.is_search_ || ans.a_->status_ == HYPERDEX_CLIENT_SEARCHDONE) ops.erase(op);
          }
          else
          {
            op = -1;
            internal_error = " - loop call for unknown operation call ";
          }
        }
        else ans.a_->status_ = status;

        ans.a_->m.phase_ = 2;
        ans.a_->m.is_completed_ = true;

        if (status == HYPERDEX_CLIENT_NONEPENDING) ops.clear();
        if (ops.empty() && !lock_)
        {
          seria_ = false;
          ans.a_->m.last_op_ = true;
        }

        if (op < 1) log_error(ans);

        return ans;

      } // loop(hyperdex_timeout)

      bool loop_all(hyperdex_timeout timeout = -1) noexcept
      {
        client_answer ans {std::nothrow};
        if (!ans.a_)
        {
          log_error(ans);
          return false;
        }

        bool res = true;

        do
        {
          ans = loop(timeout);
          if (!ans) res = false;
        }
        while (!ans.is_last_op());

        return res;

      } // loop_all(hyperdex_timeout)

      bool seria_start() noexcept
      {
        bool res = true;

        if (!ops.empty() || seria_)
        {
          res = false;
          LOG(error) << "Try to start new async. seria, while old seria was not terminated\n";
        }

        if (lock_)
        {
          res = false;
          LOG(error) << "Try to start async. seria, while client class is locked\n";
        }

        return seria_ = res;

      } // seria_start()

      bool seria_end(hyperdex_timeout timeout = -1) noexcept
      {

        return loop_all(timeout);

      } // seria_end


      // Microtransactions:

      microtransaction uxact_init(space_proxy const& space, hyperdex_client_returncode *status = nullptr) noexcept
      {
        if ( space.status != HYPERDEX_DS_SUCCESS ) { return nullptr; }
        hyperdex_client_returncode s;
        hyperdex_client_returncode *sp = status ? status : &s;
        return hyperdex_client_uxact_init(client_.get(), space.str, sp);
      }

      client_answer uxact_commit(microtransaction m, key_proxy const& key) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_uxact_commit(client_.get(), m, key.str, key.key_sz);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // uxact_commit

      client_answer uxact_commit_sync(microtransaction m, key_proxy const& key) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_uxact_commit(client_.get(), m, key.str, key.key_sz);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // uxact_commit_sync

      client_answer uxact_group_commit(microtransaction m,
                                       predicates_proxy const& preds,
                                       hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_ = hyperdex_client_uxact_group_commit(client_.get(), m, preds.attrs, preds.size, count);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // uxact_group_commit

      client_answer uxact_group_commit_sync(microtransaction m,
                                            predicates_proxy const& preds,
                                            hyperdex_group_count& count) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_ = hyperdex_client_uxact_group_commit(client_.get(), m, preds.attrs, preds.size, count);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // uxact_group_commit_sync

      client_answer uxact_cond_commit(microtransaction m,
                                      key_proxy const& key,
                                      predicates_proxy const& preds) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_uxact_cond_commit(client_.get(), m,
                                                           key.str, key.key_sz,
                                                           preds.attrs, preds.size);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // uxact_cond_commit

      client_answer uxact_cond_commit_sync(microtransaction m,
                                           key_proxy const& key,
                                           predicates_proxy const& preds) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_KEY_CHECK;

        ans.a_->op_id_ = hyperdex_client_uxact_cond_commit(client_.get(), m,
                                                           key.str, key.key_sz,
                                                           preds.attrs, preds.size);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // uxact_cond_commit_sync

      __HYPERDEX_UXACT_ATTR(put)
      __HYPERDEX_UXACT_ATTR(atomic_add)
      __HYPERDEX_UXACT_ATTR(atomic_sub)
      __HYPERDEX_UXACT_ATTR(atomic_mul)
      __HYPERDEX_UXACT_ATTR(atomic_div)
      __HYPERDEX_UXACT_ATTR(atomic_and)
      __HYPERDEX_UXACT_ATTR(atomic_or)
      __HYPERDEX_UXACT_ATTR(string_prepend)
      __HYPERDEX_UXACT_ATTR(string_append)
      __HYPERDEX_UXACT_ATTR(list_lpush)
      __HYPERDEX_UXACT_ATTR(list_rpush)
      __HYPERDEX_UXACT_ATTR(document_rename)
      __HYPERDEX_UXACT_ATTR(document_unset)
      __HYPERDEX_UXACT_ATTR(atomic_min)
      __HYPERDEX_UXACT_ATTR(atomic_max)
      __HYPERDEX_UXACT_ATTR(string_ltrim)
      __HYPERDEX_UXACT_ATTR(string_rtrim)


      // Not documented yet in hyperdex documentation:
      //================================================

      hyperdatatype attribute_type(space_proxy const& space,
                                   space_proxy const& name,
                                   hyperdex_client_returncode& status) noexcept
      {
        if (space.status != HYPERDEX_DS_SUCCESS) { status = space.status; return HYPERDATATYPE_STRING; }
        if ( name.status != HYPERDEX_DS_SUCCESS) { status =  name.status; return HYPERDATATYPE_STRING; }
        return hyperdex_client_attribute_type(client_.get(), space.str, name.str, &status);
      }

      bool poll_fd()          noexcept { return ~hyperdex_client_poll_fd(client_.get())       ; }
      bool block(hyperdex_timeout timeout)
                              noexcept { return ~hyperdex_client_block(client_.get(), timeout); }
      string error_message()  noexcept { return  hyperdex_client_error_message(client_.get()) ; }
      string error_location() noexcept { return  hyperdex_client_error_location(client_.get()); }
      string error_code_to_string(hyperdex_client_returncode code)
                              noexcept { return  hyperdex_client_returncode_to_string(code)   ; }

      // Auth context:
      //================================================
      void clear_auth_context() noexcept { hyperdex_client_clear_auth_context(client_.get()); }

      void set_auth_context(char const* const* macaroons, size_t macaroons_sz) noexcept
          { hyperdex_client_set_auth_context(client_.get(), const_cast<char const**>(macaroons), macaroons_sz); }

      template<size_t N>
      void set_auth_context(name_c_array<N> const& macaroons) noexcept
          { hyperdex_client_set_auth_context(client_.get(), const_cast<char const**>(macaroons), N); }

      template<typename T>
      void set_auth_context(T&& macaroons,
                            std::enable_if_t<is_linear_safe_name_container<T>::value &&
                                    !is_c_array_of_names<T>::value
                                    >* = nullptr) noexcept
      {
        static_assert(std::is_lvalue_reference<decltype(macaroons)>::value,
                        "rvalue reference is not allowed as an argument for set_auth_context");
        hyperdex_client_set_auth_context(client_.get(), low_address(macaroons), container_size(macaroons));
      }


      // use field HYPERDEX_ATTRIBUTE_SECRET ("__secret") to put a secret for macaroons

      // Z-index extension
      //=================================================
      #ifdef HYPERDEX_USE_Z_INDEX

        //TODO place z extension methods realization here

      #endif // HYPERDEX_USE_Z_INDEX

    private:

      struct Deleter
      {
        void operator () (hyperdex_client* cl) const noexcept
        {
          if (!cl) return;
          try
          {
            hyperdex_client_destroy(cl);
          }
          catch (...)
          {
            LOG(error) << "Error destroying hyperdex client\n";
          }
        };
      };

      void log_error(client_answer const& ans)
      {
        string s;

        if (!ans.a_) s = "Not enough memory." + internal_error;
        else if (!ans.a_->m.phase_) s = "Operation was not started!\n";
        else
        {
          s = static_cast<string>("Phase ") + to_string(ans.a_->m.phase_) + " " +
              hyperdex_client_returncode_to_string(ans.a_->status_) +
              " [ " + hyperdex_client_error_message(client_.get()) + internal_error +
              " ] at " + hyperdex_client_error_location(client_.get());
        }

        internal_error = "";

        LOG(error) << s;
      }

      std::unordered_map<hyperdex_opid, client_answer> ops {};

      string internal_error {};

      bool lock_  = false;
      bool seria_ = false;

      unique_ptr<hyperdex_client, Deleter> client_;

  }; // class basic_client

  using client = basic_client<>;

} // namespace hyperdex

#undef __HYPERDEX_ANSWER_LOCK
#undef __HYPERDEX_ANSWER_LOCK_S
#undef __HYPERDEX_SPACE_CHECK
#undef __HYPERDEX_KEY_CHECK
#undef __HYPERDEX_ANSWER_POST_CHECK
#undef __HYPERDEX_ANSWER_POST_CHECK_S
#undef __HYPERDEX_FN_COND_GROUP_TEMPLATE
#undef __HYPERDEX_FN_COND_GROUP_ATTR
#undef __HYPERDEX_FN_COND_GROUP_MAP
#undef __HYPERDEX_FN_COND_GROUP_ATTR_SYNC
#undef __HYPERDEX_FN_COND_GROUP_MAP_SYNC
#undef __HYPERDEX_FN_COND_GROUP_ATTR_D
#undef __HYPERDEX_FN_COND_GROUP_MAP_D
#undef __HYPERDEX_UXACT_ATTR

#endif // HYPERDEX_CLIENT_HPP_INCLUDED
