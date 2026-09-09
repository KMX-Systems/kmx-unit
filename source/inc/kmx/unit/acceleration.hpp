/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/acceleration.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <kmx/unit/distance.hpp>
#endif

/// @brief Units of acceleration. The base SI unit of the family is the meter per second squared.
namespace kmx::unit::acceleration
{
    /// @brief The size of standard gravity, the conventional acceleration of free fall, exactly 9.80665 m/s².
    /// @details It is kept exact rather than rounded because the pound-force, the psi and the foot-pound
    /// are all defined from it, and a rounding here would put every one of those beside its defined value.
    /// @reference CGPM, 3rd Conference (1901), definition of standard acceleration due to gravity.
    using standard_gravity_magnitude = scale::ratio<196133, 20000>;

    /// @brief Standard gravity, the conventional acceleration of free fall, in base SI units.
    inline constexpr double standard_gravity_si = scale::value_of<double, standard_gravity_magnitude>();

    /// @brief The base SI unit of acceleration.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(meters_per_second_squared, dimension::acceleration_t, scale::one, "m/s2")

    /// @brief One foot per second, per second; used where the customary units are the reporting units.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(feet_per_second_squared, dimension::acceleration_t, distance::meter_per_foot, "ft/s2")

    /// @brief Standard gravity as a typed value, the single definition every caller should refer to.
    /// @note This is the conventional constant, not the local gravity of a given latitude and altitude.
    inline constexpr meters_per_second_squared<double> standard_gravity {standard_gravity_si};
}

/// @brief The literal suffixes building acceleration values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(mps2, unit::acceleration::meters_per_second_squared)
    KMX_UNIT_LITERALS(fps2, unit::acceleration::feet_per_second_squared)
}
