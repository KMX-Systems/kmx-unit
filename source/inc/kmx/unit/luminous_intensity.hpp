/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/luminous_intensity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::luminous_intensity
{
    template <typename T = double>
    struct candela: base<candela<T>, dimension::luminous_intensity, T>
    {
        using base<candela<T>, dimension::luminous_intensity, T>::base;

        template <typename U>
        using rebind = candela<U>;

        static constexpr std::string_view text = "cd";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_cd, unit::luminous_intensity::candela)
}
