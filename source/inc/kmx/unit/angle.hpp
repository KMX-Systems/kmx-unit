/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/angle.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <numbers>
#endif

namespace kmx::unit::angle
{
    template <typename T = double>
    struct radian: base<radian<T>, dimension::angle, T>
    {
        using base<radian<T>, dimension::angle, T>::base;

        template <typename U>
        using rebind = radian<U>;
    };

    template <typename T = double>
    struct degree: base<degree<T>, dimension::angle, T, std::numbers::pi / 180.0>
    {
        using base<degree<T>, dimension::angle, T, std::numbers::pi / 180.0>::base;

        template <typename U>
        using rebind = degree<U>;
    };

    template <typename T = double>
    struct arcminute: base<arcminute<T>, dimension::angle, T, std::numbers::pi / (180.0 * 60.0)>
    {
        using base<arcminute<T>, dimension::angle, T, std::numbers::pi / (180.0 * 60.0)>::base;
        template <typename U>
        using rebind = arcminute<U>;
    };

    template <typename T = double>
    struct arcsecond: base<arcsecond<T>, dimension::angle, T, std::numbers::pi / (180.0 * 3600.0)>
    {
        using base<arcsecond<T>, dimension::angle, T, std::numbers::pi / (180.0 * 3600.0)>::base;
        template <typename U>
        using rebind = arcsecond<U>;
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_rad, unit::angle::radian)
    KMX_UNIT_FACTORY_FUNCTIONS(_deg, unit::angle::degree)
    KMX_UNIT_FACTORY_FUNCTIONS(_arcmin, unit::angle::arcminute)
    KMX_UNIT_FACTORY_FUNCTIONS(_arcsec, unit::angle::arcsecond)
}
