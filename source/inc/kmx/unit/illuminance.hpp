/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/illuminance.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::illuminance
{
    template <typename T = double>
    struct lux: base<lux<T>, dimension::illuminance, T>
    {
        using base<lux<T>, dimension::illuminance, T>::base;

        template <typename U>
        using rebind = lux<U>;

        static constexpr std::string_view text = "lx";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_lx, unit::illuminance::lux)
}
