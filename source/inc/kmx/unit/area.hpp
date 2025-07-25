/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/area.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::area
{
    template <typename T = double>
    struct square_meter: base<square_meter<T>, dimension::area, T>
    {
        using base<square_meter<T>, dimension::area, T>::base;

        template <typename U>
        using rebind = square_meter<U>;
    };

    template <typename T = double>
    struct hectare: base<hectare<T>, dimension::area, T, 10000.0>
    {
        using base<hectare<T>, dimension::area, T, 10000.0>::base;

        template <typename U>
        using rebind = hectare<U>;
    };

    template <typename T = double>
    struct are: base<are<T>, dimension::area, T, 100.0>
    {
        using base<are<T>, dimension::area, T, 100.0>::base;

        template <typename U>
        using rebind = are<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_m2, square_meter)
    KMX_UNIT_FACTORY_FUNCTIONS(_ha, hectare)
    KMX_UNIT_FACTORY_FUNCTIONS(_a, are)
}
