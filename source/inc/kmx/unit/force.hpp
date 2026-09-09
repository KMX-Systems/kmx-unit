/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/force.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/acceleration.hpp>
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of force. The base SI unit of the family is the newton.
/// @details A weight belongs here, a mass does not: kmx::unit::mass::pound measures a mass, while the
/// force that mass exerts under standard gravity is a value of this family.
namespace kmx::unit::force
{
    /// @brief The base SI unit of force, the force accelerating one kilogram by one meter per second squared.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(newton, dimension::force_t, scale::one, "N")

    /// @brief One thousand newtons.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilonewton, dimension::force_t, scale::kilo, "kN")

    /// @brief The force one kilogram of mass exerts under standard gravity.
    /// @note This is a force, not a mass. A mass quoted in kilograms belongs to kmx::unit::mass.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilogram_force, dimension::force_t, acceleration::standard_gravity_magnitude, "kgf")
}

/// @brief The literal suffixes building force values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(N, unit::force::newton)
    KMX_UNIT_LITERALS(kN, unit::force::kilonewton)
    KMX_UNIT_LITERALS(kgf, unit::force::kilogram_force)
}
