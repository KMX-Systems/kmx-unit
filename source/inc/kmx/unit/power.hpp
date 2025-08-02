/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/power.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::power
{
    constexpr std::uint64_t kilo = 1000ull;
    constexpr std::uint64_t mega = kilo * 1000ull;
    constexpr std::uint64_t giga = mega * 1000ull; // Included for completeness
    constexpr std::uint64_t tera = giga * 1000ull;
    constexpr std::uint64_t peta = tera * 1000ull;

    template <typename T = double>
    struct watt: base<watt<T>, dimension::power, T>
    {
        using base<watt<T>, dimension::power, T>::base;

        template <typename U>
        using rebind = watt<U>;

        static constexpr std::string_view text = "W";
    };

    template <typename T = double>
    struct kilowatt: base<kilowatt<T>, dimension::power, T, static_cast<double>(kilo)>
    {
        using base<kilowatt<T>, dimension::power, T, static_cast<double>(kilo)>::base;

        template <typename U>
        using rebind = kilowatt<U>;

        static constexpr std::string_view text = "kW";
    };

    template <typename T = double>
    struct megawatt: base<megawatt<T>, dimension::power, T, static_cast<double>(mega)>
    {
        using base<megawatt<T>, dimension::power, T, static_cast<double>(mega)>::base;

        template <typename U>
        using rebind = megawatt<U>;

        static constexpr std::string_view text = "MW";
    };

    template <typename T = double>
    struct gigawatt: base<gigawatt<T>, dimension::power, T, static_cast<double>(giga)>
    {
        using base<gigawatt<T>, dimension::power, T, static_cast<double>(giga)>::base;

        template <typename U>
        using rebind = gigawatt<U>;

        static constexpr std::string_view text = "GW";
    };

    template <typename T = double>
    struct terawatt: base<terawatt<T>, dimension::power, T, static_cast<double>(tera)>
    {
        using base<terawatt<T>, dimension::power, T, static_cast<double>(tera)>::base;

        template <typename U>
        using rebind = terawatt<U>;

        static constexpr std::string_view text = "TW";
    };

    template <typename T = double>
    struct petawatt: base<petawatt<T>, dimension::power, T, static_cast<double>(peta)>
    {
        using base<petawatt<T>, dimension::power, T, static_cast<double>(peta)>::base;

        template <typename U>
        using rebind = petawatt<U>;

        static constexpr std::string_view text = "PW";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_W, unit::power::watt)
    KMX_UNIT_FACTORY_FUNCTIONS(_kW, unit::power::kilowatt)
    KMX_UNIT_FACTORY_FUNCTIONS(_MW, unit::power::megawatt)
    KMX_UNIT_FACTORY_FUNCTIONS(_GW, unit::power::gigawatt)
    KMX_UNIT_FACTORY_FUNCTIONS(_TW, unit::power::terawatt)
    KMX_UNIT_FACTORY_FUNCTIONS(_PW, unit::power::petawatt)
}
