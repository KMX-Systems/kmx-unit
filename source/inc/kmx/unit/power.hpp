/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/power.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::power
{
    struct tag
    {
    };

    template <typename T = double>
    struct watt: base<watt<T>, tag, T>
    {
        using base<watt<T>, tag, T>::base;

        template <typename U>
        using rebind = watt<U>;
    };

    template <typename T = double>
    struct kilowatt: base<kilowatt<T>, tag, T, 1000.0>
    {
        using base<kilowatt<T>, tag, T, 1000.0>::base;

        template <typename U>
        using rebind = kilowatt<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_W, watt)
    KMX_UNIT_FACTORY_FUNCTIONS(_kW, kilowatt)
}
