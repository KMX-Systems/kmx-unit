/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/distance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::distance
{
    struct tag
    {
    };

    template <typename T = double>
    struct meter: base<meter<T>, tag, T>
    {
        using base<meter<T>, tag, T>::base;

        template <typename U>
        using rebind = meter<U>;
    };

    template <typename T = double>
    struct kilometer: base<kilometer<T>, tag, T, 1000.0>
    {
        using base<kilometer<T>, tag, T, 1000.0>::base;

        template <typename U>
        using rebind = kilometer<U>;
    };

    template <typename T = double>
    struct foot: base<foot<T>, tag, T, 0.3048>
    {
        using base<foot<T>, tag, T, 0.3048>::base;

        template <typename U>
        using rebind = foot<U>;
    };

    template <typename T = double>
    struct yard: base<yard<T>, tag, T, 0.9144>
    {
        using base<yard<T>, tag, T, 0.9144>::base;

        template <typename U>
        using rebind = yard<U>;
    };

    template <typename T = double>
    struct mile: base<mile<T>, tag, T, 1609.344>
    {
        using base<mile<T>, tag, T, 1609.344>::base;

        template <typename U>
        using rebind = mile<U>;
    };

    template <typename T = double>
    struct nautical_mile: base<nautical_mile<T>, tag, T, 1852.0>
    {
        using base<nautical_mile<T>, tag, T, 1852.0>::base;

        template <typename U>
        using rebind = nautical_mile<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_m, meter)
    KMX_UNIT_FACTORY_FUNCTIONS(_km, kilometer)
    KMX_UNIT_FACTORY_FUNCTIONS(_ft, foot)
    KMX_UNIT_FACTORY_FUNCTIONS(_yd, yard)
    KMX_UNIT_FACTORY_FUNCTIONS(_mi, mile)
    KMX_UNIT_FACTORY_FUNCTIONS(_nmi, nautical_mile)
}
