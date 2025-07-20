/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/volume.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::volume
{
    template <typename T = double>
    struct cubic_meter: base<cubic_meter<T>, dimension::volume, T>
    {
        using base<cubic_meter<T>, dimension::volume, T>::base;

        template <typename U>
        using rebind = cubic_meter<U>;
    };

    template <typename T = double>
    struct liter: base<liter<T>, dimension::volume, T, 0.001>
    {
        using base<liter<T>, dimension::volume, T, 0.001>::base;

        template <typename U>
        using rebind = liter<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_m3, cubic_meter)
    KMX_UNIT_FACTORY_FUNCTIONS(_L, liter)
}
