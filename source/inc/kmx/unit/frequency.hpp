/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/frequency.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of frequency. The base SI unit of the family is the hertz.
/// @details A frequency is a count of events per unit of time, so its dimension is the reciprocal of a
/// time, which is also the dimension of an angular velocity. The dimension system cannot tell the two
/// apart; the unit type and its symbol can, and one hertz is 2·pi radians per second rather than one.
namespace kmx::unit::frequency
{
    /// @brief The size of one revolution per minute in hertz, exactly one sixtieth.
    using hertz_per_revolution_per_minute = scale::ratio<1, 60>;

    /// @brief The base SI unit of frequency, one cycle per second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(hertz, dimension::frequency_t, scale::one, "Hz")

    /// @brief One thousand hertz.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilohertz, dimension::frequency_t, scale::kilo, "kHz")

    /// @brief One million hertz.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megahertz, dimension::frequency_t, scale::mega, "MHz")

    /// @brief One thousand million hertz.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigahertz, dimension::frequency_t, scale::giga, "GHz")

    /// @brief One revolution per minute, that is one sixtieth of a hertz.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(revolutions_per_minute, dimension::frequency_t, hertz_per_revolution_per_minute, "rpm")
}

/// @brief The literal suffixes building frequency values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(Hz, unit::frequency::hertz)
    KMX_UNIT_LITERALS(kHz, unit::frequency::kilohertz)
    KMX_UNIT_LITERALS(MHz, unit::frequency::megahertz)
    KMX_UNIT_LITERALS(GHz, unit::frequency::gigahertz)
    KMX_UNIT_LITERALS(rpm, unit::frequency::revolutions_per_minute)
}
