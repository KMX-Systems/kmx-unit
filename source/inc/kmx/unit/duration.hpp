/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/duration.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::duration
{
    struct tag
    {
    };

    template <typename T = double>
    struct second: base<second<T>, tag, T>
    {
        using base<second<T>, tag, T>::base;

        template <typename U>
        using rebind = second<U>;
    };

    template <typename T = double>
    struct millisecond: base<millisecond<T>, tag, T, 1.0e-3>
    {
        using base<millisecond<T>, tag, T, 1.0e-3>::base;

        template <typename U>
        using rebind = millisecond<U>;
    };

    template <typename T = double>
    struct microsecond: base<microsecond<T>, tag, T, 1.0e-6>
    {
        using base<microsecond<T>, tag, T, 1.0e-6>::base;

        template <typename U>
        using rebind = microsecond<U>;
    };

    template <typename T = double>
    struct nanosecond: base<nanosecond<T>, tag, T, 1.0e-9>
    {
        using base<nanosecond<T>, tag, T, 1.0e-9>::base;

        template <typename U>
        using rebind = nanosecond<U>;
    };

    template <typename T = double>
    struct minute: base<minute<T>, tag, T, 60.0>
    {
        using base<minute<T>, tag, T, 60.0>::base;

        template <typename U>
        using rebind = minute<U>;
    };

    template <typename T = double>
    struct hour: base<hour<T>, tag, T, 3600.0>
    {
        using base<hour<T>, tag, T, 3600.0>::base;

        template <typename U>
        using rebind = hour<U>;
    };

    template <typename T = double>
    struct day: base<day<T>, tag, T, 86400.0>
    {
        using base<day<T>, tag, T, 86400.0>::base;

        template <typename U>
        using rebind = day<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_s, second)
    KMX_UNIT_FACTORY_FUNCTIONS(_ms, millisecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_us, microsecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_ns, nanosecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_min, minute)
    KMX_UNIT_FACTORY_FUNCTIONS(_h, hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_d, day)
}
