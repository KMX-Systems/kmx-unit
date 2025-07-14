/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/force.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::force
{
    struct tag
    {
    };

    template <typename T = double>
    struct newton: base<newton<T>, tag, T>
    {
        using base<newton<T>, tag, T>::base;

        template <typename U>
        using rebind = newton<U>;
    };

    template <typename T = double>
    struct kilonewton: base<kilonewton<T>, tag, T, 1000.0>
    {
        using base<kilonewton<T>, tag, T, 1000.0>::base;

        template <typename U>
        using rebind = kilonewton<U>;
    };

    template <typename T = double>
    struct kilogram_force: base<kilogram_force<T>, tag, T, 9.80665>
    {
        using base<kilogram_force<T>, tag, T, 9.80665>::base;

        template <typename U>
        using rebind = kilogram_force<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_N, newton)
    KMX_UNIT_FACTORY_FUNCTIONS(_kN, kilonewton)
    KMX_UNIT_FACTORY_FUNCTIONS(_kgf, kilogram_force)
}
