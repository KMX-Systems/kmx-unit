/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/testing.hpp
#pragma once
#define CATCH_CONFIG_MAIN
#ifndef PCH
    #include <catch2/catch_test_macros.hpp>
    #include <catch2/matchers/catch_matchers_floating_point.hpp>
#endif

/// @brief The helpers shared by the test suite of the library.
namespace kmx::unit
{
    /// @brief Matches a floating point value against an expected one within an absolute tolerance.
    /// @param val The expected value.
    /// @return A Catch2 matcher accepting a value within 1e-4 of @p val.
    inline auto approx(const auto val) noexcept(false)
    {
        return Catch::Matchers::WithinAbs(val, 1e-4);
    }

    /// @brief Matches a floating point value against an expected one within a relative tolerance.
    /// @details An absolute tolerance is meaningless once the magnitude of a value grows, as it does for a
    /// data rate in bits per second or an energy in joules.
    /// @param val The expected value.
    /// @param tolerance The accepted relative deviation.
    /// @return A Catch2 matcher accepting a value within @p tolerance of @p val.
    inline auto approx_rel(const auto val, const double tolerance = 1e-9) noexcept(false)
    {
        return Catch::Matchers::WithinRel(static_cast<double>(val), tolerance);
    }

    // Expression validity probes, used to assert that a misuse does not compile. They have to be written
    // as concepts rather than as a bare requires-expression, so that the check stays a substitution.

    /// @brief Satisfied when two values may be added.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_add = requires(const Lhs lhs, const Rhs rhs) { lhs + rhs; };

    /// @brief Satisfied when two values may be subtracted.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_subtract = requires(const Lhs lhs, const Rhs rhs) { lhs - rhs; };

    /// @brief Satisfied when two values may be compared for equality.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_compare = requires(const Lhs lhs, const Rhs rhs) { lhs == rhs; };

    /// @brief Satisfied when two values may be ordered.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_order = requires(const Lhs lhs, const Rhs rhs) { lhs < rhs; };

    /// @brief Satisfied when a value may be multiplied by a scalar.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_scale = requires(const Unit unit) { unit * 2.0; };

    /// @brief Satisfied when a value may be divided by a scalar.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_descale = requires(const Unit unit) { unit / 2.0; };

    /// @brief Satisfied when a value may be multiplied by a scalar written on the left.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_prescale = requires(const Unit unit) { 2.0 * unit; };

    /// @brief Satisfied when a value may be multiplied by a scalar of a given type.
    /// @tparam Unit Candidate unit type.
    /// @tparam Scalar Candidate scalar type.
    template <typename Unit, typename Scalar>
    concept can_scale_by = requires(const Unit unit, const Scalar scalar) { unit * scalar; };

    /// @brief Satisfied when a value may be divided by a scalar of a given type.
    /// @tparam Unit Candidate unit type.
    /// @tparam Scalar Candidate scalar type.
    template <typename Unit, typename Scalar>
    concept can_descale_by = requires(const Unit unit, const Scalar scalar) { unit / scalar; };

    /// @brief Satisfied when a value may be multiplied by a scalar of a given type written on the left.
    /// @tparam Scalar Candidate scalar type.
    /// @tparam Unit Candidate unit type.
    template <typename Scalar, typename Unit>
    concept can_prescale_by = requires(const Scalar scalar, const Unit unit) { scalar * unit; };

    /// @brief Satisfied when a value may be scaled in place by a scalar of a given type.
    /// @tparam Unit Candidate unit type.
    /// @tparam Scalar Candidate scalar type.
    template <typename Unit, typename Scalar>
    concept can_scale_assign_by = requires(Unit unit, const Scalar scalar) { unit *= scalar; };

    /// @brief Satisfied when a value may be divided in place by a scalar of a given type.
    /// @tparam Unit Candidate unit type.
    /// @tparam Scalar Candidate scalar type.
    template <typename Unit, typename Scalar>
    concept can_descale_assign_by = requires(Unit unit, const Scalar scalar) { unit /= scalar; };

    /// @brief Satisfied when a scalar of a given type may be divided by a value, inverting its dimension.
    /// @tparam Scalar Candidate scalar type.
    /// @tparam Unit Candidate unit type.
    template <typename Scalar, typename Unit>
    concept can_invert_by = requires(const Scalar scalar, const Unit unit) { scalar / unit; };

    /// @brief Satisfied when a value may be added to another in place.
    /// @tparam Lhs Left operand type, the one assigned to.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_add_assign = requires(Lhs lhs, const Rhs rhs) { lhs += rhs; };

    /// @brief Satisfied when a value may be subtracted from another in place.
    /// @tparam Lhs Left operand type, the one assigned to.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_subtract_assign = requires(Lhs lhs, const Rhs rhs) { lhs -= rhs; };

    /// @brief Satisfied when the magnitude of a value may be taken.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_abs = requires(const Unit unit) { abs(unit); };

    /// @brief Satisfied when the square root of a value may be taken.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_sqrt = requires(const Unit unit) { sqrt(unit); };

    /// @brief Satisfied when a value may be raised to a power.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept can_pow = requires(const Unit unit) { pow<2>(unit); };

    /// @brief Satisfied when the smaller of two values may be taken.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_min = requires(const Lhs lhs, const Rhs rhs) { min(lhs, rhs); };

    /// @brief Satisfied when the hypotenuse of two values may be taken.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_hypot = requires(const Lhs lhs, const Rhs rhs) { hypot(lhs, rhs); };

    /// @brief Satisfied when two values may be multiplied by each other.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_multiply = requires(const Lhs lhs, const Rhs rhs) { lhs * rhs; };

    /// @brief Satisfied when two values may be divided by each other.
    /// @tparam Lhs Left operand type.
    /// @tparam Rhs Right operand type.
    template <typename Lhs, typename Rhs>
    concept can_divide = requires(const Lhs lhs, const Rhs rhs) { lhs / rhs; };
}
