/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/distance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::distance
{
    template <typename T = double>
    struct meter: base<meter<T>, dimension::length, T>
    {
        using base<meter<T>, dimension::length, T>::base;

        template <typename U>
        using rebind = meter<U>;
    };

    template <typename T = double>
    struct kilometer: base<kilometer<T>, dimension::length, T, 1000.0>
    {
        using base<kilometer<T>, dimension::length, T, 1000.0>::base;

        template <typename U>
        using rebind = kilometer<U>;
    };

    template <typename T = double>
    struct foot: base<foot<T>, dimension::length, T, 0.3048>
    {
        using base<foot<T>, dimension::length, T, 0.3048>::base;

        template <typename U>
        using rebind = foot<U>;
    };

    template <typename T = double>
    struct yard: base<yard<T>, dimension::length, T, 0.9144>
    {
        using base<yard<T>, dimension::length, T, 0.9144>::base;

        template <typename U>
        using rebind = yard<U>;
    };

    template <typename T = double>
    struct mile: base<mile<T>, dimension::length, T, 1609.344>
    {
        using base<mile<T>, dimension::length, T, 1609.344>::base;

        template <typename U>
        using rebind = mile<U>;
    };

    template <typename T = double>
    struct nautical_mile: base<nautical_mile<T>, dimension::length, T, 1852.0>
    {
        using base<nautical_mile<T>, dimension::length, T, 1852.0>::base;

        template <typename U>
        using rebind = nautical_mile<U>;
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_m, unit::distance::meter)
    KMX_UNIT_FACTORY_FUNCTIONS(_km, unit::distance::kilometer)
    KMX_UNIT_FACTORY_FUNCTIONS(_ft, unit::distance::foot)
    KMX_UNIT_FACTORY_FUNCTIONS(_yd, unit::distance::yard)
    KMX_UNIT_FACTORY_FUNCTIONS(_mi, unit::distance::mile)
    KMX_UNIT_FACTORY_FUNCTIONS(_nmi, unit::distance::nautical_mile)
}
