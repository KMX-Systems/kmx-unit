/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/math.hpp
/// @brief The mathematical functions that carry a unit through, rather than dropping it.
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <cmath>
    #include <cstdint>
#endif

namespace kmx::unit::detail
{
    template <typename T, bool = std::is_integral_v<T> && std::is_signed_v<T>>
    struct abs_value
    {
        using type_t = T;
    };

    template <typename T>
    struct abs_value<T, true>
    {
        using type_t = std::make_unsigned_t<T>;
    };

    /// @brief Raises a value to a compile-time integer power by exponentiation by squaring.
    /// @details `std::pow` is neither constexpr nor exact for an integer exponent, so binary exponentiation
    /// gives both a compile-time result and the exact one for a power of a representable value. Its work is
    /// logarithmic in the exponent rather than linear, which matters for dimension powers beyond the usual
    /// square and cube.
    /// @tparam N The exponent.
    /// @tparam T The arithmetic type of the value.
    /// @param value The value to raise.
    /// @return @p value raised to the power @p N.
    template <std::intmax_t N, typename T>
    [[nodiscard]] constexpr T integer_power(const T value) noexcept
    {
        if constexpr (N == 0)
            return T {1};
        else
        {
            constexpr auto exponent = []() constexpr {
                if constexpr (N < 0)
                    return static_cast<std::uintmax_t>(-(N + 1)) + 1u;
                else
                    return static_cast<std::uintmax_t>(N);
            }();

            T result {1};
            T factor {value};
            auto remaining = exponent;
            while (remaining != 0u)
            {
                if ((remaining & 1u) != 0u)
                    result *= factor;
                remaining >>= 1u;
                if (remaining != 0u)
                    factor *= factor;
            }

            if constexpr (N < 0)
                return T {1} / result;
            else
                return result;
        }
    }
}

namespace kmx::unit
{
    /// @brief The magnitude of a value, in the unit it is expressed in.
    /// @details An absolute reading is rejected: the magnitude of a position on a scale with an origin
    /// follows the choice of origin rather than the quantity. A signed integral value is rebound to its
    /// unsigned value type, so the magnitude of the most-negative value remains representable.
    /// @tparam Unit The unit type.
    /// @param value The value.
    /// @return The magnitude of @p value.
    template <typename Unit>
    [[nodiscard]] constexpr auto abs(const Unit& value) noexcept
        requires(unit_type<Unit> && !is_absolute_scale_v<Unit>)
    {
        using value_t = typename Unit::value_t;
        using result_value_t = typename detail::abs_value<value_t>::type_t;
        using result_t = typename Unit::template rebind_t<result_value_t>;
        const auto native = value.as_native();
        if constexpr (std::is_integral_v<value_t> && std::is_signed_v<value_t>)
        {
            if (native < value_t {})
                return result_t(static_cast<result_value_t>(-(native + 1)) + result_value_t {1});
        }
        else if constexpr (std::is_signed_v<value_t>)
        {
            if (native < value_t {})
                return result_t(-native);
        }

        return result_t(static_cast<result_value_t>(native));
    }

    /// @brief The smaller of two values of the same dimension and kind.
    /// @tparam U1 Left unit type, whose unit the result is expressed in.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The smaller value, expressed in the unit of @p lhs.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto min(const U1& lhs, const U2& rhs) noexcept
        requires comparable_units<U1, U2>
    {
        using value_t = additive_t<U1, U2>;
        using result_t = typename U1::template rebind_t<value_t>;
        const auto left = static_cast<value_t>(lhs.as_native());
        const auto right = detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native()));
        return result_t((right < left) ? right : left);
    }

    /// @brief The larger of two values of the same dimension and kind.
    /// @tparam U1 Left unit type, whose unit the result is expressed in.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The larger value, expressed in the unit of @p lhs.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto max(const U1& lhs, const U2& rhs) noexcept
        requires comparable_units<U1, U2>
    {
        using value_t = additive_t<U1, U2>;
        using result_t = typename U1::template rebind_t<value_t>;
        const auto left = static_cast<value_t>(lhs.as_native());
        const auto right = detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native()));
        return result_t((left < right) ? right : left);
    }

    /// @brief Confines a value to a range.
    /// @tparam Unit The unit type of the value, whose unit the result is expressed in.
    /// @tparam Low The unit type of the lower bound.
    /// @tparam High The unit type of the upper bound.
    /// @param value The value to confine.
    /// @param low The lower bound.
    /// @param high The upper bound.
    /// @return @p value, or the bound it exceeds, expressed in the unit of @p value.
    template <typename Unit, typename Low, typename High>
    [[nodiscard]] constexpr auto clamp(const Unit& value, const Low& low, const High& high) noexcept
        requires(comparable_units<Unit, Low> && comparable_units<Unit, High>)
    {
        return min(max(value, low), high);
    }

    /// @brief The length of the hypotenuse of the right triangle with the two given sides.
    /// @details Computed with std::hypot, which does not overflow for values whose squares would.
    /// @note The two sides must denote the same KIND of quantity and not merely the same dimension, exactly
    /// as kmx::unit::min and kmx::unit::max require: a torque and an energy share a dimension, and the
    /// hypotenuse of one of each is not a quantity.
    /// @tparam U1 Left unit type, whose unit the result is expressed in.
    /// @tparam U2 Right unit type.
    /// @param lhs One side.
    /// @param rhs The other side.
    /// @return The hypotenuse, expressed in the unit of @p lhs.
    template <typename U1, typename U2>
    [[nodiscard]] auto hypot(const U1& lhs, const U2& rhs) noexcept
        requires(product_units<U1, U2> && same_quantity<U1, U2>)
    {
        using value_t = arithmetic_t<U1, U2>;
        using result_t = typename U1::template rebind_t<value_t>;
        return result_t(std::hypot(static_cast<value_t>(lhs.as_native()), detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native()))));
    }

    /// @brief The square root of a value, whose dimension is halved.
    /// @details The dimension exponents are std::ratio values, so the root of a dimension that does not
    /// halve evenly is still exact: the square root of a volume is a length to the power 3/2.
    /// @tparam Unit The unit type.
    /// @param value The value.
    /// @return The root, as a kmx::unit::si_unit of the halved dimension.
    template <typename Unit>
    [[nodiscard]] auto sqrt(const Unit& value) noexcept
        requires(unit_type<Unit> && !is_absolute_scale_v<Unit>)
    {
        using value_t = computation_t<typename Unit::value_t>;
        using result_dimension_t = dimension::root_t<typename Unit::dimension_t, 2>;
        return si_unit<result_dimension_t, value_t>(std::sqrt(value.template as_si<value_t>()));
    }

    /// @brief The cube root of a value, whose dimension is divided by three.
    /// @tparam Unit The unit type.
    /// @param value The value.
    /// @return The root, as a kmx::unit::si_unit of the divided dimension.
    template <typename Unit>
    [[nodiscard]] auto cbrt(const Unit& value) noexcept
        requires(unit_type<Unit> && !is_absolute_scale_v<Unit>)
    {
        using value_t = computation_t<typename Unit::value_t>;
        using result_dimension_t = dimension::root_t<typename Unit::dimension_t, 3>;
        return si_unit<result_dimension_t, value_t>(std::cbrt(value.template as_si<value_t>()));
    }

    /// @brief A value raised to a compile-time integer power, whose dimension is raised with it.
    /// @tparam Exponent The power to raise to; it may be negative.
    /// @tparam Unit The unit type.
    /// @param value The value.
    /// @return The power, as a kmx::unit::si_unit of the raised dimension.
    template <std::intmax_t Exponent, typename Unit>
    [[nodiscard]] constexpr auto pow(const Unit& value) noexcept
        requires(unit_type<Unit> && !is_absolute_scale_v<Unit>)
    {
        using value_t = computation_t<typename Unit::value_t>;
        using result_dimension_t = dimension::raise_t<typename Unit::dimension_t, Exponent>;
        return si_unit<result_dimension_t, value_t>(detail::integer_power<Exponent>(value.template as_si<value_t>()));
    }
}
