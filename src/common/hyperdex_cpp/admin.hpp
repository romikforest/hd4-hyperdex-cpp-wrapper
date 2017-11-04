#ifndef HYPERDEX_ADMIN_HPP_INCLUDED
#define HYPERDEX_ADMIN_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: admin.hpp
// First created: 2015-09-10-10.14
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
#include <hyperdex_cpp/admin_answer.hpp>
#include <hyperdex_cpp/log.hpp>

// forest
#include <forest/unique_ptr.hpp>
#include <forest/tools.hpp>

// boost
#include <boost/preprocessor/cat.hpp>

// C++
#include <utility>
#include <unordered_map>

#define __HYPERDEX_ANSWER_LOCK \
  admin_answer ans {std::nothrow}; \
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
  admin_answer ans {std::nothrow}; \
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
  if(~pc_op) \
  { \
    ans.a_->m.is_completed_ = true; \
    ans.a_->m.last_op_ = true; \
    ans.a_->m.lock_ = lock_; \
    internal_error = " - sync. operation while perf counters in proccess"; \
    log_error(ans); \
    return ans; \
  } \
  /**/

#define __HYPERDEX_SPACE_CHECK \
  if ( space.status != HYPERDEX_DS_SUCCESS ) \
  { \
    ans.a_->status_ = ds_to_admin_status(space.status); \
    return ans; \
  } \
  /**/

#define __HYPERDEX_DESCRIPTION_CHECK \
  if ( description.status != HYPERDEX_DS_SUCCESS ) \
  { \
    ans.a_->status_ = ds_to_admin_status(description.status); \
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
    hyperdex_admin_returncode status; \
    hyperdex_opid loop_id = hyperdex_admin_loop(admin_.get(), -1, &status); \
    if (loop_id < 1) \
    { \
      ans.a_->status_ = status; \
      log_error(ans); \
    } \
    else if (ans.a_->op_id_ != loop_id) \
    { \
      ans.a_->status_ = HYPERDEX_ADMIN_INTERNAL; \
      internal_error = " - operation id is not equivalent for stadies of sync. operation"; \
      log_error(ans); \
    } \
  } \
  /**/

namespace hyperdex
{
  using forest::string;
  using forest::to_string;
  using forest::unique_ptr;

  template <typename, typename> class c_string_traits;

  inline
  hyperdex_admin_returncode
  ds_to_admin_status(hyperdex_ds_returncode status) noexcept
  {
    hyperdex_admin_returncode res;

    switch (status)
    {
      case HYPERDEX_DS_NOMEM           : res = HYPERDEX_ADMIN_NOMEM   ;  break;
      case HYPERDEX_DS_MIXED_TYPES     : res = HYPERDEX_ADMIN_INTERNAL;  break;
      case HYPERDEX_DS_WRONG_STATE     : res = HYPERDEX_ADMIN_INTERNAL;  break;
      case HYPERDEX_DS_STRING_TOO_LONG : res = HYPERDEX_ADMIN_INTERNAL;  break;
      case HYPERDEX_DS_SUCCESS         : res = HYPERDEX_ADMIN_SUCCESS ;  break;
      default                          : res = HYPERDEX_ADMIN_INTERNAL;  break;
    }

    return res;

  } // ds_to_admin_status

  template<template<typename, typename = void> typename   CStringTraits = c_string_traits>
  class basic_admin final
  {

      struct c_string_proxy
      {
        c_string_proxy(char const* s) noexcept: str { s } {}

        template<typename CString>
        c_string_proxy(CString&& s) noexcept
        : str { nullptr }
        {
          arena_vector arenas {};

          CStringTraits<std::decay_t<CString>>::serialize(arenas, status, str, s);
        }

        char const* str;
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};

      }; // struct c_string_proxy

    public:

      basic_admin(string coordinator = DEFAULT_HYPERDEX_COORDINATOR,
                  uint16_t port = DEFAULT_HYPERDEX_PORT) noexcept
        :  admin_ { nullptr, Deleter{} }
      {

        try
        {
          admin_ = std::move(
                      unique_ptr<hyperdex_admin, Deleter>{
                        hyperdex_admin_create(coordinator.c_str(), port),
                        Deleter{}
                      }
                    );
        }
        catch (...)
        {
          LOG(error) << "Error creating hyperdex admin\n";
        }

      } // basic_admin constructor(string coordinator, uint16_t port)

      basic_admin(hyperdex_admin* cl) noexcept
        :  admin_ { cl, Deleter{} } {}

      ~basic_admin() noexcept = default;

      explicit operator bool() const { return admin_.get(); }

      FOREST_NONCOPYABLE_CLASS(basic_admin);
      FOREST_NONMOVABLE_CLASS(basic_admin);
      FOREST_NONALLOCATABLE_CLASS(basic_admin);

      bool is_locked()   const { return  lock_; }
      void    unlock()         { lock_ = false; }
      void  set_lock(bool val) { lock_ =   val; }

      admin_answer read_only(bool ro) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_read_only(admin_.get(),
                                                   ro,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // read_only

      admin_answer read_only_sync(bool ro) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_read_only(admin_.get(),
                                                   ro,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // read_only_sync

      admin_answer wait_until_stable() noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_wait_until_stable(admin_.get(), &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // wait_until_stable

      admin_answer wait_until_stable_sync() noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_wait_until_stable(admin_.get(), &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // wait_until_stable_sync

      admin_answer fault_tolerance(c_string_proxy const& space, uint64_t ft) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_fault_tolerance(admin_.get(),
                                                         space.str,
                                                         ft,
                                                         &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // fault_tolerance

      admin_answer fault_tolerance_sync(c_string_proxy const& space, uint64_t ft) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_fault_tolerance(admin_.get(),
                                                         space.str,
                                                         ft,
                                                         &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // fault_tolerance_sync

      bool validate_space(c_string_proxy const& description) noexcept
      {
        hyperdex_admin_returncode status;
        return ~hyperdex_admin_validate_space(admin_.get(),
                                              description.str,
                                              &status);

      } // validate_space

      admin_answer add_space(c_string_proxy const& description) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_DESCRIPTION_CHECK;

        ans.a_->op_id_  = hyperdex_admin_add_space(admin_.get(),
                                                   description.str,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // add_space

      admin_answer add_space_sync(c_string_proxy const& description) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_DESCRIPTION_CHECK;

        ans.a_->op_id_  = hyperdex_admin_add_space(admin_.get(),
                                                   description.str,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // add_space_sync

      admin_answer rm_space(c_string_proxy const& space) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_rm_space(admin_.get(),
                                                  space.str,
                                                  &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // rm_space

      admin_answer rm_space_sync(c_string_proxy const& space) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_rm_space(admin_.get(),
                                                  space.str,
                                                  &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // rm_space_sync

      admin_answer mv_space(c_string_proxy const& source,
                            c_string_proxy const& target) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        if ( source.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(source.status);
          return ans;
        }

        if ( target.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(target.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_mv_space(admin_.get(),
                                                  source.str, target.str,
                                                  &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // mv_space

      admin_answer mv_space_sync(c_string_proxy const& source,
                                 c_string_proxy const& target) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        if ( source.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(source.status);
          return ans;
        }

        if ( target.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(target.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_mv_space(admin_.get(),
                                                  source.str, target.str,
                                                  &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // mv_space_sync

      admin_answer list_spaces(char const*& spaces) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_list_spaces(admin_.get(),
                                                     &ans.a_->status_,
                                                     &spaces);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // list_spaces

      admin_answer list_spaces_sync(char const*& spaces) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_list_spaces(admin_.get(),
                                                     &ans.a_->status_,
                                                     &spaces);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // list_spaces_sync

      admin_answer list_indices(c_string_proxy const& space,
                                char const*& indices) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_list_indices(admin_.get(),
                                                      space.str,
                                                      &ans.a_->status_,
                                                      &indices);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // list_indices

      admin_answer list_indices_sync(c_string_proxy const& space,
                                     char const*& indices) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_list_indices(admin_.get(),
                                                      space.str,
                                                      &ans.a_->status_,
                                                      &indices);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // list_indices_sync

      admin_answer list_subspaces(c_string_proxy const& space,
                                  char const*& subspaces) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_list_subspaces(admin_.get(),
                                                        space.str,
                                                        &ans.a_->status_,
                                                        &subspaces);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // list_subspaces

      admin_answer list_subspaces_sync(c_string_proxy const& space,
                                       char const*& subspaces) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        ans.a_->op_id_  = hyperdex_admin_list_subspaces(admin_.get(),
                                                        space.str,
                                                        &ans.a_->status_,
                                                        &subspaces);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // list_subspaces_sync

      admin_answer add_index(c_string_proxy const& space,
                             c_string_proxy const& attribute) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;
        __HYPERDEX_SPACE_CHECK;

        if ( attribute.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(attribute.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_add_index(admin_.get(),
                                                   space.str,
                                                   attribute.str,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // add_index

      admin_answer add_index_sync(c_string_proxy const& space,
                                  c_string_proxy const& attribute) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;
        __HYPERDEX_SPACE_CHECK;

        if ( attribute.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(attribute.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_add_index(admin_.get(),
                                                   space.str,
                                                   attribute.str,
                                                   &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // add_index_sync

      admin_answer rm_index(uint64_t idxid) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_rm_index(admin_.get(),
                                                  idxid,
                                                  &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // rm_index

      admin_answer rm_index_sync(uint64_t idxid) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_ = hyperdex_admin_rm_index(admin_.get(),
                                                 idxid,
                                                 &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // rm_index_sync

      admin_answer server_register(uint64_t token, c_string_proxy const& address) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        if ( address.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(address.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_server_register(admin_.get(),
                                                         token,
                                                         address.str,
                                                         &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // server_register

      admin_answer server_register_sync(uint64_t token, c_string_proxy const& address) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        if ( address.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(address.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_server_register(admin_.get(),
                                                         token,
                                                         address.str,
                                                         &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // server_register_sync

      admin_answer server_online(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_server_online(admin_.get(),
                                                       token,
                                                       &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // server_online

      admin_answer server_online_sync(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_server_online(admin_.get(),
                                                       token,
                                                       &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // server_online_sync

      admin_answer server_offline(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_server_offline(admin_.get(),
                                                        token,
                                                        &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // server_offline

      admin_answer server_offline_sync(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_server_offline(admin_.get(),
                                                        token,
                                                        &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // server_offline_sync

      admin_answer server_forget(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_server_forget(admin_.get(),
                                                       token,
                                                       &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // server_forget

      admin_answer server_forget_sync(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_server_forget(admin_.get(),
                                                       token,
                                                       &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // server_forget_sync

      admin_answer server_kill(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_server_kill(admin_.get(),
                                                     token,
                                                     &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // server_kill

      admin_answer server_kill_sync(uint64_t token) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_server_kill(admin_.get(),
                                                     token,
                                                     &ans.a_->status_);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // server_kill_sync

      admin_answer backup(c_string_proxy const& backup, char const*& backups) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        if ( backup.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(backup.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_backup(admin_.get(),
                                                backup.str,
                                                &ans.a_->status_,
                                                &backups);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // backup

      admin_answer backup_sync(c_string_proxy const& backup, char const*& backups) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        if ( backup.status != HYPERDEX_DS_SUCCESS )
        {
          ans.a_->status_ = ds_to_admin_status(backup.status);
          return ans;
        }

        ans.a_->op_id_  = hyperdex_admin_backup(admin_.get(),
                                                backup.str,
                                                &ans.a_->status_,
                                                &backups);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // backup_sync

      admin_answer enable_perf_counters(hyperdex_admin_perf_counter& pc) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_enable_perf_counters(admin_.get(),
                                                              &ans.a_->status_,
                                                              &pc);

        if (~ans.a_->op_id_) pc_op = ans.a_->op_id_;

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // enable_perf_counters

      void disable_perf_counters() noexcept
      {

        hyperdex_admin_disable_perf_counters(admin_.get());
        if (~pc_op) ops.erase(pc_op);
        pc_op = -1;

      } // disable_perf_counters

      // Not documented yet:

      admin_answer dump_config(char const*& config) noexcept
      {
        __HYPERDEX_ANSWER_LOCK;

        ans.a_->op_id_  = hyperdex_admin_dump_config(admin_.get(),
                                                     &ans.a_->status_,
                                                     &config);

        __HYPERDEX_ANSWER_POST_CHECK;

        return ans;

      } // dump_config

      admin_answer dump_config_sync(char const*& config) noexcept
      {
        __HYPERDEX_ANSWER_LOCK_S;

        ans.a_->op_id_  = hyperdex_admin_dump_config(admin_.get(),
                                                     &ans.a_->status_,
                                                     &config);

        __HYPERDEX_ANSWER_POST_CHECK_S;

        return ans;

      } // dump_config_sync

      bool raw_backup(c_string_proxy const& host, uint16_t port,
                      c_string_proxy const& name) noexcept
      {
        if ( host.status != HYPERDEX_DS_SUCCESS || name.status != HYPERDEX_DS_SUCCESS) return false;
        hyperdex_admin_returncode status;
        return ~hyperdex_admin_raw_backup(host.str, port, name.str, &status);
      }

      admin_answer loop(hyperdex_timeout timeout = -1) noexcept
      {
        admin_answer ans { std::nothrow };
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

        hyperdex_admin_returncode status;
        hyperdex_opid op  = hyperdex_admin_loop(admin_.get(), timeout, &status);

        if (~op)
        {
          if (ops.find(op) != ops.end())
          {
            ans = ops.at(op);
            if (op != pc_op) ops.erase(op);
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

        if (status == HYPERDEX_ADMIN_NONEPENDING) ops.clear();
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
        admin_answer ans {std::nothrow};
        if (!ans.a_)
        {
          log_error(ans);
          return false;
        }

        bool res = !~pc_op;
        if (!res)
        {
          LOG(error) << "Can't do loop_all while performing counters enabled.";
          return res;
        }

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
          LOG(error) << "Try to start async. seria, while admin class is locked\n";
        }

        return seria_ = res;

      } // seria_start()

      bool seria_end(hyperdex_timeout timeout = -1) noexcept
      {

        return loop_all(timeout);

      } // seria_end

      string error_message()  noexcept { return  hyperdex_admin_error_message(admin_.get()) ; }
      string error_location() noexcept { return  hyperdex_admin_error_location(admin_.get()); }
      string error_code_to_string(hyperdex_admin_returncode code)
                              noexcept { return  hyperdex_admin_returncode_to_string(code)   ; }

    private:

      struct Deleter
      {
        void operator () (hyperdex_admin* cl) const noexcept
        {
          if (!cl) return;
          try
          {
            hyperdex_admin_destroy(cl);
          }
          catch (...)
          {
            LOG(error) << "Error destroying hyperdex admin\n";
          }
        };
      };

      void log_error(admin_answer const& ans)
      {
        string s;

        if (!ans.a_) s = "Not enough memory." + internal_error;
        else if (!ans.a_->m.phase_) s = "Operation was not started!\n";
        else
        {
          s = static_cast<string>("Phase ") + to_string(ans.a_->m.phase_) + " " +
              hyperdex_admin_returncode_to_string(ans.a_->status_) +
              " [ " + hyperdex_admin_error_message(admin_.get()) + internal_error +
              " ] at " + hyperdex_admin_error_location(admin_.get());
        }

        internal_error = "";

        LOG(error) << s;
      }

      std::unordered_map<hyperdex_opid, admin_answer> ops {};
      hyperdex_opid pc_op {-1};

      string internal_error {};

      bool lock_  = false;
      bool seria_ = false;

      unique_ptr<hyperdex_admin, Deleter> admin_;

  }; // class basic_admin

  using admin = basic_admin<>;

} // namespace hyperdex

#undef __HYPERDEX_ANSWER_LOCK
#undef __HYPERDEX_ANSWER_LOCK_S
#undef __HYPERDEX_SPACE_CHECK
#undef __HYPERDEX_DESCRIPTION_CHECK
#undef __HYPERDEX_ANSWER_POST_CHECK
#undef __HYPERDEX_ANSWER_POST_CHECK_S

#endif // HYPERDEX_ADMIN_HPP_INCLUDED
