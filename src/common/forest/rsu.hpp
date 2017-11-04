#ifndef RSU_HPP_INCLUDED
#define RSU_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: rsu.hpp
// First created: 2015-09-27-12.03
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#if __cplusplus < 201103L
  #error forest/rsu.hpp needs at least a C++11 compliant compiler
#endif

// C++
#include <cstddef>

// boost
#include <boost/multiprecision/cpp_int.hpp>

// forest
#include <forest/alternatives.hpp>
#include <forest/int128.hpp>

namespace forest
{

  using uint256_t = boost::multiprecision::uint256_t;

  template<typename T> class rsu_traits;

  template<> class rsu_traits<uint8_t>
  {
    public:
      using  type = uint8_t;
      using dtype = uint16_t;

      enum {    hex_digits = 2 };
      enum { base64_digits = 2 };

      enum {         prime = 251u };

      enum {    base_const = 0x61  };
      enum {  offset_const = 0x45  };
      enum {     xor_const = 0xbf  };
  };

  template<> class rsu_traits<uint16_t>
  {
    public:
      using  type = uint16_t;
      using dtype = uint32_t;

      enum {    hex_digits = 4 };
      enum { base64_digits = 3 };

      enum {         prime = 65519u };

      enum {    base_const = 0x3161  };
      enum {  offset_const = 0x2925  };
      enum {     xor_const = 0x3635  };
  };

  template<> class rsu_traits<uint32_t>
  {
    public:
      using  type = uint32_t;
      using dtype = uint64_t;

      enum {    hex_digits = 8 };
      enum { base64_digits = 6 };

      enum {        prime = 4294967291u };

      enum {    base_const = 0x682f0161  };
      enum {  offset_const = 0x46790905  };
      enum {     xor_const = 0x5bf03635  };
  };

  template<> class rsu_traits<uint64_t>
  {
    public:
      using  type =  uint64_t;
      using dtype = uint128_t;

      enum {    hex_digits = 16 };
      enum { base64_digits = 11 };

      enum {        prime = ~(static_cast<uint64_t>(0)) - 188 };

      enum {    base_const = 0x682f0161035fbc46  };
      enum {  offset_const = 0x4679090526f07940  };
      enum {     xor_const = 0xb378dc945bf03635  };
  };

  template<> class rsu_traits<uint128_t>
  {
    public:
      using  type = uint128_t;
      using dtype = uint256_t;

      enum {    hex_digits = 32 };
      enum { base64_digits = 22 };

      enum : type { prime = ~(static_cast<uint128_t>(0)) - 172 };

      enum : type {    base_const = (static_cast<uint128_t>(0x682f0161035fbc46) << 64) | 0xf1038f54c4af08c1 };
      enum : type {  offset_const = (static_cast<uint128_t>(0x4679090526f07940) << 64) | 0x52960757f9072b1f };
      enum : type {     xor_const = (static_cast<uint128_t>(0xb378dc945bf03635) << 64) | 0x50943fb5860b4e95 };
  };

  template<typename T = uint64_t, template<typename> class Traits = rsu_traits>
  class rsu final
  {
    public:

      using  type = typename Traits<T>::type;
      using dtype = typename Traits<T>::dtype;

      enum {    hex_digits = Traits<T>::hex_digits    };
      enum { base64_digits = Traits<T>::base64_digits };

    private:

      type           start_index_;
      type                 index_;
      type   intermediate_offset_;

      enum : type {   base_const = Traits<T>::base_const   };
      enum : type { offset_const = Traits<T>::offset_const };
      enum : type {    xor_const = Traits<T>::xor_const    };

      static type permute( type x ) noexcept
      {
        enum : type { prime = Traits<T>::prime };

        if ( x >= prime ) return x;

        type residue = static_cast<type>( static_cast<dtype>(x) * x % static_cast<type>( prime ) );

        return ( x <= prime / 2 ) ? residue : prime - residue;
      }

      // may be very slow.
      static type unpermute( type x ) noexcept
      {
        static_cast(std::is_same<type, uint8_t>::value || std::is_same<type, uint16_t>::value,
                    "Are you really want to use forest::rsu::unpermute for such a numbers?"
                    " Curse of dimensionality may do it impossible.");

        constexpr type prime = Traits<T>::prime;
        constexpr type p = ( prime + 1 ) / 4;

        type res = x;

        using bt = boost::multiprecision::cpp_int;

        if (x < prime)
        {
          bt power = pow(bt{x}, p);
          power %= prime;

          res = power.operator type();
          if (permute(res) != x) res = prime - res;
        }

        return res;
      }

    public:

      rsu( type seed_base, type seed_offset ) noexcept
        :          start_index_ { permute( permute( seed_base   ) + base_const   ) },
                         index_ { start_index_ },
           intermediate_offset_ { permute( permute( seed_offset ) + offset_const ) }
      {}

      type next() noexcept
      {
        return permute( ( permute( index_++ ) + intermediate_offset_ ) ^ xor_const );
      }

      type operator[] ( type index ) const noexcept
      {
        return permute( ( permute( start_index_ + index ) + intermediate_offset_ ) ^ xor_const );
      }

      // may be very slow:
      type to_index( type p ) const noexcept
      {
        static_cast(std::is_same<type, uint8_t>::value || std::is_same<type, uint16_t>::value,
                    "Are you really want to use forest::rsu::to_index for such a numbers?"
                    " Curse of dimensionality may do it impossible.");
        return unpermute( ( unpermute(p) ^ xor_const ) - intermediate_offset_) - start_index_;
      }

  }; // class rsu

} // namespace forest

#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::rsu;

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL

#endif // RSU_HPP_INCLUDED

// Comments:

// Простое перед 2 в 128 (340282366920938463463374607431768211456):
// 340282366920938463463374607431768211297 (1 mod 4) разница с 2 в 128 - 159
// 340282366920938463463374607431768211283 (3 mod 4) 173

// Простое перед 2 в 63 (9223372036854775808):
// 9223372036854775783 (3 mod 4) разница 25 !!!

// Простое перед 2 в 64 (18446744073709551616):
// 18446744073709551557 (1 mod 4) 59
// 18446744073709551533 (1 mod 4) 83
// 18446744073709551521 (1 mod 4) 95
// 18446744073709551437 (1 mod 4)
// 18446744073709551427 (3 mod 4) 189 !!!

// Original
#if 0

#ifndef __RANDOM_SEQUENCE_H__
#define __RANDOM_SEQUENCE_H__


class RandomSequenceOfUnique
{
  private:
    unsigned int m_index;
    unsigned int m_intermediateOffset;

    static unsigned int permuteQPR( unsigned int x )
    {
      static const unsigned int prime = 4294967291u;
      if ( x >= prime )
        return x;  // The 5 integers out of range are mapped to themselves.
      unsigned int residue = ( ( unsigned long long ) x * x ) % prime;
      return ( x <= prime / 2 ) ? residue : prime - residue;
    }

  public:
    RandomSequenceOfUnique( unsigned int seedBase, unsigned int seedOffset )
    {
      m_index = permuteQPR( permuteQPR( seedBase ) + 0x682f0161 );
      m_intermediateOffset = permuteQPR( permuteQPR( seedOffset ) + 0x46790905 );
    }

    unsigned int next()
    {
      return permuteQPR( ( permuteQPR( m_index++ ) + m_intermediateOffset ) ^ 0x5bf03635 );
    }
};

#endif // __RANDOM_SEQUENCE_H__

#endif // 0
