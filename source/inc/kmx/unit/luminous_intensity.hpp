/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/luminous_intensity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of luminous intensity. The base SI unit of the family is the candela.
namespace kmx::unit::luminous_intensity
{
    /// @brief The base SI unit of luminous intensity.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(candela, dimension::luminous_intensity_t, scale::one, "cd")
}

/// @brief The literal suffixes building luminous intensity values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(cd, unit::luminous_intensity::candela)
}
