/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/duration.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::duration
{
    template <typename T = double>
    struct second: base<second<T>, dimension::time, T>
    {
        using base<second<T>, dimension::time, T>::base;

        template <typename U>
        using rebind = second<U>;
    };

    template <typename T = double>
    struct millisecond: base<millisecond<T>, dimension::time, T, 1.0e-3>
    {
        using base<millisecond<T>, dimension::time, T, 1.0e-3>::base;

        template <typename U>
        using rebind = millisecond<U>;
    };

    template <typename T = double>
    struct microsecond: base<microsecond<T>, dimension::time, T, 1.0e-6>
    {
        using base<microsecond<T>, dimension::time, T, 1.0e-6>::base;

        template <typename U>
        using rebind = microsecond<U>;
    };

    template <typename T = double>
    struct nanosecond: base<nanosecond<T>, dimension::time, T, 1.0e-9>
    {
        using base<nanosecond<T>, dimension::time, T, 1.0e-9>::base;

        template <typename U>
        using rebind = nanosecond<U>;
    };

    template <typename T = double>
    struct minute: base<minute<T>, dimension::time, T, 60.0>
    {
        using base<minute<T>, dimension::time, T, 60.0>::base;

        template <typename U>
        using rebind = minute<U>;
    };

    template <typename T = double>
    struct hour: base<hour<T>, dimension::time, T, 3600.0>
    {
        using base<hour<T>, dimension::time, T, 3600.0>::base;

        template <typename U>
        using rebind = hour<U>;
    };

    template <typename T = double>
    struct day: base<day<T>, dimension::time, T, 86400.0>
    {
        using base<day<T>, dimension::time, T, 86400.0>::base;

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
