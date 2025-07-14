/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/electromagnetism.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::electromagnetism
{
    namespace current
    {
        struct tag
        {
        };

        template <typename T = double>
        struct ampere: base<ampere<T>, tag, T>
        {
            using base<ampere<T>, tag, T>::base;

            template <typename U>
            using rebind = ampere<U>;
        };

        template <typename T = double>
        struct milliampere: base<milliampere<T>, tag, T, 1e-3>
        {
            using base<milliampere<T>, tag, T, 1e-3>::base;

            template <typename U>
            using rebind = milliampere<U>;
        };

        template <typename T = double>
        struct kiloampere: base<kiloampere<T>, tag, T, 1e3>
        {
            using base<kiloampere<T>, tag, T, 1e3>::base;

            template <typename U>
            using rebind = kiloampere<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_A, ampere)
        KMX_UNIT_FACTORY_FUNCTIONS(_mA, milliampere)
        KMX_UNIT_FACTORY_FUNCTIONS(_kA, kiloampere)
    }

    namespace charge
    {
        struct tag
        {
        };

        template <typename T = double>
        struct coulomb: base<coulomb<T>, tag, T>
        {
            using base<coulomb<T>, tag, T>::base;

            template <typename U>
            using rebind = coulomb<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_C, coulomb)
    }

    namespace voltage
    {
        struct tag
        {
        };

        template <typename T = double>
        struct volt: base<volt<T>, tag, T>
        {
            using base<volt<T>, tag, T>::base;

            template <typename U>
            using rebind = volt<U>;
        };

        template <typename T = double>
        struct millivolt: base<millivolt<T>, tag, T, 1e-3>
        {
            using base<millivolt<T>, tag, T, 1e-3>::base;

            template <typename U>
            using rebind = millivolt<U>;
        };

        template <typename T = double>
        struct kilovolt: base<kilovolt<T>, tag, T, 1e3>
        {
            using base<kilovolt<T>, tag, T, 1e3>::base;

            template <typename U>
            using rebind = kilovolt<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_V, volt)
        KMX_UNIT_FACTORY_FUNCTIONS(_mV, millivolt)
        KMX_UNIT_FACTORY_FUNCTIONS(_kV, kilovolt)
    }

    namespace resistance
    {
        struct tag
        {
        };

        template <typename T = double>
        struct ohm: base<ohm<T>, tag, T>
        {
            using base<ohm<T>, tag, T>::base;

            template <typename U>
            using rebind = ohm<U>;
        };

        template <typename T = double>
        struct kiloohm: base<kiloohm<T>, tag, T, 1000.0>
        {
            using base<kiloohm<T>, tag, T, 1000.0>::base;

            template <typename U>
            using rebind = kiloohm<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_Ohm, ohm)
        KMX_UNIT_FACTORY_FUNCTIONS(_kOhm, kiloohm)
    }

    namespace capacitance
    {
        struct tag
        {
        };

        template <typename T = double>
        struct farad: base<farad<T>, tag, T>
        {
            using base<farad<T>, tag, T>::base;

            template <typename U>
            using rebind = farad<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_F, farad)
    }

    namespace inductance
    {
        struct tag
        {
        };

        template <typename T = double>
        struct henry: base<henry<T>, tag, T>
        {
            using base<henry<T>, tag, T>::base;

            template <typename U>
            using rebind = henry<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_H, henry)
    }
}
