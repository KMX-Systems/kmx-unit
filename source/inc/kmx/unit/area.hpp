/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/area.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of area. The base SI unit of the family is the square meter.
namespace kmx::unit::area
{
    /// @brief The base SI unit of area.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(square_meter, dimension::area_t, scale::one, "m2")

    /// @brief Ten thousand square meters, the unit land area is quoted in.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(hectare, dimension::area_t, scale::ratio<10000>, "ha")

    /// @brief One hundred square meters, that is one hundredth of a hectare.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(are, dimension::area_t, scale::hecto, "a")
}

/// @brief The literal suffixes building area values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(m2, unit::area::square_meter)
    KMX_UNIT_LITERALS(ha, unit::area::hectare)
    KMX_UNIT_LITERALS(a, unit::area::are)
}
