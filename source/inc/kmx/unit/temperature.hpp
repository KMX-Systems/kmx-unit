/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/temperature.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::temperature
{
    template <typename T = double>
    struct kelvin: base<kelvin<T>, dimension::temperature, T>
    {
        using base<kelvin<T>, dimension::temperature, T>::base;

        template <typename U>
        using rebind = kelvin<U>;

        static constexpr std::string_view text = "K";
    };

    template <typename T = double>
    struct celsius: base<celsius<T>, dimension::temperature, T, 1.0, 273.15>
    {
        using base<celsius<T>, dimension::temperature, T, 1.0, 273.15>::base;

        template <typename U>
        using rebind = celsius<U>;

        static constexpr std::string_view text = "deg C";
    };

    template <typename T = double>
    struct fahrenheit: base<fahrenheit<T>, dimension::temperature, T, 5.0 / 9.0, 273.15 - (32.0 * 5.0 / 9.0)>
    {
        using base<fahrenheit<T>, dimension::temperature, T, 5.0 / 9.0, 273.15 - (32.0 * 5.0 / 9.0)>::base;

        template <typename U>
        using rebind = fahrenheit<U>;

        static constexpr std::string_view text = "deg F";
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_K, unit::temperature::kelvin)
    KMX_UNIT_FACTORY_FUNCTIONS(_degC, unit::temperature::celsius)
    KMX_UNIT_FACTORY_FUNCTIONS(_degF, unit::temperature::fahrenheit)
}
