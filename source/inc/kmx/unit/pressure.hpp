/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/pressure.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::pressure
{
    template <typename T = double>
    struct pascal: base<pascal<T>, dimension::pressure, T>
    {
        using base<pascal<T>, dimension::pressure, T>::base;

        template <typename U>
        using rebind = pascal<U>;
    };

    template <typename T = double>
    struct kilopascal: base<kilopascal<T>, dimension::pressure, T, 1000.0>
    {
        using base<kilopascal<T>, dimension::pressure, T, 1000.0>::base;

        template <typename U>
        using rebind = kilopascal<U>;
    };

    template <typename T = double>
    struct megapascal: base<megapascal<T>, dimension::pressure, T, 1.0e6>
    {
        using base<megapascal<T>, dimension::pressure, T, 1.0e6>::base;

        template <typename U>
        using rebind = megapascal<U>;
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_Pa, unit::pressure::pascal)
    KMX_UNIT_FACTORY_FUNCTIONS(_kPa, unit::pressure::kilopascal)
    KMX_UNIT_FACTORY_FUNCTIONS(_MPa, unit::pressure::megapascal)
}
