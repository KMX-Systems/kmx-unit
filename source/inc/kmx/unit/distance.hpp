/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/distance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of length. The base SI unit of the family is the meter.
/// @note The imperial units are the international definitions of 1959, which are exact by definition:
/// one inch is exactly 25.4 mm and every other imperial length follows from it.
namespace kmx::unit::distance
{
    /// @brief The size of one inch in meters, exactly 0.0254 by definition.
    using meter_per_inch = scale::ratio<127, 5000>;

    /// @brief The size of one foot in meters, exactly twelve inches.
    using meter_per_foot = scale::multiply_t<meter_per_inch, scale::ratio<12>>;

    /// @brief The size of one yard in meters, exactly three feet.
    using meter_per_yard = scale::multiply_t<meter_per_foot, scale::ratio<3>>;

    /// @brief The size of one mile in meters, exactly 1760 yards.
    using meter_per_mile = scale::multiply_t<meter_per_yard, scale::ratio<1760>>;

    /// @brief The size of one nautical mile in meters, exactly 1852 by definition.
    using meter_per_nautical_mile = scale::ratio<1852>;

    /// @brief One thousandth of a meter, used for bore diameters and short offsets.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(millimeter, dimension::length_t, scale::milli, "mm")

    /// @brief The base SI unit of length.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(meter, dimension::length_t, scale::one, "m")

    /// @brief One thousand meters.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilometer, dimension::length_t, scale::kilo, "km")

    /// @brief The international inch, exactly 25.4 mm.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(inch, dimension::length_t, meter_per_inch, "in")

    /// @brief The international foot, exactly 12 inches.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(foot, dimension::length_t, meter_per_foot, "ft")

    /// @brief The international yard, exactly 3 feet.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(yard, dimension::length_t, meter_per_yard, "yd")

    /// @brief The international statute mile, exactly 1760 yards.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(mile, dimension::length_t, meter_per_mile, "mi")

    /// @brief The international nautical mile, exactly 1852 m.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(nautical_mile, dimension::length_t, meter_per_nautical_mile, "nmi")
}

/// @brief The literal suffixes building distance values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(mm, unit::distance::millimeter)
    KMX_UNIT_LITERALS(m, unit::distance::meter)
    KMX_UNIT_LITERALS(km, unit::distance::kilometer)
    KMX_UNIT_LITERALS(in, unit::distance::inch)
    KMX_UNIT_LITERALS(ft, unit::distance::foot)
    KMX_UNIT_LITERALS(yd, unit::distance::yard)
    KMX_UNIT_LITERALS(mi, unit::distance::mile)
    KMX_UNIT_LITERALS(nmi, unit::distance::nautical_mile)
}
