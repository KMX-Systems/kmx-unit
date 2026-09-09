/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/electromagnetism.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of the electromagnetic quantities, grouped by quantity in a namespace of their own.
/// @details The base SI unit of each family is the one the SI names: the ampere, the coulomb, the volt,
/// the ohm, the farad and the henry.
namespace kmx::unit::electromagnetism
{
    /// @brief Units of electric current.
    namespace current
    {
        /// @brief The base SI unit of electric current.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(ampere, dimension::current_t, scale::one, "A")

        /// @brief One thousandth of an ampere.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(milliampere, dimension::current_t, scale::milli, "mA")

        /// @brief One thousand amperes.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(kiloampere, dimension::current_t, scale::kilo, "kA")
    }

    /// @brief Units of electric charge.
    namespace charge
    {
        /// @brief The SI unit of electric charge, the charge one ampere carries in one second.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(coulomb, dimension::charge_t, scale::one, "C")
    }

    /// @brief Units of electric potential.
    namespace voltage
    {
        /// @brief The SI unit of electric potential, one joule per coulomb.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(volt, dimension::voltage_t, scale::one, "V")

        /// @brief One thousandth of a volt.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(millivolt, dimension::voltage_t, scale::milli, "mV")

        /// @brief One thousand volts.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(kilovolt, dimension::voltage_t, scale::kilo, "kV")
    }

    /// @brief Units of electric resistance.
    namespace resistance
    {
        /// @brief The SI unit of electric resistance, one volt per ampere.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(ohm, dimension::resistance_t, scale::one, "Ohm")

        /// @brief One thousand ohms.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(kiloohm, dimension::resistance_t, scale::kilo, "kOhm")
    }

    /// @brief Units of capacitance.
    namespace capacitance
    {
        /// @brief The SI unit of capacitance, one coulomb per volt.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(farad, dimension::capacitance_t, scale::one, "F")
    }

    /// @brief Units of inductance.
    namespace inductance
    {
        /// @brief The SI unit of inductance, one volt-second per ampere.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(henry, dimension::inductance_t, scale::one, "H")
    }
    /// @brief Units of magnetic flux.
    namespace magnetic_flux
    {
        /// @brief The SI unit of magnetic flux, the flux that induces one volt as it decays in one second.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(weber, dimension::magnetic_flux_t, scale::one, "Wb")

        /// @brief One thousandth of a weber.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(milliweber, dimension::magnetic_flux_t, scale::milli, "mWb")
    }

    /// @brief Units of magnetic flux density.
    namespace magnetic_flux_density
    {
        /// @brief The size of one gauss in tesla, exactly one ten-thousandth.
        using tesla_per_gauss = scale::ratio<1, 10000>;

        /// @brief The SI unit of magnetic flux density, one weber per square metre.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(tesla, dimension::magnetic_flux_density_t, scale::one, "T")

        /// @brief One thousandth of a tesla.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(millitesla, dimension::magnetic_flux_density_t, scale::milli, "mT")

        /// @brief One ten thousandth of a tesla, the unit of the CGS system.
        /// @tparam T The arithmetic type holding the value.
        KMX_UNIT_DEFINE(gauss, dimension::magnetic_flux_density_t, tesla_per_gauss, "G")
    }
}

/// @brief The literal suffixes building electromagnetism values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(A, unit::electromagnetism::current::ampere)
    KMX_UNIT_LITERALS(mA, unit::electromagnetism::current::milliampere)
    KMX_UNIT_LITERALS(kA, unit::electromagnetism::current::kiloampere)
    KMX_UNIT_LITERALS(C, unit::electromagnetism::charge::coulomb)
    KMX_UNIT_LITERALS(V, unit::electromagnetism::voltage::volt)
    KMX_UNIT_LITERALS(mV, unit::electromagnetism::voltage::millivolt)
    KMX_UNIT_LITERALS(kV, unit::electromagnetism::voltage::kilovolt)
    KMX_UNIT_LITERALS(Ohm, unit::electromagnetism::resistance::ohm)
    KMX_UNIT_LITERALS(kOhm, unit::electromagnetism::resistance::kiloohm)
    KMX_UNIT_LITERALS(F, unit::electromagnetism::capacitance::farad)
    KMX_UNIT_LITERALS(H, unit::electromagnetism::inductance::henry)
    KMX_UNIT_LITERALS(Wb, unit::electromagnetism::magnetic_flux::weber)
    KMX_UNIT_LITERALS(mWb, unit::electromagnetism::magnetic_flux::milliweber)
    KMX_UNIT_LITERALS(T, unit::electromagnetism::magnetic_flux_density::tesla)
    KMX_UNIT_LITERALS(mT, unit::electromagnetism::magnetic_flux_density::millitesla)
    KMX_UNIT_LITERALS(G, unit::electromagnetism::magnetic_flux_density::gauss)
}
