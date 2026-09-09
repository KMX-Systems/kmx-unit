/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/pressure.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/acceleration.hpp>
    #include <kmx/unit/base.hpp>
    #include <kmx/unit/distance.hpp>
    #include <kmx/unit/mass.hpp>
#endif

/// @brief Units of pressure. The base SI unit of the family is the pascal.
/// @note Every unit of this family carries an absolute pressure, never a gauge pressure. A gauge reading is
/// the difference between an absolute pressure and the ambient one, so it is the caller who has to add the
/// ambient pressure before handing a value to this library. No name or helper of this library implies a
/// gauge reading, and none should be read as one.
namespace kmx::unit::pressure
{
    /// @brief The pressure of one pound-force per square inch, in pascal.
    /// @details Derived from exact definitions: one pound-force is 0.45359237 kg under standard gravity and
    /// one square inch is 0.0254 m squared.
    using pascal_per_psi = scale::divide_t<scale::multiply_t<mass::kilogram_per_pound, acceleration::standard_gravity_magnitude>,
                                           scale::multiply_t<distance::meter_per_inch, distance::meter_per_inch>>;

    /// @brief The pressure of one psi in pascals.
    inline constexpr double pascal_per_psi_si = scale::value_of<double, pascal_per_psi>();

    /// @brief The pressure of one millimeter of mercury, in pascal.
    /// @details The conventional millimeter of mercury: a mercury density of 13595.1 kg/m3 under standard
    /// gravity, which gives exactly 133.322387415 Pa.
    /// @reference ISO 80000-4, conventional millimetre of mercury.
    using pascal_per_millimeter_of_mercury =
        scale::multiply_t<scale::multiply_t<scale::ratio<135951, 10>, acceleration::standard_gravity_magnitude>, scale::milli>;

    /// @brief The pressure of one inch of mercury, in pascal, being 25.4 millimeters of mercury.
    using pascal_per_inch_of_mercury = scale::multiply_t<pascal_per_millimeter_of_mercury, scale::ratio<254, 10>>;

    /// @brief The base SI unit of pressure.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(pascal, dimension::pressure_t, scale::one, "Pa")

    /// @brief One hundred pascal, the unit of the meteorological station pressure report.
    /// @note One hectopascal equals one millibar.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(hectopascal, dimension::pressure_t, scale::hecto, "hPa")

    /// @brief One thousand pascal.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilopascal, dimension::pressure_t, scale::kilo, "kPa")

    /// @brief One million pascal.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megapascal, dimension::pressure_t, scale::mega, "MPa")

    /// @brief Exactly one hundred thousand pascal.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(bar, dimension::pressure_t, scale::ratio<100000>, "bar")

    /// @brief One pound-force per square inch.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(psi, dimension::pressure_t, pascal_per_psi, "psi")

    /// @brief One inch of mercury, the customary unit of a station pressure report.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(inch_of_mercury, dimension::pressure_t, pascal_per_inch_of_mercury, "inHg")

    /// @brief One millimeter of mercury.
    /// @note The conventional millimeter of mercury is not exactly the torr, which is one 760th of the
    /// standard atmosphere: the two differ by about two parts in ten million.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(millimeter_of_mercury, dimension::pressure_t, pascal_per_millimeter_of_mercury, "mmHg")
}

/// @brief The literal suffixes building pressure values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(Pa, unit::pressure::pascal)
    KMX_UNIT_LITERALS(hPa, unit::pressure::hectopascal)
    KMX_UNIT_LITERALS(kPa, unit::pressure::kilopascal)
    KMX_UNIT_LITERALS(MPa, unit::pressure::megapascal)
    KMX_UNIT_LITERALS(bar, unit::pressure::bar)
    KMX_UNIT_LITERALS(psi, unit::pressure::psi)
    KMX_UNIT_LITERALS(inHg, unit::pressure::inch_of_mercury)
    KMX_UNIT_LITERALS(mmHg, unit::pressure::millimeter_of_mercury)
}
