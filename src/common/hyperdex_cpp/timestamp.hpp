#ifndef HYPERDEX_TIMESTAMP_HPP_INCLUDED
#define HYPERDEX_TIMESTAMP_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: timestamp.hpp
// First created: 2015-10-18-07.24
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

// C++
#include <chrono>

// forest
#include <forest/date.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>

namespace hyperdex
{
  template <typename, typename> class complex_data_traits;

  template<hyperdatatype T,
           template<typename, typename = void> typename ComplexTraits = complex_data_traits>
  class timestamp final
  {
      static_assert(T == HYPERDATATYPE_STRING           ||
                    T == HYPERDATATYPE_TIMESTAMP_SECOND ||
                    T == HYPERDATATYPE_TIMESTAMP_MINUTE ||
                    T == HYPERDATATYPE_TIMESTAMP_HOUR   ||
                    T == HYPERDATATYPE_TIMESTAMP_DAY    ||
                    T == HYPERDATATYPE_TIMESTAMP_WEEK   ||
                    T == HYPERDATATYPE_TIMESTAMP_MONTH,
                    "Wrong template parameter for timestamp type.");

      using stamp_t
        = std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<int128_t, std::nano>>;

      static_assert(sizeof(stamp_t) == 16,
                    "Wrong size of underlying timestamps type!"
                    " Underlying type for timestamps must be signed 128 bit integer."
                    " It is used to cast types in library.");

      static_assert(sizeof(unsigned) >= 4,
                    "Type 'unsigned' was used in functions' parameters for nanoseconds"
                    " and is too small on your compiler to save 1e9 (1 sec).");

      template<typename R, typename P>
      friend constexpr timestamp operator+(timestamp const& ts,
                                           std::chrono::duration<R,P> const& d)
      {
        return timestamp{ts} += d;
      }

      template<typename R, typename P>
      friend constexpr timestamp operator-(timestamp const& ts,
                                           std::chrono::duration<R,P> const& d)
      {
        return timestamp{ts} -= d;
      }

      template<typename R, typename P>
      friend constexpr timestamp operator+(std::chrono::duration<R,P> const& d,
                                           timestamp const& ts)
      {
        return timestamp(d + ts.stamp_);
      }

      template<typename R, typename P>
      friend constexpr timestamp operator-(std::chrono::duration<R,P> const& d,
                                           timestamp const& ts)
      {
        return timestamp(d + ts.stamp_);
      }

      template<typename D>
      friend constexpr bool operator==(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ == t;
      }

      template<typename D>
      friend constexpr bool operator!=(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ != t;
      }

      template<typename D>
      friend constexpr bool operator>(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ > t;
      }

      template<typename D>
      friend constexpr bool operator>=(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ >= t;
      }

      template<typename D>
      friend constexpr bool operator<(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ < t;
      }

      template<typename D>
      friend constexpr bool operator<=(timestamp const& ts,
                           std::chrono::time_point<std::chrono::system_clock,D> const& t)
      {
        return ts.stamp_ <= t;
      }

      template<typename D>
      friend constexpr bool operator==(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                       timestamp const& ts)
      {
        return ts.stamp_ == t;
      }

      template<typename D>
      friend constexpr bool operator!=(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                       timestamp const& ts)
      {
        return ts.stamp_ != t;
      }

      template<typename D>
      friend constexpr bool operator>(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                      timestamp const& ts)
      {
        return t > ts.stamp_;
      }

      template<typename D>
      friend constexpr bool operator>=(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                       timestamp const& ts)
      {
        return t >= ts.stamp_;
      }

      template<typename D>
      friend constexpr bool operator<(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                      timestamp const& ts)
      {
        return t < ts.stamp_;
      }

      template<typename D>
      friend constexpr bool operator<=(std::chrono::time_point<std::chrono::system_clock,D> const& t,
                                       timestamp const& ts)
      {
        return t <= ts.stamp_;
      }

      template<hyperdatatype U>
      friend constexpr bool operator==(timestamp    const& t1,
                                       timestamp<U> const& t2)
      {
        return t1.stamp_ == t2.stamp();
      }

      template<hyperdatatype U>
      friend constexpr bool operator!=(timestamp    const& t1,
                                       timestamp<U> const& t2)
      {
        return t1.stamp_ != t2.stamp();
      }

      template<hyperdatatype U>
      friend constexpr bool operator>(timestamp    const& t1,
                                      timestamp<U> const& t2)
      {
        return t1.stamp_ > t2.stamp();
      }

      template<hyperdatatype U>
      friend constexpr bool operator>=(timestamp    const& t1,
                                       timestamp<U> const& t2)
      {
        return t1.stamp_ >= t2.stamp();
      }

      template<hyperdatatype U>
      friend constexpr bool operator<(timestamp    const& t1,
                                      timestamp<U> const& t2)
      {
        return t1.stamp_ < t2.stamp();
      }

      template<hyperdatatype U>
      friend constexpr bool operator<=(timestamp    const& t1,
                                       timestamp<U> const& t2)
      {
        return t1.stamp_ <= t2.stamp();
      }

      template<typename ToDuration>
      friend constexpr
      std::chrono::time_point<std::chrono::system_clock, ToDuration>
      time_point_cast(timestamp const& ts)
      {
        return std::chrono::time_point_cast<ToDuration>(ts);
      }

    public:

      static constexpr bool const  is_timestamp = true;
      static constexpr hyperdatatype const type = T;

      constexpr timestamp() noexcept = default;

      constexpr explicit timestamp(stamp_t const& stamp) noexcept: stamp_ { stamp } {}

      template<hyperdatatype U>
      constexpr explicit timestamp(timestamp<U> const& ts) noexcept
      {
        stamp_ = ts.stamp();
      }

      constexpr explicit timestamp(an_attribute const& attr)
      : stamp_ { ComplexTraits<std::decay_t<timestamp>>::deserialize(attr) }
      {}

      template<hyperdatatype U>
      constexpr timestamp& operator=(timestamp<U> const& ts) noexcept
      {
        stamp_ = ts.stamp();
        return *this;
      }

      constexpr timestamp& operator=(stamp_t const& stamp) noexcept
      {
        stamp_ = stamp;
        return *this;
      }

      template<hyperdatatype U>
      constexpr operator timestamp<U>() const noexcept
      {
        return timestamp<U>{stamp_};
      }

      template <typename Rep, typename Period>
      constexpr timestamp& operator+=(std::chrono::duration<Rep,Period> const& d)
      {
        stamp_ += d;
        return *this;
      }

      template <typename Rep, typename Period>
      constexpr timestamp& operator-=(std::chrono::duration<Rep,Period> const& d)
      {
        stamp_ -= d;
        return *this;
      }

      constexpr auto time_since_epoch() const { return stamp_.time_since_epoch(); }

      static constexpr auto min() { return stamp_t::min; }
      static constexpr auto max() { return stamp_t::max; }

      constexpr timestamp(unsigned year, unsigned month, unsigned day,
                          unsigned hour, unsigned minute, unsigned sec,
                          unsigned ns = 0) noexcept
      : stamp_ { date::day_point { date::year(year)/month/day }
                 + std::chrono::hours(hour)
                 + std::chrono::minutes(minute)
                 + std::chrono::seconds(sec)
                 + std::chrono::nanoseconds(ns) }
      {}

      constexpr operator stamp_t() const noexcept { return stamp_; }

      constexpr auto stamp() const noexcept { return stamp_; }

      constexpr timestamp from_hyperdex(hyperdex_int t) noexcept { return from_microseconds(t); }

      constexpr timestamp from_time_t(time_t t) noexcept
      {
        stamp_ = stamp_t { std::chrono::duration<__int128_t> { t } };
        return *this;
      }

      constexpr timestamp from_seconds(int128_t t) noexcept
      {
        stamp_ = stamp_t { std::chrono::duration<__int128_t> { t } };
        return *this;
      }

      constexpr timestamp from_milliseconds(int128_t t) noexcept
      {
        stamp_ = stamp_t { std::chrono::duration<__int128_t, std::milli> { t } };
        return *this;
      }

      constexpr timestamp from_microseconds(int128_t t) noexcept
      {
        stamp_ = stamp_t { std::chrono::duration<__int128_t, std::micro> { t } };
        return *this;
      }

      constexpr timestamp from_nanoseconds(int128_t t) noexcept
      {
        stamp_ = *reinterpret_cast<stamp_t*>(&t);
        return *this;
      }

      constexpr timestamp from_ymd_hms(unsigned year, unsigned month, unsigned day,
                                       unsigned hour, unsigned minute, unsigned sec,
                                       unsigned ns = 0) noexcept
      {
        stamp_ = date::day_point { date::year(year)/month/day }
                 + std::chrono::hours(hour)
                 + std::chrono::minutes(minute)
                 + std::chrono::seconds(sec)
                 + std::chrono::nanoseconds(ns);

        return *this;
      }

      constexpr int128_t     as_int128_t() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_); }
      constexpr hyperdex_int         to_hyperdex() const noexcept { return to_microseconds(); }
      constexpr time_t                 to_time_t() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_) / 1e9; }
      constexpr int128_t      to_seconds() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_) / 1e9; }
      constexpr int128_t to_milliseconds() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_) / 1e6; }
      constexpr int128_t to_microseconds() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_) / 1e3; }
      constexpr int128_t  to_nanoseconds() const noexcept { return *reinterpret_cast<int128_t const*>(&stamp_); }

      constexpr bool is_in_hyperdex_limits() const noexcept
      {
        constexpr int128_t base { ~uint64_t{0} >> 1 };
        constexpr int128_t max  { base * 1e3 };
        constexpr int128_t min  { - (base + 1) * 1e3 };
        return *reinterpret_cast<int128_t const*>(&stamp_) >= min
            && *reinterpret_cast<int128_t const*>(&stamp_) <= max;
      }

      constexpr bool is_in_time_t_limits() const noexcept
      {
        constexpr int128_t base { ~uint64_t{0} >> 1 };
        constexpr int128_t max  { base * 1e9 };
        constexpr int128_t min  { - (base + 1) * 1e9 };
        return *reinterpret_cast<int128_t const*>(&stamp_) >= min
            && *reinterpret_cast<int128_t const*>(&stamp_) <= max;
      }

      constexpr bool is_in_system_clock_limits() const noexcept
      {
        constexpr int128_t base { ~uint64_t{0} >> 1 };
        constexpr int128_t max  { base };
        constexpr int128_t min  { - (base + 1) };
        return *reinterpret_cast<int128_t const*>(&stamp_) >= min
            && *reinterpret_cast<int128_t const*>(&stamp_) <= max;
      }

      constexpr bool lost_precision_to_hyperdex() const noexcept
      {
        return *reinterpret_cast<int128_t const*>(&stamp_) !=
              (*reinterpret_cast<int128_t const*>(&stamp_) / 1000) * 1000;
      }

      constexpr bool lost_precision_to_time_t() const noexcept
      {
        return *reinterpret_cast<int128_t const*>(&stamp_) !=
              (*reinterpret_cast<int128_t const*>(&stamp_) / 1e9) * 1e9;
      }

    private:
      stamp_t stamp_ {};

  }; // template<hyperdatatype T> class timestamp

  using timestamp_second = timestamp<HYPERDATATYPE_TIMESTAMP_SECOND>;
  using timestamp_minute = timestamp<HYPERDATATYPE_TIMESTAMP_MINUTE>;
  using timestamp_hour   = timestamp<HYPERDATATYPE_TIMESTAMP_HOUR>;
  using timestamp_day    = timestamp<HYPERDATATYPE_TIMESTAMP_DAY>;
  using timestamp_week   = timestamp<HYPERDATATYPE_TIMESTAMP_WEEK>;
  using timestamp_month  = timestamp<HYPERDATATYPE_TIMESTAMP_MONTH>;
  using full_timestamp   = timestamp<HYPERDATATYPE_STRING>;

} // namespace hyperdex

#endif // HYPERDEX_TIMESTAMP_HPP_INCLUDED
