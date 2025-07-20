/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/velocity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::velocity
{
    template <typename T = double>
    struct meters_per_second: base<meters_per_second<T>, dimension::velocity, T>
    {
        using base<meters_per_second<T>, dimension::velocity, T>::base;
        template <typename U>
        using rebind = meters_per_second<U>;
    };

    template <typename T = double>
    struct miles_per_hour: base<miles_per_hour<T>, dimension::velocity, T, 1609.344 / 3600.0>
    {
        using base<miles_per_hour<T>, dimension::velocity, T, 1609.344 / 3600.0>::base;
        template <typename U>
        using rebind = miles_per_hour<U>;
    };

    template <typename T = double>
    struct knot: base<knot<T>, dimension::velocity, T, 1852.0 / 3600.0>
    {
        using base<knot<T>, dimension::velocity, T, 1852.0 / 3600.0>::base;
        template <typename U>
        using rebind = knot<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_mps, meters_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_mph, miles_per_hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_kn, knot)
}
