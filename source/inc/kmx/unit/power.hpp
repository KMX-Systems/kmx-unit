/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/power.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of power. The base SI unit of the family is the watt.
namespace kmx::unit::power
{
    /// @brief The base SI unit of power, one joule per second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(watt, dimension::power_t, scale::one, "W")

    /// @brief One thousand watts.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilowatt, dimension::power_t, scale::kilo, "kW")

    /// @brief One million watts.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megawatt, dimension::power_t, scale::mega, "MW")

    /// @brief One thousand million watts.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigawatt, dimension::power_t, scale::giga, "GW")

    /// @brief One million million watts.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(terawatt, dimension::power_t, scale::tera, "TW")

    /// @brief One thousand million million watts.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(petawatt, dimension::power_t, scale::peta, "PW")
}

/// @brief The literal suffixes building power values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(W, unit::power::watt)
    KMX_UNIT_LITERALS(kW, unit::power::kilowatt)
    KMX_UNIT_LITERALS(MW, unit::power::megawatt)
    KMX_UNIT_LITERALS(GW, unit::power::gigawatt)
    KMX_UNIT_LITERALS(TW, unit::power::terawatt)
    KMX_UNIT_LITERALS(PW, unit::power::petawatt)
}
