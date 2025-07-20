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
        template <typename T = double>
        struct ampere: base<ampere<T>, dimension::current, T>
        {
            using base<ampere<T>, dimension::current, T>::base;

            template <typename U>
            using rebind = ampere<U>;
        };

        template <typename T = double>
        struct milliampere: base<milliampere<T>, dimension::current, T, 1e-3>
        {
            using base<milliampere<T>, dimension::current, T, 1e-3>::base;

            template <typename U>
            using rebind = milliampere<U>;
        };

        template <typename T = double>
        struct kiloampere: base<kiloampere<T>, dimension::current, T, 1e3>
        {
            using base<kiloampere<T>, dimension::current, T, 1e3>::base;

            template <typename U>
            using rebind = kiloampere<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_A, ampere)
        KMX_UNIT_FACTORY_FUNCTIONS(_mA, milliampere)
        KMX_UNIT_FACTORY_FUNCTIONS(_kA, kiloampere)
    }

    namespace charge
    {
        template <typename T = double>
        struct coulomb: base<coulomb<T>, dimension::charge, T>
        {
            using base<coulomb<T>, dimension::charge, T>::base;

            template <typename U>
            using rebind = coulomb<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_C, coulomb)
    }

    namespace voltage
    {
        template <typename T = double>
        struct volt: base<volt<T>, dimension::voltage, T>
        {
            using base<volt<T>, dimension::voltage, T>::base;

            template <typename U>
            using rebind = volt<U>;
        };

        template <typename T = double>
        struct millivolt: base<millivolt<T>, dimension::voltage, T, 1e-3>
        {
            using base<millivolt<T>, dimension::voltage, T, 1e-3>::base;

            template <typename U>
            using rebind = millivolt<U>;
        };

        template <typename T = double>
        struct kilovolt: base<kilovolt<T>, dimension::voltage, T, 1e3>
        {
            using base<kilovolt<T>, dimension::voltage, T, 1e3>::base;

            template <typename U>
            using rebind = kilovolt<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_V, volt)
        KMX_UNIT_FACTORY_FUNCTIONS(_mV, millivolt)
        KMX_UNIT_FACTORY_FUNCTIONS(_kV, kilovolt)
    }

    namespace resistance
    {
        template <typename T = double>
        struct ohm: base<ohm<T>, dimension::resistance, T>
        {
            using base<ohm<T>, dimension::resistance, T>::base;

            template <typename U>
            using rebind = ohm<U>;
        };

        template <typename T = double>
        struct kiloohm: base<kiloohm<T>, dimension::resistance, T, 1000.0>
        {
            using base<kiloohm<T>, dimension::resistance, T, 1000.0>::base;

            template <typename U>
            using rebind = kiloohm<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_Ohm, ohm)
        KMX_UNIT_FACTORY_FUNCTIONS(_kOhm, kiloohm)
    }

    namespace capacitance
    {
        template <typename T = double>
        struct farad: base<farad<T>, dimension::capacitance, T>
        {
            using base<farad<T>, dimension::capacitance, T>::base;

            template <typename U>
            using rebind = farad<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_F, farad)
    }

    namespace inductance
    {
        template <typename T = double>
        struct henry: base<henry<T>, dimension::inductance, T>
        {
            using base<henry<T>, dimension::inductance, T>::base;

            template <typename U>
            using rebind = henry<U>;
        };

        KMX_UNIT_FACTORY_FUNCTIONS(_H, henry)
    }
}
