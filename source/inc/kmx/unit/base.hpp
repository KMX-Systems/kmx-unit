/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/base.hpp
#pragma once
#ifndef PCH
    #include <cmath>
    #include <cstdint>
    #include <ratio>
    #include <string_view>
    #include <type_traits>
#endif

// The factory macro is unchanged, but now creates units with their native value.
#define KMX_UNIT_FACTORY_FUNCTIONS(func_name, type_name)                     \
    [[nodiscard]] constexpr auto func_name(const double val) noexcept        \
    {                                                                        \
        return type_name<double>(val);                                       \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const long double val) noexcept   \
    {                                                                        \
        return type_name<long double>(val);                                  \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const float val) noexcept         \
    {                                                                        \
        return type_name<float>(val);                                        \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::int8_t val) noexcept   \
    {                                                                        \
        return type_name<std::int8_t>(val);                                  \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::uint8_t val) noexcept  \
    {                                                                        \
        return type_name<std::uint8_t>(val);                                 \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::int16_t val) noexcept  \
    {                                                                        \
        return type_name<std::int16_t>(val);                                 \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::uint16_t val) noexcept \
    {                                                                        \
        return type_name<std::uint16_t>(val);                                \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::int32_t val) noexcept  \
    {                                                                        \
        return type_name<std::int32_t>(val);                                 \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::uint32_t val) noexcept \
    {                                                                        \
        return type_name<std::uint32_t>(val);                                \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::int64_t val) noexcept  \
    {                                                                        \
        return type_name<std::int64_t>(val);                                 \
    }                                                                        \
    [[nodiscard]] constexpr auto func_name(const std::uint64_t val) noexcept \
    {                                                                        \
        return type_name<std::uint64_t>(val);                                \
    }

namespace kmx::unit::dimension
{
    /// @brief Represents the dimension of a unit as a product of powers of the 7 base SI dimensions.
    /// @details The template parameters are std::ratio types representing the exponent for each base dimension.
    template <typename L = std::ratio<0>,     // Length
              typename M = std::ratio<0>,     // Mass
              typename T = std::ratio<0>,     // Time
              typename I = std::ratio<0>,     // Electric Current
              typename Theta = std::ratio<0>, // Thermodynamic Temperature
              typename N = std::ratio<0>,     // Amount of Substance
              typename J = std::ratio<0>      // Luminous Intensity
              >
    struct item
    {
    };

    // Helper to extract the dimension parameters from a dimension type.
    template <typename>
    struct params;

    template <typename L, typename M, typename T, typename I, typename Th, typename N, typename J>
    struct params<item<L, M, T, I, Th, N, J>>
    {
        using l = L;
        using m = M;
        using t = T;
        using i = I;
        using th = Th;
        using n = N;
        using j = J;
    };

    /// @brief A type alias for multiplying two dimensions (adds their exponents).
    template <typename D1, typename D2>
    using multiply =
        item<std::ratio_add<typename params<D1>::l, typename params<D2>::l>, std::ratio_add<typename params<D1>::m, typename params<D2>::m>,
             std::ratio_add<typename params<D1>::t, typename params<D2>::t>, std::ratio_add<typename params<D1>::i, typename params<D2>::i>,
             std::ratio_add<typename params<D1>::th, typename params<D2>::th>,
             std::ratio_add<typename params<D1>::n, typename params<D2>::n>,
             std::ratio_add<typename params<D1>::j, typename params<D2>::j>>;

    /// @brief A type alias for dividing two dimensions (subtracts their exponents).
    template <typename D1, typename D2>
    using divide = item<std::ratio_subtract<typename params<D1>::l, typename params<D2>::l>,
                        std::ratio_subtract<typename params<D1>::m, typename params<D2>::m>,
                        std::ratio_subtract<typename params<D1>::t, typename params<D2>::t>,
                        std::ratio_subtract<typename params<D1>::i, typename params<D2>::i>,
                        std::ratio_subtract<typename params<D1>::th, typename params<D2>::th>,
                        std::ratio_subtract<typename params<D1>::n, typename params<D2>::n>,
                        std::ratio_subtract<typename params<D1>::j, typename params<D2>::j>>;

    // Base and Common Dimension Type Aliases
    using dimensionless = item<>;
    using length = item<std::ratio<1>>;
    using mass = item<std::ratio<0>, std::ratio<1>>;
    using time = item<std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using current = item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using temperature = item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using luminous_intensity =
        item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;

    // Derived Dimension Type Aliases
    using area = multiply<length, length>;        // L²
    using volume = multiply<area, length>;        // L³
    using velocity = divide<length, time>;        // L/T
    using acceleration = divide<velocity, time>;  // L/T²
    using force = multiply<mass, acceleration>;   // M·L/T²
    using pressure = divide<force, area>;         // M/L·T²
    using energy = multiply<force, length>;       // M·L²/T²
    using power = divide<energy, time>;           // M·L²/T³
    using angle = dimensionless;                  // Angles are fundamentally dimensionless (ratio of arc length to radius)
    using angular_velocity = divide<angle, time>; // 1/T
    using charge = multiply<current, time>;       // I·T
    using voltage = divide<energy, charge>;       // M·L²/I·T³
    using resistance = divide<voltage, current>;  // M·L²/I²·T³
    using capacitance = divide<charge, voltage>;  // I²·T⁴/M·L²
    using inductance = divide<multiply<energy, time>, multiply<charge, charge>>; // M·L²/I²·T²
    using illuminance = divide<luminous_intensity, area>;
}

namespace kmx::unit
{
    /// @brief The generic CRTP base for all units.
    /// @details This class stores the unit's value in its NATIVE representation for performance.
    /// Arithmetic between same-dimension units is optimized to avoid SI round-trips.
    /// Comparison and cross-dimension arithmetic are performed via a common SI representation.
    template <typename DerivedUnit, typename Dimension, typename ValueType, double FactorToSI = 1.0, double OffsetToSI = 0.0>
    struct base
    {
        using value_type = ValueType;
        using dimension = Dimension;
        static constexpr double factor_to_si = FactorToSI;
        static constexpr double offset_to_si = OffsetToSI;

        value_type value {};

        constexpr base() noexcept = default;
        [[nodiscard]] constexpr explicit base(const value_type val_native) noexcept: value(val_native) {}

        [[nodiscard]] constexpr auto as_native() const noexcept { return value; }

        template <typename T = double>
        [[nodiscard]] constexpr T as_si() const noexcept
        {
            return static_cast<T>(as_native()) * static_cast<T>(factor_to_si) + static_cast<T>(offset_to_si);
        }

        [[nodiscard]] constexpr DerivedUnit operator+() const noexcept { return DerivedUnit(+value); }
        [[nodiscard]] constexpr DerivedUnit operator-() const noexcept { return DerivedUnit(-value); }

        // SCALAR ARITHMETIC
        template <typename Scalar>
        [[nodiscard]] constexpr auto operator*(const Scalar scalar) const noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>, "Can only multiply a unit by a scalar value.");
            using result_value_type = std::common_type_t<value_type, Scalar>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;
            return result_unit_type(static_cast<result_value_type>(value) * scalar);
        }

        template <typename Scalar>
        [[nodiscard]] constexpr auto operator/(const Scalar scalar) const noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>, "Can only divide a unit by a scalar value.");
            using result_value_type = std::common_type_t<value_type, Scalar>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;
            return result_unit_type(static_cast<result_value_type>(value) / scalar);
        }

        // COMPOUND ASSIGNMENT (SCALAR)
        template <typename Scalar>
        constexpr DerivedUnit& operator*=(const Scalar scalar) noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>);
            if constexpr (std::is_convertible_v<Scalar, value_type>)
                value *= scalar;
            else
                *this = (*this * scalar);

            return static_cast<DerivedUnit&>(*this);
        }

        template <typename Scalar>
        constexpr DerivedUnit& operator/=(const Scalar scalar) noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>);
            if constexpr (std::is_convertible_v<Scalar, value_type>)
                value /= scalar;
            else
                *this = (*this / scalar);

            return static_cast<DerivedUnit&>(*this);
        }

        // COMPARISON

        [[nodiscard]] constexpr auto operator<=>(const base& other) const noexcept { return this->value <=> other.value; }

        [[nodiscard]] constexpr bool operator!=(const base& other) const noexcept { return this->value != other.value; }

        [[nodiscard]] constexpr bool operator==(const base& other) const noexcept { return this->value == other.value; }

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator<=>(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->as_si() <=> other.as_si();
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr bool operator!=(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->as_si() != other.as_si();
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr bool operator==(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->as_si() == other.as_si();
        }
    };

    /// @brief A generic unit type representing a value in base SI units.
    /// @details This is the return type for cross-dimensional arithmetic.
    template <typename Dimension, typename T = double>
    struct si_unit: base<si_unit<Dimension, T>, Dimension, T>
    {
        using dimension = Dimension;

        using base<si_unit<Dimension, T>, Dimension, T>::base;

        template <typename U>
        using rebind = si_unit<Dimension, U>;

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator<=>(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->value <=> other.as_si();
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr bool operator!=(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->value != other.as_si();
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr bool operator==(const OtherUnit& other) const noexcept
            requires(std::is_same_v<dimension, typename OtherUnit::dimension>)
        {
            return this->value == other.as_si();
        }
    };

    /// @brief Calculates the relative factor and offset for direct unit-to-unit conversion at compile time.
    template <typename TargetUnit, typename SourceUnit>
    struct relative_conversion
    {
        static_assert(std::is_same_v<typename TargetUnit::dimension, typename SourceUnit::dimension>);
        static constexpr double factor = SourceUnit::factor_to_si / TargetUnit::factor_to_si;
        static constexpr double offset = (SourceUnit::offset_to_si - TargetUnit::offset_to_si) / TargetUnit::factor_to_si;
    };

    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator+(const U1& lhs, const U2& rhs) noexcept
        requires(std::is_same_v<typename U1::dimension, typename U2::dimension>)
    {
        using result_value_type = std::common_type_t<typename U1::value_type, typename U2::value_type, double>;
        using result_unit_type = typename U1::template rebind<result_value_type>;
        constexpr auto conv = relative_conversion<result_unit_type, U2>();
        const auto result_native =
            static_cast<result_value_type>(lhs.as_native()) + (static_cast<result_value_type>(rhs.as_native()) * conv.factor + conv.offset);
        return result_unit_type(result_native);
    }

    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator-(const U1& lhs, const U2& rhs) noexcept
        requires(std::is_same_v<typename U1::dimension, typename U2::dimension>)
    {
        using result_value_type = std::common_type_t<typename U1::value_type, typename U2::value_type, double>;
        using result_unit_type = typename U1::template rebind<result_value_type>;
        constexpr auto conv = relative_conversion<result_unit_type, U2>();
        const auto result_native =
            static_cast<result_value_type>(lhs.as_native()) - (static_cast<result_value_type>(rhs.as_native()) * conv.factor + conv.offset);
        return result_unit_type(result_native);
    }

    // CROSS-DIMENSION ARITHMETIC (MULTIPLICATION/DIVISION)
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator*(const U1& lhs, const U2& rhs) noexcept
    {
        using result_dimension = dimension::multiply<typename U1::dimension, typename U2::dimension>;
        using result_value_type = std::common_type_t<typename U1::value_type, typename U2::value_type, double>;
        const auto result_si_value = lhs.template as_si<result_value_type>() * rhs.template as_si<result_value_type>();
        return si_unit<result_dimension, result_value_type>(result_si_value);
    }

    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator/(const U1& lhs, const U2& rhs) noexcept
    {
        using result_dimension = dimension::divide<typename U1::dimension, typename U2::dimension>;
        using result_value_type = std::common_type_t<typename U1::value_type, typename U2::value_type, double>;
        const auto result_si_value = lhs.template as_si<result_value_type>() / rhs.template as_si<result_value_type>();
        return si_unit<result_dimension, result_value_type>(result_si_value);
    }

    // Commutative scalar multiplication
    template <typename Scalar, typename DerivedUnit, typename Dimension, typename ValueType, double FactorToSI, double OffsetToSI>
    [[nodiscard]] constexpr auto operator*(const Scalar scalar,
                                           const base<DerivedUnit, Dimension, ValueType, FactorToSI, OffsetToSI>& unit) noexcept
    {
        static_assert(std::is_arithmetic_v<Scalar>);
        return unit * scalar;
    }

    // Conversion
    template <typename TargetUnit, typename SourceUnit>
    [[nodiscard]] constexpr TargetUnit convert(const SourceUnit& source) noexcept
    {
        static_assert(std::is_same_v<typename TargetUnit::dimension, typename SourceUnit::dimension>);
        const auto source_si = source.as_si();
        const auto target_native = (source_si - TargetUnit::offset_to_si) / TargetUnit::factor_to_si;
        return TargetUnit(static_cast<typename TargetUnit::value_type>(target_native));
    }

    template <typename TargetUnit, typename SourceUnit, typename FloatType = double>
    [[nodiscard]] constexpr TargetUnit fp_convert(const SourceUnit& source) noexcept
    {
        static_assert(std::is_floating_point_v<FloatType>);
        static_assert(std::is_same_v<typename TargetUnit::dimension, typename SourceUnit::dimension>);
        const FloatType source_si = source.template as_si<FloatType>();
        const FloatType target_native =
            (source_si - static_cast<FloatType>(TargetUnit::offset_to_si)) / static_cast<FloatType>(TargetUnit::factor_to_si);
        return TargetUnit(static_cast<typename TargetUnit::value_type>(std::round(target_native)));
    }
}
