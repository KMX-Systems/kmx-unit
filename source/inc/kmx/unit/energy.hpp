/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/energy.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::energy
{
    struct tag
    {
    };

    template <typename T = double>
    struct joule: base<joule<T>, tag, T>
    {
        using base<joule<T>, tag, T>::base;

        template <typename U>
        using rebind = joule<U>;
    };

    template <typename T = double>
    struct kilojoule: base<kilojoule<T>, tag, T, 1000.0>
    {
        using base<kilojoule<T>, tag, T, 1000.0>::base;

        template <typename U>
        using rebind = kilojoule<U>;
    };

    template <typename T = double>
    struct kilowatt_hour: base<kilowatt_hour<T>, tag, T, 3.6e6>
    {
        using base<kilowatt_hour<T>, tag, T, 3.6e6>::base;

        template <typename U>
        using rebind = kilowatt_hour<U>;
    };

    template <typename T = double>
    struct megawatt_hour: base<megawatt_hour<T>, tag, T, 3.6e9>
    {
        using base<megawatt_hour<T>, tag, T, 3.6e9>::base;

        template <typename U>
        using rebind = megawatt_hour<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_J, joule)
    KMX_UNIT_FACTORY_FUNCTIONS(_kJ, kilojoule)
    KMX_UNIT_FACTORY_FUNCTIONS(_kWh, kilowatt_hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_MWh, megawatt_hour)
}
