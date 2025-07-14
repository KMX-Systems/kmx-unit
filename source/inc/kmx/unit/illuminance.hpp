/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/illuminance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::illuminance
{
    struct tag
    {
    };

    template <typename T = double>
    struct lux: base<lux<T>, tag, T>
    {
        using base<lux<T>, tag, T>::base;

        template <typename U>
        using rebind = lux<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_lx, lux)
}
