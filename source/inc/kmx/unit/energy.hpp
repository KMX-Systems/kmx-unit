/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/energy.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::energy
{
    template <typename T = double>
    struct joule: base<joule<T>, dimension::energy, T>
    {
        using base<joule<T>, dimension::energy, T>::base;

        template <typename U>
        using rebind = joule<U>;

        static constexpr std::string_view text = "J";
    };

    template <typename T = double>
    struct kilojoule: base<kilojoule<T>, dimension::energy, T, 1000.0>
    {
        using base<kilojoule<T>, dimension::energy, T, 1000.0>::base;

        template <typename U>
        using rebind = kilojoule<U>;

        static constexpr std::string_view text = "kJ";
    };

    template <typename T = double>
    struct kilowatt_hour: base<kilowatt_hour<T>, dimension::energy, T, 3.6e6>
    {
        using base<kilowatt_hour<T>, dimension::energy, T, 3.6e6>::base;

        template <typename U>
        using rebind = kilowatt_hour<U>;

        static constexpr std::string_view text = "kWh";
    };

    template <typename T = double>
    struct megawatt_hour: base<megawatt_hour<T>, dimension::energy, T, 3.6e9>
    {
        using base<megawatt_hour<T>, dimension::energy, T, 3.6e9>::base;

        template <typename U>
        using rebind = megawatt_hour<U>;

        static constexpr std::string_view text = "MWh";
    };

    template <typename T = double>
    struct gigawatt_hour: base<megawatt_hour<T>, dimension::energy, T, 3.6e12>
    {
        using base<megawatt_hour<T>, dimension::energy, T, 3.6e12>::base;

        template <typename U>
        using rebind = megawatt_hour<U>;

        static constexpr std::string_view text = "GWh";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_J, unit::energy::joule)
    KMX_UNIT_FACTORY_FUNCTIONS(_kJ, unit::energy::kilojoule)
    KMX_UNIT_FACTORY_FUNCTIONS(_kWh, unit::energy::kilowatt_hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_MWh, unit::energy::megawatt_hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_GWh, unit::energy::gigawatt_hour)
}
