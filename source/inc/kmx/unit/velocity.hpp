/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/velocity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <kmx/unit/distance.hpp>
#endif

/// @brief Units of speed. The base SI unit of the family is the meter per second.
namespace kmx::unit::velocity
{
    /// @brief The number of seconds in one hour, the divisor every per-hour unit of this family carries.
    using seconds_per_hour = scale::ratio<3600>;

    /// @brief The size of one kilometre per hour in meters per second, exactly one kilometre over an hour.
    using meter_per_second_per_kilometer_per_hour = scale::divide_t<scale::kilo, seconds_per_hour>;

    /// @brief The size of one mile per hour in meters per second, exactly one mile over an hour.
    using meter_per_second_per_mile_per_hour = scale::divide_t<distance::meter_per_mile, seconds_per_hour>;

    /// @brief The size of one knot in meters per second, exactly one nautical mile over an hour.
    using meter_per_second_per_knot = scale::divide_t<distance::meter_per_nautical_mile, seconds_per_hour>;

    /// @brief The base SI unit of speed.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(meters_per_second, dimension::velocity_t, scale::one, "m/s")

    /// @brief One foot covered in one second, the customary unit of muzzle velocity.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(feet_per_second, dimension::velocity_t, distance::meter_per_foot, "ft/s")

    /// @brief One kilometer covered in one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilometers_per_hour, dimension::velocity_t, meter_per_second_per_kilometer_per_hour, "km/h")

    /// @brief One statute mile covered in one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(miles_per_hour, dimension::velocity_t, meter_per_second_per_mile_per_hour, "mph")

    /// @brief One nautical mile covered in one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(knot, dimension::velocity_t, meter_per_second_per_knot, "kn")
}

/// @brief The literal suffixes building velocity values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(mps, unit::velocity::meters_per_second)
    KMX_UNIT_LITERALS(fps, unit::velocity::feet_per_second)
    KMX_UNIT_LITERALS(kmh, unit::velocity::kilometers_per_hour)
    KMX_UNIT_LITERALS(mph, unit::velocity::miles_per_hour)
    KMX_UNIT_LITERALS(kn, unit::velocity::knot)
}
