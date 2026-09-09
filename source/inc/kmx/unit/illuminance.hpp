/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/illuminance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of illuminance. The base SI unit of the family is the lux.
namespace kmx::unit::illuminance
{
    /// @brief The base SI unit of illuminance, one lumen per square meter.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(lux, dimension::illuminance_t, scale::one, "lx")
}

/// @brief The literal suffixes building illuminance values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(lx, unit::illuminance::lux)
}
