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

        static constexpr std::uint32_t per_day = 86400u;
        static constexpr std::uint32_t per_hour = 3600u;
        static constexpr std::uint32_t per_minute = 60u;

        static constexpr std::string_view text = "s";
    };

    template <typename T = double>
    struct millisecond: base<millisecond<T>, dimension::time, T, 1.0e-3>
    {
        using base<millisecond<T>, dimension::time, T, 1.0e-3>::base;

        template <typename U>
        using rebind = millisecond<U>;

        static constexpr std::string_view text = "ms";
    };

    template <typename T = double>
    struct microsecond: base<microsecond<T>, dimension::time, T, 1.0e-6>
    {
        using base<microsecond<T>, dimension::time, T, 1.0e-6>::base;

        template <typename U>
        using rebind = microsecond<U>;

        static constexpr std::string_view text = "us";
    };

    template <typename T = double>
    struct nanosecond: base<nanosecond<T>, dimension::time, T, 1.0e-9>
    {
        using base<nanosecond<T>, dimension::time, T, 1.0e-9>::base;

        template <typename U>
        using rebind = nanosecond<U>;

        static constexpr std::string_view text = "ns";
    };

    template <typename T = double>
    struct minute: base<minute<T>, dimension::time, T, 60.0>
    {
        using base<minute<T>, dimension::time, T, 60.0>::base;

        template <typename U>
        using rebind = minute<U>;

        static constexpr std::uint32_t per_hour = 60u;

        static constexpr std::string_view text = "min";
    };

    template <typename T = double>
    struct hour: base<hour<T>, dimension::time, T, 3600.0>
    {
        using base<hour<T>, dimension::time, T, 3600.0>::base;

        template <typename U>
        using rebind = hour<U>;

        static constexpr std::uint32_t per_day = 24u;

        static constexpr std::string_view text = "h";
    };

    template <typename T = double>
    struct day: base<day<T>, dimension::time, T, 86400.0>
    {
        using base<day<T>, dimension::time, T, 86400.0>::base;

        template <typename U>
        using rebind = day<U>;

        static constexpr std::uint32_t per_week = 7u;

        static constexpr std::string_view text = "d";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_s, unit::duration::second)
    KMX_UNIT_FACTORY_FUNCTIONS(_ms, unit::duration::millisecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_us, unit::duration::microsecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_ns, unit::duration::nanosecond)
    KMX_UNIT_FACTORY_FUNCTIONS(_min, unit::duration::minute)
    KMX_UNIT_FACTORY_FUNCTIONS(_h, unit::duration::hour)
    KMX_UNIT_FACTORY_FUNCTIONS(_d, unit::duration::day)
}
