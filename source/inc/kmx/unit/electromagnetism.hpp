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
    }
}

namespace kmx::unit
{
    KMX_UNIT_FACTORY_FUNCTIONS(_A, electromagnetism::current::ampere)
    KMX_UNIT_FACTORY_FUNCTIONS(_mA, electromagnetism::current::milliampere)
    KMX_UNIT_FACTORY_FUNCTIONS(_kA, electromagnetism::current::kiloampere)
    KMX_UNIT_FACTORY_FUNCTIONS(_C, electromagnetism::charge::coulomb)
    KMX_UNIT_FACTORY_FUNCTIONS(_V, electromagnetism::voltage::volt)
    KMX_UNIT_FACTORY_FUNCTIONS(_mV, electromagnetism::voltage::millivolt)
    KMX_UNIT_FACTORY_FUNCTIONS(_kV, electromagnetism::voltage::kilovolt)
    KMX_UNIT_FACTORY_FUNCTIONS(_Ohm, electromagnetism::resistance::ohm)
    KMX_UNIT_FACTORY_FUNCTIONS(_kOhm, electromagnetism::resistance::kiloohm)
    KMX_UNIT_FACTORY_FUNCTIONS(_F, electromagnetism::capacitance::farad)
    KMX_UNIT_FACTORY_FUNCTIONS(_H, electromagnetism::inductance::henry)
}
