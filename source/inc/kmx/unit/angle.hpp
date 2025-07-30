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
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_rad, unit::angle::radian)
    KMX_UNIT_FACTORY_FUNCTIONS(_deg, unit::angle::degree)
}
