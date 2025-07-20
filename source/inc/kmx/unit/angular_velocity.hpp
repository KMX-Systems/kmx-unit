/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/angular_velocity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <numbers>
#endif

namespace kmx::unit::angular_velocity
{
    template <typename T = double>
    struct radian_per_second: base<radian_per_second<T>, dimension::angular_velocity, T>
    {
        using base<radian_per_second<T>, dimension::angular_velocity, T>::base;

        template <typename U>
        using rebind = radian_per_second<U>;
    };

    template <typename T = double>
    struct degree_per_second: base<degree_per_second<T>, dimension::angular_velocity, T, std::numbers::pi / 180.0>
    {
        using base<degree_per_second<T>, dimension::angular_velocity, T, std::numbers::pi / 180.0>::base;

        template <typename U>
        using rebind = degree_per_second<U>;
    };

    KMX_UNIT_FACTORY_FUNCTIONS(_rad_s, radian_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_deg_s, degree_per_second)
}
