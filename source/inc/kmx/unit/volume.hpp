/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/volume.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::volume
{
    struct tag
    {
    };

    template <typename T = double>
    struct cubic_meter: base<cubic_meter<T>, tag, T>
    {
        using base<cubic_meter<T>, tag, T>::base;

        template <typename U>
        using rebind = cubic_meter<U>;
    };

    template <typename T = double>
    struct liter: base<liter<T>, tag, T, 1.0 / 1000.0>
    {
        using base<liter<T>, tag, T, 1.0 / 1000.0>::base;

        template <typename U>
        using rebind = liter<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_m3, cubic_meter)
    KMX_UNIT_FACTORY_FUNCTIONS(_L, liter)
}
