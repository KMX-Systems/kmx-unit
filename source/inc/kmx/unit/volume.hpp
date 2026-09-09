/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/volume.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of volume. The base SI unit of the family is the cubic meter.
namespace kmx::unit::volume
{
    /// @brief The base SI unit of volume.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(cubic_meter, dimension::volume_t, scale::one, "m3")

    /// @brief One thousandth of a cubic meter.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(liter, dimension::volume_t, scale::milli, "L")
}

/// @brief The literal suffixes building volume values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(m3, unit::volume::cubic_meter)
    KMX_UNIT_LITERALS(L, unit::volume::liter)
}
