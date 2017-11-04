#ifndef HYPERDEX_CLIENT_ANSWER_HPP_INCLUDED
#define HYPERDEX_CLIENT_ANSWER_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: client_answer.hpp
// First created: 2015-09-10-13.05
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
#include <hyperdex_cpp/log.hpp>

// forest
#include <forest/shared_ptr.hpp>

// C++
#include <iostream>

namespace hyperdex
{

  class client_answer final
  {

      template<template<typename, typename = void> typename,
               template<typename, typename = void> typename>
      friend class basic_client;

      friend std::ostream& operator << (std::ostream& lhs,  client_answer const& ans)
      {
        using std::endl;

        std::ios::fmtflags f(lhs.flags());

        lhs << std::boolalpha << std::dec << endl << "Client answer:" << endl << endl;
        if (!ans.a_) lhs << "\tEmpty" << endl << endl;
        else
        {
          lhs << "\tSummary: " << (ans.is_ok() ? "OK" : ans.is_critical_error() ? "critical error" : "error")
              << (ans.is_connection_error() || ans.is_database_structure_error() || ans.is_program_error() ? " [" : "")
              << (ans.is_connection_error() ? " (connection error) " : "")
              << (ans.is_database_structure_error() ? " (database error)" : "")
              << (ans.is_program_error() ? " (program error) " : "")
              << (ans.is_connection_error() || ans.is_database_structure_error() || ans.is_program_error() ? "]" : "")
              << endl;
          lhs << "\top_id: "  << ans.op_id()  << (!~ans.op_id() ? " (fatal error)" : "") << endl;
          lhs << "\tstatus: " << ans.status() << " (" << (int64_t)ans.status() << ")" <<
                 ((ans.is_ok() && ans.status() != HYPERDEX_CLIENT_SUCCESS) ? " (not actual)" : "" ) << endl;
          lhs << "\tphase: "  << (int)ans.phase() << " ("
              << (!ans.phase() ? "Nop" : ans.phase() == 1 ? "Query" : "Loop") << ") " << endl;
          lhs << "\tcompleted: " << ans.is_completed() << endl;
          lhs << "\tis last operation? :" << ans.is_last_op() << endl;
          lhs << "\tis search? :" << ans.is_search() << endl;
          lhs << "\tlocked: "    << ans.is_locked() << endl << endl;
        }

        lhs.flags(f);

        return lhs;
      }

    public:

      explicit operator bool () const noexcept { return is_ok(); }

      bool is_allocated() const noexcept { return a_.get(); }

      hyperdex_opid op_id() const noexcept { return a_ ? a_->op_id_ : -1; }

      hyperdex_client_returncode status() const noexcept { return a_ ? a_->status_ : HYPERDEX_CLIENT_NOMEM; }

      bool is_completed() const noexcept { return a_ ? a_->m.is_completed_ : false; }

      bool is_search() const noexcept { return a_ ? a_->m.is_search_ : false; }

      // Only loop procedure sets
      bool is_last_op() const noexcept { return a_ ? a_->m.last_op_ : true; }

      uint_fast8_t phase() const noexcept { return a_ ? a_->m.phase_ : 0; }

      bool is_locked() const noexcept { return a_ ? a_->m.lock_ : true; }

      bool was_operations() const noexcept
      {
        if (!a_ || (a_->m.phase_ == 2 && a_->status_ ==  HYPERDEX_CLIENT_NONEPENDING)) return false;
        return a_->m.phase_;
      }

      bool is_ok() const noexcept
      {
        if (a_)
        {
          if      (a_->m.phase_ == 1) return a_->op_id_ > 0;
          else if (a_->m.phase_ == 2) return a_->op_id_ > 0 && a_->status_ == HYPERDEX_CLIENT_SUCCESS;
        }
        return  false;
      }

      bool is_error() const noexcept
      {
        return !is_ok();
      }

      bool is_critical_error() const noexcept
      {
        if (a_)
        {
          if      (a_->m.phase_ == 1) return a_->op_id_ < 1;
          else if (a_->m.phase_ == 2) return a_->op_id_ < 1 || a_->status_ >= HYPERDEX_CLIENT_UNKNOWNSPACE;
        }
        return  true;
      }

      bool is_connection_error() const noexcept
      {
        if (a_)
        {
          if (is_read_only() || is_coord_fail() || is_server_error() || is_reconfigure() || is_timeout()
              || is_interrupted() || is_cluster_jump() || is_offline())
            return true;
        }
        return  false;
      }

      bool is_database_structure_error() const noexcept
      {
        if (a_)
        {
          if (is_not_found() || is_cmp_fail() || is_unknown_space() || is_unknown_attr()
              || is_dupe_attr() || is_dont_use_key() || is_wrong_type())
            return true;
        }
        return  false;
      }

      bool is_program_error() const noexcept
      {
        if (!a_
            || is_not_found() || is_cmp_fail() || is_unknown_space() || is_poll_failed()
            || is_over_flow() || is_unknown_attr() || is_dupe_attr()
            || is_none_pending() || is_dont_use_key() || is_wrong_type()
            || is_no_mem() || is_internal() || is_exception() || is_garbage()
           )
          return true;
        return  false;
      }

      bool is_not_found() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_NOTFOUND;
        }
        return  false;
      }

      bool search_done() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_SEARCHDONE;
        }
        return  false;
      }

      bool is_cmp_fail() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_CMPFAIL;
        }
        return  false;
      }

      bool is_read_only() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_READONLY;
        }
        return  false;
      }

      bool is_unknown_space() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_UNKNOWNSPACE;
        }
        return  false;
      }

      bool is_coord_fail() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_COORDFAIL;
        }
        return  false;
      }

      bool is_server_error() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_SERVERERROR;
        }
        return  false;
      }

      bool is_poll_failed() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_POLLFAILED;
        }
        return  false;
      }

      bool is_over_flow() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_OVERFLOW;
        }
        return  false;
      }

      bool is_reconfigure() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_RECONFIGURE;
        }
        return  false;
      }

      bool is_timeout() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_TIMEOUT;
        }
        return  false;
      }

      bool is_unknown_attr() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_UNKNOWNATTR;
        }
        return  false;
      }

      bool is_dupe_attr() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_DUPEATTR;
        }
        return  false;
      }

      bool is_none_pending() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_NONEPENDING;
        }
        return  false;
      }

      bool is_dont_use_key() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_DONTUSEKEY;
        }
        return  false;
      }

      bool is_wrong_type() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_WRONGTYPE;
        }
        return  false;
      }

      bool is_no_mem() const noexcept
      {
        if (!a_ || (a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
          return a_->status_ == HYPERDEX_CLIENT_NOMEM;
        return  false;
      }

      bool is_interrupted() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_INTERRUPTED;
        }
        return  false;
      }

      bool is_cluster_jump() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_CLUSTER_JUMP;
        }
        return  false;
      }

      bool is_offline() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_OFFLINE;
        }
        return  false;
      }

      bool is_unauthorized() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_UNAUTHORIZED;
        }
        return  false;
      }

      bool is_internal() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_INTERNAL;
        }
        return  false;
      }

      bool is_exception() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_EXCEPTION;
        }
        return  false;
      }

      bool is_garbage() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_CLIENT_GARBAGE;
        }
        return  false;
      }

    private:

      client_answer()
      : a_ { new __client_answer }
      {}

      client_answer(std::nothrow_t)
      : a_ { new(std::nothrow) __client_answer }
      {}

      class __client_answer
      {
        public:
          hyperdex_opid op_id_ { -1 };
          hyperdex_client_returncode status_ = { HYPERDEX_CLIENT_INTERNAL };

          struct
          {
            unsigned phase_    :2;
            bool is_completed_ :1;
            bool last_op_      :1;
            bool is_search_    :1;
            bool lock_         :1;
          } m { 0, false, false, false, false };

      }; // __client_answer

      forest::shared_ptr<__client_answer> a_;

  }; // class client_answer

} // namespace hyperdex

#endif // HYPERDEX_CLIENT_ANSWER_HPP_INCLUDED
