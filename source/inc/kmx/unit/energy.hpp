/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/energy.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/acceleration.hpp>
    #include <kmx/unit/base.hpp>
    #include <kmx/unit/distance.hpp>
    #include <kmx/unit/mass.hpp>
#endif

/// @brief Units of energy and work. The base SI unit of the family is the joule.
namespace kmx::unit::energy
{
    /// @brief The work of one pound-force over one foot, in joule.
    /// @details Derived from exact definitions: one pound-force is 0.45359237 kg under standard gravity and
    /// one foot is 0.3048 m, which gives 1.3558179483314004 J.
    using joule_per_foot_pound =
        scale::multiply_t<scale::multiply_t<mass::kilogram_per_pound, acceleration::standard_gravity_magnitude>, distance::meter_per_foot>;

    /// @brief The energy of one foot-pound in joules.
    inline constexpr double joule_per_foot_pound_si = scale::value_of<double, joule_per_foot_pound>();

    /// @brief The base SI unit of energy.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(joule, dimension::energy_t, scale::one, "J")

    /// @brief One thousand joule.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilojoule, dimension::energy_t, scale::kilo, "kJ")

    /// @brief The work of one pound-force over one foot, the customary unit of projectile energy.
    /// @note The "pound" of this unit is a force, not the mass unit kmx::unit::mass::pound.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(foot_pound, dimension::energy_t, joule_per_foot_pound, "ft-lbf")

    /// @brief The energy of one kilowatt sustained for one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilowatt_hour, dimension::energy_t, scale::ratio<3600000>, "kWh")

    /// @brief The energy of one megawatt sustained for one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megawatt_hour, dimension::energy_t, scale::ratio<3600000000>, "MWh")

    /// @brief The energy of one gigawatt sustained for one hour.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigawatt_hour, dimension::energy_t, scale::ratio<3600000000000>, "GWh")
}

/// @brief The literal suffixes building energy values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(J, unit::energy::joule)
    KMX_UNIT_LITERALS(kJ, unit::energy::kilojoule)
    KMX_UNIT_LITERALS(ftlbf, unit::energy::foot_pound)
    KMX_UNIT_LITERALS(kWh, unit::energy::kilowatt_hour)
    KMX_UNIT_LITERALS(MWh, unit::energy::megawatt_hour)
    KMX_UNIT_LITERALS(GWh, unit::energy::gigawatt_hour)
}
