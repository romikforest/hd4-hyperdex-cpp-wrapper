#ifndef HYPERDEX_ADMIN_ANSWER_HPP_INCLUDED
#define HYPERDEX_ADMIN_ANSWER_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: admin_answer.hpp
// First created: 2015-09-10-13.09
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

// hyperdex_cpp
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>

// forest
#include <forest/shared_ptr.hpp>

// C++
#include <iostream>

namespace hyperdex
{

  class admin_answer final
  {

      template<template<typename, typename = void> typename>
      friend class basic_admin;

      friend std::ostream& operator << (std::ostream& lhs,  admin_answer const& ans)
      {
        std::ios::fmtflags f(lhs.flags());

        lhs << std::boolalpha << std::dec << std::endl << "Admin answer:" << std::endl << std::endl;
        if (!ans.a_) lhs << "\tEmpty" << std::endl << std::endl;
        else
        {
          lhs << "\tSummary: " << (ans.is_ok() ? "OK" : "error")
              << (ans.is_connection_error() || ans.is_database_structure_error() || ans.is_program_error() ? " [" : "")
              << (ans.is_connection_error() ? " (connection error) " : "")
              << (ans.is_database_structure_error() ? " (database error)" : "")
              << (ans.is_program_error() ? " (program error) " : "")
              << (ans.is_connection_error() || ans.is_database_structure_error() || ans.is_program_error() ? "]" : "")
              << std::endl;
          lhs << "\top_id: "  << ans.op_id()  << (!~ans.op_id() ? " (fatal error)" : "") << std::endl;
          lhs << "\tstatus: " << ans.status() << " (" << (int64_t)ans.status() << ")" <<
            ((ans.is_ok() && ans.status() != HYPERDEX_ADMIN_SUCCESS) ? " (not actual)" : "" ) << std::endl;
          lhs << "\tphase: "  << (int)ans.phase() << " ("
              << (!ans.phase() ? "Nop" : ans.phase() == 1 ? "Query" : "Loop") << ") " << std::endl;
          lhs << "\tcompleted: " << ans.is_completed() << std::endl;
          lhs << "\tis last operation? :" << ans.is_last_op() << std::endl;
          lhs << "\tlocked: "    << ans.is_locked() << std::endl << std::endl;
        }

        lhs.flags(f);

        return lhs;
      }

    public:

      explicit operator bool () const noexcept { return is_ok(); }

      bool is_allocated() const noexcept { return a_.get(); }

      hyperdex_opid op_id() const noexcept { return a_ ? a_->op_id_ : -1; }

      hyperdex_admin_returncode status() const noexcept { return a_ ? a_->status_ : HYPERDEX_ADMIN_NOMEM; }

      bool is_completed() const noexcept { return a_ ? a_->m.is_completed_ : false; }

      // Only loop procedure sets
      bool is_last_op() const noexcept { return a_ ? a_->m.last_op_ : true; }

      uint_fast8_t phase() const noexcept { return a_ ? a_->m.phase_ : 0; }

      bool is_locked() const noexcept { return a_ ? a_->m.lock_ : true; }

      bool was_operations() const noexcept
      {
        if (!a_ || (a_->m.phase_ == 2 && a_->status_ ==  HYPERDEX_ADMIN_NONEPENDING)) return false;
        return a_->m.phase_;
      }

      bool is_ok() const noexcept
      {
        if (a_)
        {
          if      (a_->m.phase_ == 1) return a_->op_id_ > 0;
          else if (a_->m.phase_ == 2) return a_->op_id_ > 0 && a_->status_ == HYPERDEX_ADMIN_SUCCESS;
        }
        return  false;
      }

      bool is_error() const noexcept
      {
        return !is_ok();
      }

      bool is_connection_error() const noexcept
      {
        if (a_)
        {
          if (is_coord_fail() || is_server_error() || is_timeout()
              || is_interrupted() || is_local_error())
            return true;
        }
        return  false;
      }

      bool is_database_structure_error() const noexcept
      {
        if (a_)
        {
          if (is_not_found() || is_bad_space() || is_duplicate())
            return true;
        }
        return  false;
      }

      bool is_program_error() const noexcept
      {
        if (!a_
            || is_not_found() || is_bad_space() || is_poll_failed()
            || is_duplicate() || is_none_pending() || is_no_mem()
            || is_internal() || is_exception() || is_garbage()
           )
          return true;
        return  false;
      }

      bool is_no_mem() const noexcept
      {
        if (!a_ || (a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
          return a_->status_ == HYPERDEX_ADMIN_NOMEM;
        return  false;
      }

      bool is_none_pending() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_NONEPENDING;
        }
        return  false;
      }

      bool is_poll_failed() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_POLLFAILED;
        }
        return  false;
      }

      bool is_timeout() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_TIMEOUT;
        }
        return  false;
      }

      bool is_interrupted() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_INTERRUPTED;
        }
        return  false;
      }

      bool is_server_error() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_SERVERERROR;
        }
        return  false;
      }

      bool is_coord_fail() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_COORDFAIL;
        }
        return  false;
      }

      bool is_bad_space() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_BADSPACE;
        }
        return  false;
      }

      bool is_duplicate() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_DUPLICATE;
        }
        return  false;
      }

      bool is_not_found() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_NOTFOUND;
        }
        return  false;
      }

      bool is_local_error() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_LOCALERROR;
        }
        return  false;
      }

      bool is_internal() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_INTERNAL;
        }
        return  false;
      }

      bool is_exception() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_EXCEPTION;
        }
        return  false;
      }

      bool is_garbage() const noexcept
      {
        if (a_)
        {
          if ((a_->m.phase_ == 1 && a_->op_id_ < 1) || a_->m.phase_ == 2)
            return a_->status_ == HYPERDEX_ADMIN_GARBAGE;
        }
        return  false;
      }

    private:

      admin_answer()
      : a_ { new __admin_answer }
      {}

      admin_answer(std::nothrow_t)
      : a_ { new(std::nothrow) __admin_answer }
      {}

      class __admin_answer
      {
        public:
          hyperdex_opid op_id_ { -1 };
          hyperdex_admin_returncode status_ { HYPERDEX_ADMIN_INTERNAL };

          struct
          {
            unsigned phase_    :2;
            bool is_completed_ :1;
            bool last_op_      :1;
            bool lock_         :1;
          } m { 0, false, false, false };
      };

      forest::shared_ptr<__admin_answer> a_;

  }; // class admin_answer

} // namespace hyperdex


#endif // HYPERDEX_ADMIN_ANSWER_HPP_INCLUDED
