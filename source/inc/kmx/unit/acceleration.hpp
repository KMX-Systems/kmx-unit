/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/acceleration.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::acceleration
{
    template <typename T = double>
    struct meters_per_second_squared: base<meters_per_second_squared<T>, dimension::acceleration, T>
    {
        using base<meters_per_second_squared<T>, dimension::acceleration, T>::base;

        template <typename U>
        using rebind = meters_per_second_squared<U>;

        static constexpr std::string_view text = "mps2";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_mps2, unit::acceleration::meters_per_second_squared)
}
