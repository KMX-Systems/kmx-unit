/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/mass.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of mass. The base SI unit of the family is the kilogram.
/// @note Every unit of this family measures mass, never weight. The pound and the grain are the units of
/// the avoirdupois mass system; a projectile "weight" quoted in grains or pounds in field use is a mass and
/// belongs here. A weight is a force and belongs to kmx::unit::force, where kilogram_force expresses the
/// force exerted by one kilogram of mass under standard gravity.
namespace kmx::unit::mass
{
    /// @brief The size of one avoirdupois pound in kilograms, exactly 0.45359237 by the international
    /// agreement of 1959.
    using kilogram_per_pound = scale::ratio<45359237, 100000000>;

    /// @brief The mass of one avoirdupois pound in kilograms.
    inline constexpr double kilogram_per_pound_si = scale::value_of<double, kilogram_per_pound>();

    /// @brief The size of one grain in kilograms, exactly one 7000th of a pound, that is 64.79891 mg.
    /// @details Being exact is what makes `1.0_lb == 7000.0_gr` true: the two sizes divide into the whole
    /// number 7000, where the quotient of the two rounded sizes was 6999.9999999999991.
    using kilogram_per_grain = scale::divide_t<kilogram_per_pound, scale::ratio<7000>>;

    /// @brief The mass of one grain in kilograms.
    inline constexpr double kilogram_per_grain_si = scale::value_of<double, kilogram_per_grain>();

    /// @brief The base SI unit of mass.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilogram, dimension::mass_t, scale::one, "kg")

    /// @brief One thousandth of a kilogram.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gram, dimension::mass_t, scale::milli, "g")

    /// @brief One millionth of a kilogram.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(milligram, dimension::mass_t, scale::micro, "mg")

    /// @brief One billionth of a kilogram.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(microgram, dimension::mass_t, scale::nano, "ug")

    /// @brief The grain, one 7000th of an avoirdupois pound, the customary unit of projectile mass.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(grain, dimension::mass_t, kilogram_per_grain, "gr")

    /// @brief The avoirdupois pound, exactly 0.45359237 kg.
    /// @note This is a mass, not the pound-force; see kmx::unit::force.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(pound, dimension::mass_t, kilogram_per_pound, "lb")
}

/// @brief The literal suffixes building mass values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(kg, unit::mass::kilogram)
    KMX_UNIT_LITERALS(g, unit::mass::gram)
    KMX_UNIT_LITERALS(mg, unit::mass::milligram)
    KMX_UNIT_LITERALS(ug, unit::mass::microgram)
    KMX_UNIT_LITERALS(gr, unit::mass::grain)
    KMX_UNIT_LITERALS(lb, unit::mass::pound)
}
