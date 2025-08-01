/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/mass.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::mass
{
    template <typename T = double>
    struct kilogram: base<kilogram<T>, dimension::mass, T>
    {
        using base<kilogram<T>, dimension::mass, T>::base;

        template <typename U>
        using rebind = kilogram<U>;

        static constexpr std::string_view text = "kg";
    };

    template <typename T = double>
    struct gram: base<gram<T>, dimension::mass, T, 1.0 / 1000.0>
    {
        using base<gram<T>, dimension::mass, T, 1.0 / 1000.0>::base;

        template <typename U>
        using rebind = gram<U>;

        static constexpr std::string_view text = "g";
    };

    template <typename T = double>
    struct milligram: base<milligram<T>, dimension::mass, T, 1.0 / 1e6>
    {
        using base<milligram<T>, dimension::mass, T, 1.0 / 1e6>::base;

        template <typename U>
        using rebind = milligram<U>;

        static constexpr std::string_view text = "mg";
    };

    template <typename T = double>
    struct microgram: base<microgram<T>, dimension::mass, T, 1.0 / 1e9>
    {
        using base<microgram<T>, dimension::mass, T, 1.0 / 1e9>::base;

        template <typename U>
        using rebind = microgram<U>;

        static constexpr std::string_view text = "ug";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_kg, unit::mass::kilogram)
    KMX_UNIT_FACTORY_FUNCTIONS(_g, unit::mass::gram)
    KMX_UNIT_FACTORY_FUNCTIONS(_mg, unit::mass::milligram)
    KMX_UNIT_FACTORY_FUNCTIONS(_ug, unit::mass::microgram)
}
