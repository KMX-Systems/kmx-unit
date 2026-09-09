/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/duration.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of time. The base SI unit of the family is the second.
/// @details Every unit of the family is a fixed span of time. None of them follows civil time, so a
/// day is always 86400 seconds even across a leap second or a change of time zone.
namespace kmx::unit::duration
{
    /// @brief The base SI unit of time.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(second, dimension::time_t, scale::one, std::ratio<0>, "s", static constexpr std::uint32_t per_day = 86400u;
                       static constexpr std::uint32_t per_hour = 3600u; static constexpr std::uint32_t per_minute = 60u;)

    /// @brief One thousandth of a second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(millisecond, dimension::time_t, scale::milli, "ms")

    /// @brief One millionth of a second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(microsecond, dimension::time_t, scale::micro, "us")

    /// @brief One thousand millionth of a second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(nanosecond, dimension::time_t, scale::nano, "ns")

    /// @brief Sixty seconds.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(minute, dimension::time_t, scale::ratio<60>, std::ratio<0>, "min", static constexpr std::uint32_t per_hour = 60u;)

    /// @brief Sixty minutes, that is 3600 seconds.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(hour, dimension::time_t, scale::ratio<3600>, std::ratio<0>, "h", static constexpr std::uint32_t per_day = 24u;)

    /// @brief Twenty-four hours, that is 86400 seconds.
    /// @note This is a fixed span of time, not a calendar day: a calendar day is longer or shorter than
    /// this one whenever a leap second or a change of civil time falls into it.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(day, dimension::time_t, scale::ratio<86400>, std::ratio<0>, "d", static constexpr std::uint32_t per_week = 7u;)
}

namespace kmx::unit
{
    /// @brief Satisfied by the units measuring a time.
    /// @details It lives in this header rather than in kmx/unit/chrono.hpp so that a header needing only
    /// the concept, such as kmx/unit/data_rate.hpp, does not have to include the whole of <chrono> for it.
    /// @tparam U Candidate type.
    template <typename U>
    concept time_unit = unit_type<U> && std::is_same_v<typename U::dimension_t, dimension::time_t>;
}

/// @brief The literal suffixes building duration values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(s, unit::duration::second)
    KMX_UNIT_LITERALS(ms, unit::duration::millisecond)
    KMX_UNIT_LITERALS(us, unit::duration::microsecond)
    KMX_UNIT_LITERALS(ns, unit::duration::nanosecond)
    KMX_UNIT_LITERALS(min, unit::duration::minute)
    KMX_UNIT_LITERALS(h, unit::duration::hour)
    KMX_UNIT_LITERALS(d, unit::duration::day)
}
