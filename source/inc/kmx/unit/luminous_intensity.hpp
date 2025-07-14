/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/luminous_intensity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::luminous_intensity
{
    struct tag
    {
    };

    template <typename T = double>
    struct candela: base<candela<T>, tag, T>
    {
        using base<candela<T>, tag, T>::base;

        template <typename U>
        using rebind = candela<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_cd, candela)
}
