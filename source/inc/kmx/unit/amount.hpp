/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/amount.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of amount of substance. The base SI unit of the family is the mole.
namespace kmx::unit::amount
{
    /// @brief The base SI unit of amount of substance.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(mole, dimension::amount_of_substance_t, scale::one, "mol")

    /// @brief One thousandth of a mole.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(millimole, dimension::amount_of_substance_t, scale::milli, "mmol")

    /// @brief One millionth of a mole.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(micromole, dimension::amount_of_substance_t, scale::micro, "umol")

    /// @brief One thousand moles.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilomole, dimension::amount_of_substance_t, scale::kilo, "kmol")
}

/// @brief The literal suffixes building amount values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(mol, unit::amount::mole)
    KMX_UNIT_LITERALS(mmol, unit::amount::millimole)
    KMX_UNIT_LITERALS(umol, unit::amount::micromole)
    KMX_UNIT_LITERALS(kmol, unit::amount::kilomole)
}
