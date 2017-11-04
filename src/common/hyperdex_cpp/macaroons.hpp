#ifndef HYPERDEX_MACAROONS_HPP_INCLUDED
#define HYPERDEX_MACAROONS_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: macaroons.hpp
// First created: 2015-10-16-10.50
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifdef HYPERDEX_USE_GOOGLE_MACAROONS

#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>

#include <macaroons/macaroons.h>

// forest
#include <forest/unique_ptr.hpp>
#include <forest/string.hpp>
#include <forest/vector.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/log.hpp>

// C++
#include <iostream>
#include <utility>
#include <type_traits>
#include <unordered_map>

namespace hyperdex
{
  using forest::string;

  template<typename, typename> class c_string_traits;

  template<template<typename, typename> typename>
  class basic_google_macaroon_verifier;

  template<template<typename, typename = void> typename   CStringTraits = c_string_traits>
  class basic_google_macaroon final
  {
      friend class basic_google_macaroon_verifier<CStringTraits>;

      template<template<typename, typename> typename T>
      friend class basic_google_macaroon;

      template<template<typename, typename> typename T1,
               template<typename, typename> typename T2>
      friend bool operator == (basic_google_macaroon<T1> const& m1, basic_google_macaroon<T2> const& m2) noexcept;

      template<template<typename, typename> typename T>
      friend bool operator == (basic_google_macaroon<T> const& m1, macaroon const* m2) noexcept;

      template<template<typename, typename> typename T>
      friend bool operator == (macaroon const* m1, basic_google_macaroon<T> const& m2) noexcept;

      template<template<typename, typename> typename T>
      friend bool operator != (basic_google_macaroon const& m1, basic_google_macaroon<T> const& m2) noexcept
      {
        return !(m1 == m2);
      }

      friend bool operator != (basic_google_macaroon const& m1, macaroon const* m2) noexcept
      {
        return !(m1 == m2);
      }

      friend bool operator != (macaroon const* m1, basic_google_macaroon const& m2) noexcept
      {
        return !(m1 == m2);
      }

      struct Deleter
      {
        void operator () (macaroon* m) const noexcept
        {
          if (!m) return;
          try { macaroon_destroy(m); }
          catch (...) { LOG(error) << "Error destroying macaroon\n"; }
        };
      }; // Deleter

      forest::unique_ptr<macaroon, Deleter> m_ {nullptr, Deleter{}};

      void init_(char unsigned const* key, size_t key_sz,
                 char unsigned const* location, size_t location_sz,
                 char unsigned const* id, size_t id_sz) noexcept
      {
        try
        {
          macaroon_returncode err;
          m_.reset(macaroon_create(location, location_sz, key, key_sz, id, id_sz, &err));
          if (!m_) log_error(err);
        }
        catch (...) { LOG(error) << "Error creating google macaroon\n"; }

      } // init_

    public:

      // constructors:

      basic_google_macaroon() noexcept = default;

      basic_google_macaroon(macaroon const* m) noexcept
      {
        if (!m) { log_error(MACAROON_INVALID); return; }
        try
        {
          macaroon_returncode err;
          m_.reset(macaroon_copy(m, &err));
          if (!m_) log_error(err);
        }
        catch (...) { LOG(error) << "Error creating google macaroon from c macaroon\n"; }
      }

      basic_google_macaroon(macaroon* m) noexcept
      {
        if (!m) { log_error(MACAROON_INVALID); return; }
        try
        {
          macaroon_returncode err;
          m_.reset(macaroon_copy(m, &err));
          if (!m_) log_error(err);
        }
        catch (...) { LOG(error) << "Error creating google macaroon from c macaroon\n"; }
      }

      basic_google_macaroon(basic_google_macaroon const& m) noexcept
      {
        if (!m) { log_error(MACAROON_INVALID); return; }
        try
        {
          macaroon_returncode err;
          m_.reset(macaroon_copy(m.m_.get(), &err));
          if (!m_) log_error(err);
        }
        catch (...) { LOG(error) << "Error copying google macaroon\n"; }
      }

      basic_google_macaroon(basic_google_macaroon&& m) noexcept
      {
        m_ = std::move(m.m_);
      }

      template<template<typename, typename> typename T>
      basic_google_macaroon(basic_google_macaroon<T> const& m) noexcept
      {
        if (!m) { log_error(MACAROON_INVALID); return; }
        try
        {
          macaroon_returncode err;
          m_.reset(macaroon_copy(m.m_.get(), &err));
          if (!m_) log_error(err);
        }
        catch (...) { LOG(error) << "Error copying google macaroon\n"; }
      }

      template<template<typename, typename> typename T>
      basic_google_macaroon(basic_google_macaroon<T>&& m) noexcept
      {
        m_ = std::move(m.m_);
      }

      basic_google_macaroon(char unsigned const* key, size_t key_sz,
                            char unsigned const* location, size_t location_sz,
                            char unsigned const* id, size_t id_sz) noexcept
      {
        init_(key, key_sz, location, location_sz, id, id_sz);
      }

      basic_google_macaroon(char unsigned const* key, size_t key_sz,
                            char unsigned const* location = nullptr, size_t location_sz = 0) noexcept
      {
        init_(key, key_sz, location, location_sz, nullptr, 0);
      }

      basic_google_macaroon(char unsigned const* key, size_t key_sz) noexcept
      {
        init_(key, key_sz, nullptr, 0, nullptr, 0);
      }

      basic_google_macaroon(char const* key, size_t key_sz,
                            char const* location, size_t location_sz,
                            char const* id, size_t id_sz) noexcept
      {
        init_(reinterpret_cast<char unsigned const*>(key), key_sz,
              reinterpret_cast<char unsigned const*>(location), location_sz,
              reinterpret_cast<char unsigned const*>(id), id_sz);
      }

      basic_google_macaroon(char const* key, size_t key_sz,
                            char const* location, size_t location_sz) noexcept
      {
        init_(reinterpret_cast<char unsigned const*>(key), key_sz,
              reinterpret_cast<char unsigned const*>(location), location_sz,
              nullptr, 0);
      }

      basic_google_macaroon(char const* key, size_t key_sz) noexcept
      {
        init_(reinterpret_cast<char unsigned const*>(key), key_sz,
              nullptr, 0, nullptr, 0);
      }

      template<typename Key>
      basic_google_macaroon(Key const& key,
                            char const* location, size_t location_sz,
                            char const* id, size_t id_sz) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(location), location_sz,
                reinterpret_cast<char unsigned const*>(id), id_sz);
      }

      template<typename Key>
      basic_google_macaroon(Key const& key,
                            char const* location, size_t location_sz) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(location), location_sz,
                nullptr, 0);
      }

      template<typename Key>
      basic_google_macaroon(Key const& key) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                nullptr, 0, nullptr, 0);
      }

      template<typename Loc>
      basic_google_macaroon(char const* key, size_t key_sz,
                            Loc  const& loc,
                            char const* id, size_t id_sz) noexcept
      {
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          init_(reinterpret_cast<char unsigned const*>(key), key_sz,
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                reinterpret_cast<char unsigned const*>(id), id_sz);
      }

      template<typename Loc>
      basic_google_macaroon(char const* key, size_t key_sz,
                            Loc  const& loc) noexcept
      {
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          init_(reinterpret_cast<char unsigned const*>(key), key_sz,
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                nullptr, 0);
      }

      template<typename Id>
      basic_google_macaroon(char const* key, size_t key_sz,
                            char const* location, size_t location_sz,
                            Id   const& id) noexcept
      {
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          init_(reinterpret_cast<char unsigned const*>(key), key_sz,
                reinterpret_cast<char unsigned const*>(location), location_sz,
                reinterpret_cast<char unsigned const*>(i), strlen(i));
      }

      template<typename Key, typename Loc>
      basic_google_macaroon(Key const& key,
                            Loc const& loc,
                            char const* id, size_t id_sz) noexcept
      {
        char const* k;
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                reinterpret_cast<char unsigned const*>(id), id_sz);
      }

      template<typename Key, typename Loc>
      basic_google_macaroon(Key const& key,
                            Loc const& loc) noexcept
      {
        char const* k;
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                nullptr, 0);
      }

      template<typename Key, typename Loc, typename Id>
      basic_google_macaroon(Key const& key,
                            Loc const& loc,
                            Id  const& id) noexcept
      {
        char const* k;
        char const* l;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                reinterpret_cast<char unsigned const*>(i), strlen(i));
      }

      template<typename Loc, typename Id>
      basic_google_macaroon(char const* key, size_t key_sz,
                            Loc const& loc,
                            Id  const& id) noexcept
      {
        char const* l;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          init_(reinterpret_cast<char unsigned const*>(key), key_sz,
                reinterpret_cast<char unsigned const*>(l), strlen(l),
                reinterpret_cast<char unsigned const*>(i), strlen(i));
      }

      template<typename Key, typename Id>
      basic_google_macaroon(Key const& key,
                            char const* location, size_t location_sz,
                            Id  const& id) noexcept
      {
        char const* k;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          init_(reinterpret_cast<char unsigned const*>(k), strlen(k),
                reinterpret_cast<char unsigned const*>(location), location_sz,
                reinterpret_cast<char unsigned const*>(i), strlen(i));
      }

      // assignment operators
      basic_google_macaroon& operator=(macaroon const* m) noexcept
      {
        if (*this != m)
        {
          if (!m) { log_error(MACAROON_INVALID); return; }
          try
          {
            macaroon_returncode err;
            m_.reset(macaroon_copy(m, &err));
            if (!m_) log_error(err);
          }
          catch (...) { LOG(error) << "Error assigning google macaroon\n"; }
        }
        return *this;
      }

      basic_google_macaroon& operator=(basic_google_macaroon const& m) noexcept
      {
        if (*this != m)
        {
          if (!m)
          {
            log_error(MACAROON_INVALID);
            m_.reset(nullptr);
          }
          else
          {
            try
            {
              macaroon_returncode err;
              m_.reset(macaroon_copy(m.m_.get(), &err));
              if (!m_) log_error(err);
            }
            catch (...) { LOG(error) << "Error assigning google macaroon\n"; }
          }
        }
        return *this;
      }

      basic_google_macaroon& operator=(basic_google_macaroon&& m) noexcept
      {
        m_ = std::move(m.m_);
        return *this;
      }

      template<template<typename, typename> typename T>
      basic_google_macaroon& operator=(basic_google_macaroon<T> const& m) noexcept
      {
        if (*this != m)
        {
          if (!m)
          {
            log_error(MACAROON_INVALID);
            m_.reset(nullptr);
          }
          else
          {
            try
            {
              macaroon_returncode err;
              m_.reset(macaroon_copy(m.m_.get(), &err));
              if (!m_) log_error(err);
            }
            catch (...) { LOG(error) << "Error assigning google macaroon\n"; }
          }
        }
        return *this;
      }

      template<template<typename, typename> typename T>
      basic_google_macaroon& operator=(basic_google_macaroon<T>&& m) noexcept
      {
        m_ = std::move(m.m_);
        return *this;
      }

      // check:
      explicit operator bool () const noexcept { return m_.get(); }

      // comparision:
      bool is_same(macaroon* m) const noexcept
      {
        return m_.get() && m && !macaroon_cmp(m_.get(), m);
      }

      template<template<typename, typename> typename T>
      bool is_same(basic_google_macaroon<T> const& m) const noexcept
      {
        return m_.get() == m;
      }

      // serialization:
      string serialize() const noexcept
      {
        bool res = false;
        string res_str{};
        macaroon_returncode err = MACAROON_INVALID;
        size_t data_sz;
        char*  data;

        if (m_)
        {
          err = MACAROON_OUT_OF_MEMORY;
          data_sz = macaroon_serialize_size_hint(m_.get());
          if (data_sz)
          {
            data = new(std::nothrow) char[data_sz];
            if (data)
            {
              if (~macaroon_serialize(m_.get(), data, data_sz, &err))
              {
                res = true;
                res_str = string { data, data_sz };
              }
              delete [] data;
            }
          } // if (data_sz)
        } // if (m_)

        if (!res) log_error(err);
        return res_str;

      } // serialize()

      // serialization to json:
      string serialize_json() const noexcept
      {
        bool res = false;
        string res_str{};
        macaroon_returncode err = MACAROON_INVALID;
        size_t data_sz;
        char*  data;

        if (m_)
        {
          err = MACAROON_OUT_OF_MEMORY;
          data_sz = macaroon_serialize_json_size_hint(m_.get());
          if (data_sz)
          {
            data = new(std::nothrow) char[data_sz];
            if (data)
            {
              if (~macaroon_serialize_json(m_.get(), data, data_sz, &err))
              {
                res = true;
                res_str = string { data, data_sz };
              }
              delete [] data;
            }
          } // if (data_sz)
        } // if (m_)

        if (!res) log_error(err);
        return res_str;

      } // serialize_json

      // inspect: human readable representation, for debug only
      string inspect() const noexcept
      {
        bool res = false;
        string res_str{};
        macaroon_returncode err = MACAROON_INVALID;
        size_t data_sz;
        char*  data;

        if (m_)
        {
          err = MACAROON_OUT_OF_MEMORY;
          data_sz = macaroon_inspect_size_hint(m_.get());
          if (data_sz)
          {
            data = new(std::nothrow) char[data_sz];
            if (data)
            {
              if (~macaroon_inspect(m_.get(), data, data_sz, &err))
              {
                res = true;
                res_str = string { data, data_sz };
              }
              delete [] data;
            }
          } // if (data_sz)
        } // if (m_)

        if (!res) log_error(err);
        return res_str;

      } // inspect()

      // deserialization
      bool deserialize(char const* buf) noexcept
      {
        macaroon_returncode err;
        m_.reset(macaroon_deserialize(buf, &err));
        if (!m_) log_error(err);
        return m_.get();

      } // deserialize(char const* buf)

      template<typename Val>
      bool deserialize(Val const& val) noexcept
      {
        char const* buf;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (!~CStringTraits<std::decay_t<Val>>::serialize(arenas, status, buf, val))
        {
          log_ds_status(status);
          return false;
        }
        return deserialize(buf);

      } // deserialize(char const* buf)

      // deserialization from json
      bool deserialize_json(char const* buf, size_t sz) noexcept
      {
        macaroon_returncode err;
        m_.reset(macaroon_deserialize_json(buf, sz, &err));
        if (!m_) log_error(err);
        return m_.get();

      } // deserialize_json(char const* buf)

      bool deserialize_json(char const* buf) noexcept
      {
        macaroon_returncode err;
        m_.reset(macaroon_deserialize_json(buf, strlen(buf), &err));
        if (!m_) log_error(err);
        return m_.get();
      }

      bool deserialize_json(string const& buf) noexcept
      {
        macaroon_returncode err;
        m_.reset(macaroon_deserialize_json(buf.c_str(), buf.size(), &err));
        if (!m_) log_error(err);
        return m_.get();
      }

      template<typename Val>
      bool deserialize_json(Val const& val) noexcept
      {
        char const* buf;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (!~CStringTraits<std::decay_t<Val>>::serialize(arenas, status, buf, val))
        {
          log_ds_status(status);
          return false;
        }
        return deserialize_json(buf);
      }

      // Properties:
      string location() const noexcept
      {
        if (!m_) return {};
        unsigned char const* l = nullptr;
        size_t sz = 0;
        macaroon_location(m_.get(), &l, &sz);
        return { reinterpret_cast<char const*>(l), sz };
      }

      string identifier() const noexcept
      {
        if (!m_) return {};
        unsigned char const* i = nullptr;
        size_t sz = 0;
        macaroon_identifier(m_.get(), &i, &sz);
        return { reinterpret_cast<char const*>(i), sz };
      }

      string signature() const noexcept
      {
        if (!m_) return {};
        unsigned char const* s = nullptr;
        size_t sz = 0;
        macaroon_signature(m_.get(), &s, &sz);
        return { reinterpret_cast<char const*>(s), sz };
      }

      // Check a macaroon's integrity.
      // This routine is used internally, and is exposed as part of the public API for
      // use in assert() statements.
      bool validate() const noexcept { return !macaroon_validate(m_); }

      // Prepare the macaroon for a request
      template<template<typename, typename> typename T>
      basic_google_macaroon prepare_for_request(basic_google_macaroon<T> const& d) const noexcept
      {
        if (!m_ || !d.m_) { log_error(MACAROON_INVALID); return nullptr; }
        macaroon_returncode err;
        basic_google_macaroon res { macaroon_prepare_for_request(m_.get(), d.m_.get(), &err) };
        if (!res.m_) log_error(err);
        return res;
      }

      // Add a new first party caveat, and return a new macaroon.
      // - predicate/predicate_sz is the caveat to be added to the macaroon
      // Returns a new macaroon, leaving the original untouched.
      basic_google_macaroon add_first_party_caveat(unsigned char const* pred, size_t sz) const noexcept
      {
        if (!m_) { log_error(MACAROON_INVALID); return nullptr; }
        macaroon_returncode err;
        basic_google_macaroon res { macaroon_add_first_party_caveat(m_.get(), pred, sz, &err) };
        if (!res.m_) log_error(err);
        return res;
      }

      basic_google_macaroon add_first_party_caveat(char const* pred, size_t sz) const noexcept
      {
        return add_first_party_caveat(reinterpret_cast<unsigned char const*>(pred), sz);
      }

      basic_google_macaroon add_first_party_caveat(char const* pred) const noexcept
      {
        return add_first_party_caveat(reinterpret_cast<unsigned char const*>(pred), strlen(pred));
      }

      template<typename Val>
      basic_google_macaroon add_first_party_caveat(Val const& pred) const noexcept
      {
        char const* buf;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (!~CStringTraits<std::decay_t<Val>>::serialize(arenas, status, buf, pred))
        {
          log_ds_status(status);
          return nullptr;
        }
        return add_first_party_caveat(reinterpret_cast<unsigned char const*>(buf), strlen(buf));
      }

      // Add a new third party caveat, and return a new macaroon.
      // - key/keys_sz is a secret shared shared between this macaroon and the third
      // party.  Guard it as carefully as you do the key for macaroon_create.
      // - location/location_sz is a hint to the third party's location
      // - id/id_sz is the identifier for this macaroon.  If presented to the third
      // party, the third party must be able to recall the secret and predicate to
      // check.
      // A good way to generate this ID is to generate N random bytes as the key,
      // and encrypt these bytes and the caveat.  Pass the bytes and N as the key,
      // and pass the ciphertext as the ID.
      // Returns a new macaroon, leaving the original untouched.
      basic_google_macaroon add_third_party_caveat(char unsigned const* key, size_t key_sz,
                                                   char unsigned const* location, size_t location_sz,
                                                   char unsigned const* id, size_t id_sz) noexcept
      {
        macaroon_returncode err;
        basic_google_macaroon res{ macaroon_add_third_party_caveat(m_.get(),
                                                                   location, location_sz,
                                                                   key, key_sz,
                                                                   id, id_sz, &err) };
        if (!res.m_) log_error(err);
        return res;
      }

      basic_google_macaroon add_third_party_caveat(char unsigned const* key, size_t key_sz,
                                                   char unsigned const* location, size_t location_sz) noexcept
      {
        return add_third_party_caveat(key, key_sz, location, location_sz, nullptr, 0);
      }

      basic_google_macaroon add_third_party_caveat(char unsigned const* key, size_t key_sz) noexcept
      {
        return add_third_party_caveat(key, key_sz, nullptr, 0, nullptr, 0);
      }

      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz,
                                                   char const* location, size_t location_sz,
                                                   char const* id, size_t id_sz) noexcept
      {
        return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                      reinterpret_cast<char unsigned const*>(location), location_sz,
                                      reinterpret_cast<char unsigned const*>(id), id_sz);
      }

      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz,
                                                   char const* location, size_t location_sz) noexcept
      {
        return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                      reinterpret_cast<char unsigned const*>(location), location_sz,
                                      nullptr, 0);
      }

      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz) noexcept
      {
        return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                      nullptr, 0, nullptr, 0);
      }

      template<typename Key>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   char const* location, size_t location_sz,
                                                   char const* id, size_t id_sz) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(location), location_sz,
                                        reinterpret_cast<char unsigned const*>(id), id_sz);
        return nullptr;
      }

      template<typename Key>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   char const* location, size_t location_sz) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(location), location_sz,
                                        nullptr, 0);
        return nullptr;
      }

      template<typename Key>
      basic_google_macaroon add_third_party_caveat(Key const& key) noexcept
      {
        char const* k;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        nullptr, 0, nullptr, 0);
        return nullptr;
      }

      template<typename Loc>
      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz,
                                                   Loc  const& loc,
                                                   char const* id, size_t id_sz) noexcept
      {
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        reinterpret_cast<char unsigned const*>(id), id_sz);
        return nullptr;
      }

      template<typename Loc>
      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz,
                                                   Loc  const& loc) noexcept
      {
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        nullptr, 0);
        return nullptr;
      }

      template<typename Id>
      basic_google_macaroon add_third_party_caveat(char const* key, size_t key_sz,
                                                   char const* location, size_t location_sz,
                                                   Id   const& id) noexcept
      {
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                        reinterpret_cast<char unsigned const*>(location), location_sz,
                                        reinterpret_cast<char unsigned const*>(i), strlen(i));
        return nullptr;
      }

      template<typename Key, typename Loc>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   Loc const& loc,
                                                   char const* id, size_t id_sz) noexcept
      {
        char const* k;
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        reinterpret_cast<char unsigned const*>(id), id_sz);
        return nullptr;
      }

      template<typename Key, typename Loc>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   Loc const& loc) noexcept
      {
        char const* k;
        char const* l;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        nullptr, 0);
        return nullptr;
      }

      template<typename Key, typename Loc, typename Id>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   Loc const& loc,
                                                   Id  const& id) noexcept
      {
        char const* k;
        char const* l;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        reinterpret_cast<char unsigned const*>(i), strlen(i));
        return nullptr;
      }

      template<typename Loc, typename Id>
      basic_google_macaroon  add_third_party_caveat(char const* key, size_t key_sz,
                                                    Loc const& loc,
                                                    Id  const& id) noexcept
      {
        char const* l;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Loc>>::serialize(arenas, status, l, loc) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(key), key_sz,
                                        reinterpret_cast<char unsigned const*>(l), strlen(l),
                                        reinterpret_cast<char unsigned const*>(i), strlen(i));
        return nullptr;
      }

      template<typename Key, typename Id>
      basic_google_macaroon add_third_party_caveat(Key const& key,
                                                   char const* location, size_t location_sz,
                                                   Id  const& id) noexcept
      {
        char const* k;
        char const* i;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Key>>::serialize(arenas, status, k, key) &&
            ~CStringTraits<std::decay_t<Id>>::serialize(arenas, status, i, id))
          return add_third_party_caveat(reinterpret_cast<char unsigned const*>(k), strlen(k),
                                        reinterpret_cast<char unsigned const*>(location), location_sz,
                                        reinterpret_cast<char unsigned const*>(i), strlen(i));
        return nullptr;
      }

      // Where are the third-parties that give discharge macaroons?
      size_t num_third_party_caveats() const noexcept
      {
        return macaroon_num_third_party_caveats(m_.get());
      }

      std::unordered_map<string, string> third_party_caveats() const
      {
        if (!m_) { log_error(MACAROON_INVALID); return {}; }
        std::unordered_map<string, string> map {};
        unsigned char const* location = nullptr;
        size_t location_sz = 0;
        unsigned char const* identifier = nullptr;
        size_t identifier_sz = 0;
        size_t const num = macaroon_num_third_party_caveats(m_.get());
        for (size_t i = 0; i < num; ++i)
        {
          if (~macaroon_third_party_caveat(m_.get(), i,
                                           &location,   &location_sz,
                                           &identifier, &identifier_sz))
          {
            map.emplace(std::piecewise_construct,
                        std::forward_as_tuple(reinterpret_cast<char const*>(location),   location_sz),
                        std::forward_as_tuple(reinterpret_cast<char const*>(identifier), identifier_sz));
          }
          else
          {
            log_error(MACAROON_INVALID);
            return {};
          }
        }
        return map;
      }

    private:

      void log_error(macaroon_returncode err) const
      {
        LOG(error) << "Google macaroon error " << err << " (" << (int64_t)err << ")";
      }

  }; // basic_google_macaroon

  template<template<typename, typename> typename T1,
           template<typename, typename> typename T2>
  inline bool operator == (basic_google_macaroon<T1> const& m1, basic_google_macaroon<T2> const& m2) noexcept
  {
    return m1.m_.get() && m2.m_.get() && !macaroon_cmp(m1.m_.get(), m2.m_.get());
  }

  template<template<typename, typename> typename T>
  inline bool operator == (macaroon const* m1, basic_google_macaroon<T> const& m2) noexcept
  {
    return m1 && m2.m_.get() && !macaroon_cmp(m1, m2.m_.get());;
  }

  template<template<typename, typename> typename T>
  inline bool operator == (basic_google_macaroon<T> const& m1, macaroon const* m2) noexcept
  {
    return m1.m_.get() && m2 && !macaroon_cmp(m1.m_.get(), m2);;
  }

  using google_macaroon = basic_google_macaroon<>;

  template<template<typename, typename = void> typename CStringTraits = c_string_traits>
  using basic_google_macaroon_vector = forest::vector<basic_google_macaroon<CStringTraits>>;

  using google_macaroon_vector = forest::vector<google_macaroon>;

  static_assert(sizeof(google_macaroon) == sizeof(macaroon*),
                 "class google_macaroon must be just a wrapper for struct macaroon*. Their sizes must be equal.");

  inline int general_cb(void* f, unsigned char const* pred, size_t pred_sz) noexcept
  {
    using bfun = bool(*)(char const*, size_t);
    try { if (reinterpret_cast<bfun>(f)(reinterpret_cast<char const*>(pred), pred_sz)) return 0; }
    catch(...) {}
    return -1;
  }

  template<template<typename, typename = void> typename   CStringTraits = c_string_traits>
  class basic_google_macaroon_verifier final
  {
      using bfun = bool(*)(char const*, size_t);

      void log_error(macaroon_returncode err) const
      {
        LOG(error) << "Google macaroon error " << err << " (" << (int64_t)err << ")";
      }

      struct Deleter
      {
        void operator () (macaroon_verifier* v) const noexcept
        {
          if (!v) return;
          try { macaroon_verifier_destroy(v); }
          catch (...) { LOG(error) << "Error destroying macaroon verifier\n"; }
        };
      }; // Deleter

      forest::unique_ptr<macaroon_verifier, Deleter> v_ {macaroon_verifier_create(), Deleter{}};

    public:

      explicit operator bool () const noexcept { return v_.get(); }

      bool satisfy_exact(unsigned char const* pred, size_t sz) const noexcept
      {
        macaroon_returncode err;
        bool res = ~macaroon_verifier_satisfy_exact(v_.get(), pred, sz, &err);
        if (!res) log_error(err);
        return res;
      }

      bool satisfy_exact(char const* pred, size_t sz) const noexcept
      {
        return satisfy_exact(reinterpret_cast<unsigned char const*>(pred), sz);
      }

      bool satisfy_exact(char const* pred) const noexcept
      {
        return satisfy_exact(reinterpret_cast<unsigned char const*>(pred), strlen(pred));
      }

      bool satisfy_exact(string const& pred) const noexcept
      {
        return satisfy_exact(pred.c_str(), pred.size());
      }

      template<typename Val>
      bool satisfy_exact(Val const& val) const noexcept
      {
        char const* pred;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Val>>::serialize(arenas, status, pred, val))
          return satisfy_exact(reinterpret_cast<unsigned char const*>(pred), strlen(pred));
        return false;
      }

      bool satisfy_general(bfun f) const noexcept
      {
        macaroon_returncode err;
        bool res = ~macaroon_verifier_satisfy_general(v_.get(),
                                                      general_cb,
                                                      (void*)f, &err);
        if (!res) log_error(err);
        return res;
      }

      bool verify(basic_google_macaroon<CStringTraits> const& m,
                  unsigned char const* key, size_t key_sz,
                  basic_google_macaroon_vector<CStringTraits> const& ms
                    = basic_google_macaroon_vector<CStringTraits>{}) const noexcept
      {
        if (!m.m_) { log_error(MACAROON_INVALID); return false; }
        macaroon_returncode err;
        size_t const sz = ms.size();
        for (size_t i = 0; i < sz; ++i)
          if (!ms[i].m_) { log_error(MACAROON_INVALID); return false; }
        bool res = ~macaroon_verify(v_.get(), m.m_.get(), key, key_sz,
                                    const_cast<macaroon**>(reinterpret_cast<macaroon* const*>(ms.data())),
                                    ms.size(), &err);
        if (!res) log_error(err);
        return res;
      }

      bool verify(basic_google_macaroon<CStringTraits> const& m,
                  char const* key, size_t key_sz,
                  basic_google_macaroon_vector<CStringTraits> const& ms
                    = basic_google_macaroon_vector<CStringTraits>{}) const noexcept
      {
        return verify(m, reinterpret_cast<unsigned char const*>(key), key_sz, ms);
      }

      bool verify(basic_google_macaroon<CStringTraits> const& m,
                  char const* key,
                  basic_google_macaroon_vector<CStringTraits> const& ms
                    = basic_google_macaroon_vector<CStringTraits>{}) const noexcept
      {
        return verify(m, reinterpret_cast<unsigned char const*>(key), strlen(key), ms);
      }

      bool verify(basic_google_macaroon<CStringTraits> const& m,
                  string const& key,
                  basic_google_macaroon_vector<CStringTraits> const& ms
                    = basic_google_macaroon_vector<CStringTraits>{}) const noexcept
      {
        return verify(m, reinterpret_cast<unsigned char const*>(key.c_str()), key.size(), ms);
      }

      template<typename Val>
      bool verify(basic_google_macaroon<CStringTraits> const& m,
                  Val const& val,
                  basic_google_macaroon_vector<CStringTraits> const& ms
                    = basic_google_macaroon_vector<CStringTraits>{}) const noexcept
      {
        char const* key;
        arena_vector arenas {};
        hyperdex_ds_returncode status {HYPERDEX_DS_SUCCESS};
        if (~CStringTraits<std::decay_t<Val>>::serialize(arenas, status, key, val))
          return verify(m, reinterpret_cast<unsigned char const*>(key), strlen(key), ms);
        return false;
      }

  }; // class basic_google_macaroon_verifier

  using google_macaroon_verifier = basic_google_macaroon_verifier<>;

  template<template<typename, typename> typename T>
  inline bool serialize_google_macaroon_vector(name_vector& nmv,
                                               string_vector& sv,
                                               basic_google_macaroon_vector<T> const& mv) noexcept
  {
    try
    {
      for (auto const& m: mv)
      {
        forest::string s = m.serialize();
        sv.push_back(s);
        nmv.push_back(sv.back().c_str());
      }
    }
    catch(...) { return false; }

    return true;
  }

} // namespace hyperdex

#endif // HYPERDEX_USE_GOOGLE_MACAROONS

#endif // HYPERDEX_MACAROONS_HPP_INCLUDED
