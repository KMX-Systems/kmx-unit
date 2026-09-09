/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/temperature.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of thermodynamic temperature. The base SI unit of the family is the kelvin.
/// @details This family holds two kinds of value that must never be mixed:
/// - a reading, that is a position on a temperature scale: kmx::unit::temperature::kelvin,
///   kmx::unit::temperature::celsius and kmx::unit::temperature::fahrenheit;
/// - a difference, that is a step along a scale: kmx::unit::temperature::delta_kelvin,
///   kmx::unit::temperature::delta_celsius and kmx::unit::temperature::delta_fahrenheit.
///
/// The distinction is enforced by the type system. Readings carry the `absolute_scale` tag, so two of them
/// cannot be added, a reading cannot be scaled by a factor, a reading and a difference never compare, and
/// kmx::unit::convert refuses to turn one kind into the other. Subtracting two readings yields a difference
/// automatically, and adding a difference to a reading yields a reading, which is the only arithmetic that
/// makes physical sense on a scale with an origin.
///
/// Normalization to SI is safe for both kinds: a reading normalizes to kelvin through its origin offset,
/// while a difference normalizes to a step in kelvin, which for the Celsius scale is the same number.
namespace kmx::unit::temperature
{
    /// @brief The kelvin value of the origin of the Celsius scale, exactly 273.15.
    using kelvin_at_celsius_zero = std::ratio<5463, 20>;

    /// @brief The size of one degree Fahrenheit relative to one kelvin, exactly five ninths.
    using kelvin_per_fahrenheit_degree = scale::ratio<5, 9>;

    /// @brief The kelvin value of the origin of the Fahrenheit scale, exactly 273.15 - 32 * 5/9.
    /// @details Keeping it exact is what makes a round trip land back on the number it started from:
    /// 32 degF converts to exactly 0 degC, where a rounded origin left it at 3.55e-15.
    using kelvin_at_fahrenheit_zero = std::ratio_subtract<kelvin_at_celsius_zero, std::ratio_multiply<std::ratio<32>, std::ratio<5, 9>>>;

    /// @brief The kelvin value of the origin of the Celsius scale, as a number.
    inline constexpr double kelvin_at_celsius_zero_si =
        static_cast<double>(kelvin_at_celsius_zero::num) / static_cast<double>(kelvin_at_celsius_zero::den);

    /// @brief A difference of temperature expressed in kelvin.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(delta_kelvin, dimension::temperature_t, scale::one, "delta K")

    /// @brief A difference of temperature expressed in degrees Celsius.
    /// @note One degree Celsius of difference is exactly one kelvin of difference.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(delta_celsius, dimension::temperature_t, scale::one, "delta deg C")

    /// @brief A difference of temperature expressed in degrees Fahrenheit.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(delta_fahrenheit, dimension::temperature_t, kelvin_per_fahrenheit_degree, "delta deg F")

    /// @brief A temperature reading on the base SI scale, whose origin is absolute zero.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(kelvin, dimension::temperature_t, scale::one, std::ratio<0>, "K", using difference_t = delta_kelvin<T>;
                       static constexpr bool absolute_scale = true;)

    /// @brief A temperature reading on the Celsius scale.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(celsius, dimension::temperature_t, scale::one, kelvin_at_celsius_zero, "deg C",
                       using difference_t = delta_celsius<T>;
                       static constexpr bool absolute_scale = true;)

    /// @brief A temperature reading on the Fahrenheit scale.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(fahrenheit, dimension::temperature_t, kelvin_per_fahrenheit_degree, kelvin_at_fahrenheit_zero, "deg F",
                       using difference_t = delta_fahrenheit<T>;
                       static constexpr bool absolute_scale = true;)
}

/// @brief The literal suffixes building temperature values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(K, unit::temperature::kelvin)
    KMX_UNIT_LITERALS(degC, unit::temperature::celsius)
    KMX_UNIT_LITERALS(degF, unit::temperature::fahrenheit)
    KMX_UNIT_LITERALS(delta_K, unit::temperature::delta_kelvin)
    KMX_UNIT_LITERALS(delta_degC, unit::temperature::delta_celsius)
    KMX_UNIT_LITERALS(delta_degF, unit::temperature::delta_fahrenheit)
}
