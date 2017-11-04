#ifndef HYPERDEX_LOG_HPP_INCLUDED
#define HYPERDEX_LOG_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: log.hpp
// First created: 2015-10-18-07.50
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

// forest
#include <forest/log.hpp>
#include <forest/string.hpp>

// C++
#include <iostream>
#include <unordered_map> // need standard unordered_map for log_ds_status because not all
  // unordered map implementations understand brace initialization lists and have 'at' member function

// hyperdex
#include <hyperdex/admin.h>
#include <hyperdex/client.h>
#include <hyperdex/datastructures.h>
#include <hyperdex/hyperspace_builder.h>

namespace hyperdex
{

  inline void log_malformed_list() noexcept
  {
    try { LOG(error) << "Application is trying to iterate throw malformed list."; }
    catch(...) {}
  }

  inline void log_malformed_set() noexcept
  {
    try { LOG(error) << "Application is trying to iterate throw malformed set."; }
    catch(...) {}
  }

  inline void log_malformed_map() noexcept
  {
    try { LOG(error) << "Application is trying to iterate throw malformed map."; }
    catch(...) {}
  }

  inline void log_ds_status(hyperdex_ds_returncode status) noexcept
  {
    static const std::unordered_map<int, forest::string> status_messages
      {
      { HYPERDEX_DS_NOMEM, "Attribute was not serialized. Not enougth memory. (HYPERDEX_DS_NOMEM)" },
      { HYPERDEX_DS_MIXED_TYPES, "Attribute was not serialized. Mixed types of complex data field. (HYPERDEX_DS_MIXED_TYPES)" },
      { HYPERDEX_DS_WRONG_STATE, "Attribute was not serialized. Wrong state. (HYPERDEX_DS_WRONG_STATE)" },
      { HYPERDEX_DS_STRING_TOO_LONG, "Attribute was not serialized. Data string is too long. (HYPERDEX_DS_STRING_TOO_LONG)" }
    };

    if (status == HYPERDEX_DS_SUCCESS)
      try { LOG(info) << "Attribute was serialized successfully (HYPERDEX_DS_SUCCESS)"; }
      catch(...) {}
    else
    {
      try { LOG(error) << status_messages.at(status); }
      catch(...) { LOG(error) << "Unknown hyperdex serialization status"; }
    }

  } // void log_ds_status(hyperdex_ds_returncode status)

} // namespace hyperdex

std::ostream& operator << (std::ostream& lhs,  hyperdex_admin_returncode rhs);
std::ostream& operator << (std::ostream& lhs, hyperdex_client_returncode rhs);

namespace std
{
  template<>
  struct hash<hyperspace_returncode>
  {
    using result_type   = typename hash<int>::result_type;
    using argument_type = hyperspace_returncode;

    result_type operator()(argument_type arg) const
    {
      return ::std::hash<int>{}(arg);
    }
  };

} // namespace std

inline std::ostream& operator << (std::ostream& lhs, hyperspace_returncode rhs)
{
  static const std::unordered_map<hyperspace_returncode, forest::string> code_string
  {
    { HYPERSPACE_SUCCESS, "HYPERSPACE_SUCCESS" },
    { HYPERSPACE_INVALID_NAME, "HYPERSPACE_INVALID_NAME" },
    { HYPERSPACE_INVALID_TYPE, "HYPERSPACE_INVALID_TYPE" },
    { HYPERSPACE_DUPLICATE, "HYPERSPACE_DUPLICATE" },
    { HYPERSPACE_IS_KEY, "HYPERSPACE_IS_KEY" },
    { HYPERSPACE_UNKNOWN_ATTR, "HYPERSPACE_UNKNOWN_ATTR" },
    { HYPERSPACE_NO_SUBSPACE, "HYPERSPACE_NO_SUBSPACE" },
    { HYPERSPACE_OUT_OF_BOUNDS, "HYPERSPACE_OUT_OF_BOUNDS" },
    { HYPERSPACE_UNINDEXABLE, "HYPERSPACE_UNINDEXABLE" },
    { HYPERSPACE_GARBAGE, "HYPERSPACE_GARBAGE" }
  };

  try { lhs << code_string.at(rhs); }
  catch(...) {}

  return lhs;
}

#ifdef HYPERDEX_USE_GOOGLE_MACAROONS

#include <macaroons/macaroons.h>

std::ostream& operator << (std::ostream& lhs,  macaroon_returncode rhs) noexcept
{
  static const std::unordered_map<int, forest::string> messages {
      { MACAROON_SUCCESS, "MACAROON_SUCCESS" },
      { MACAROON_OUT_OF_MEMORY, "MACAROON_OUT_OF_MEMORY" },
      { MACAROON_HASH_FAILED, "MACAROON_HASH_FAILED" },
      { MACAROON_INVALID, "MACAROON_INVALID" },
      { MACAROON_TOO_MANY_CAVEATS, "MACAROON_TOO_MANY_CAVEATS" },
      { MACAROON_CYCLE, "MACAROON_CYCLE" },
      { MACAROON_BUF_TOO_SMALL, "MACAROON_BUF_TOO_SMALL" },
      { MACAROON_NOT_AUTHORIZED, "MACAROON_NOT_AUTHORIZED" },
      { MACAROON_NO_JSON_SUPPORT, "MACAROON_NO_JSON_SUPPORT" },
    };

    try { lhs << messages.at(rhs); }
    catch(...) { lhs << "MACAROON_UNKNOWN"; }

    return lhs;

} // std::ostream& operator << (std::ostream& lhs,  macaroon_returncode rhs)

#endif // HYPERDEX_USE_GOOGLE_MACAROONS

#endif // HYPERDEX_LOG_HPP_INCLUDED
