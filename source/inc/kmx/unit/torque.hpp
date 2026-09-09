/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/torque.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/acceleration.hpp>
    #include <kmx/unit/base.hpp>
    #include <kmx/unit/distance.hpp>
    #include <kmx/unit/mass.hpp>
#endif

/// @brief Units of torque. The base SI unit of the family is the newton metre.
/// @details A torque is a force applied at a distance, so its dimension is that of an energy. The two are
/// not the same quantity, but nothing in the dimension system can separate them: the distinction is
/// conventional, and this library keeps it in the unit type and its symbol. A torque therefore prints as
/// "N·m" rather than "J", and every unit of the family carries the `torsional` tag so that an interface
/// can ask for a torque specifically.
/// @note Because the dimension is shared, a torque and an energy do compare and add. Naming the parameter
/// of an interface with the concept kmx::unit::torque::torque_unit is what states the intent.
namespace kmx::unit::torque
{
    /// @brief One pound-force acting at one foot, in base SI units.
    /// @details Derived from the same constants as kmx::unit::energy::joule_per_foot_pound rather than
    /// repeated as a literal, so that the two can never drift apart: a torque and an energy of the same
    /// dimension must agree to the last bit.
    using newton_meter_per_pound_force_foot =
        scale::multiply_t<scale::multiply_t<mass::kilogram_per_pound, acceleration::standard_gravity_magnitude>, distance::meter_per_foot>;

    /// @brief The size of one pound-force inch in newton metres, exactly one twelfth of a pound-force foot.
    using newton_meter_per_pound_force_inch = scale::divide_t<newton_meter_per_pound_force_foot, scale::ratio<12>>;

    /// @brief The base SI unit of torque, one newton applied at one metre.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(newton_meter, dimension::torque_t, scale::one, std::ratio<0>, "N·m", static constexpr bool torsional = true;)

    /// @brief One thousand newton metres.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(kilonewton_meter, dimension::torque_t, scale::kilo, std::ratio<0>, "kN·m", static constexpr bool torsional = true;)

    /// @brief One pound-force applied at one foot.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(pound_force_foot, dimension::torque_t, newton_meter_per_pound_force_foot, std::ratio<0>, "lbf·ft",
                       static constexpr bool torsional = true;)

    /// @brief One pound-force applied at one inch, that is one twelfth of a pound-force foot.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(pound_force_inch, dimension::torque_t, newton_meter_per_pound_force_inch, std::ratio<0>, "lbf·in",
                       static constexpr bool torsional = true;)

    /// @brief Satisfied by the torque units of this library, which an energy unit does not satisfy.
    /// @tparam T Candidate type.
    template <typename T>
    concept torque_unit = unit_type<T> && std::is_same_v<typename T::dimension_t, dimension::torque_t> && requires { T::torsional; };
}

/// @brief The literal suffixes building torque values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(Nm, unit::torque::newton_meter)
    KMX_UNIT_LITERALS(kNm, unit::torque::kilonewton_meter)
    KMX_UNIT_LITERALS(lbfft, unit::torque::pound_force_foot)
    KMX_UNIT_LITERALS(lbfin, unit::torque::pound_force_inch)
}
