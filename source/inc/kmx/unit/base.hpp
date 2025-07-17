/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/base.hpp
#pragma once
#ifndef PCH
    #include <cmath>
    #include <cstdint>
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

namespace kmx::unit
{
    /// @brief Calculates the relative factor and offset for direct unit-to-unit conversion at compile time.
    /// @tparam TargetUnit The unit to convert to.
    /// @tparam SourceUnit The unit to convert from.
    template <typename TargetUnit, typename SourceUnit>
    struct relative_conversion
    {
        static_assert(std::is_same_v<typename TargetUnit::dimension, typename SourceUnit::dimension>,
                      "Cannot create a relative conversion between different dimensions.");

        // The compile-time calculated factor to multiply the source value by.
        static constexpr double factor = SourceUnit::factor_to_si / TargetUnit::factor_to_si;

        // The compile-time calculated offset to add after multiplying.
        static constexpr double offset = (SourceUnit::offset_to_si - TargetUnit::offset_to_si) / TargetUnit::factor_to_si;
    };

    /// @brief The generic CRTP base for all units.
    /// @details This class stores the unit's value in its NATIVE representation for performance.
    ///          All arithmetic and comparison operations are performed by converting to a common
    ///          SI representation on the fly.
    template <typename DerivedUnit, typename Dimension, typename ValueType, double FactorToSI = 1.0, double OffsetToSI = 0.0>
    struct base
    {
        using value_type = ValueType;
        using dimension = Dimension;

        static constexpr double factor_to_si = FactorToSI;
        static constexpr double offset_to_si = OffsetToSI;

        value_type value {}; // native

        constexpr base() noexcept = default;
        [[nodiscard]] constexpr explicit base(const value_type val_native) noexcept: value(val_native) {}

        [[nodiscard]] constexpr value_type as_native() const noexcept { return value; }

        template <typename T = double>
        [[nodiscard]] constexpr T as_si() const noexcept
        {
            return static_cast<T>(value) * factor_to_si + offset_to_si;
        }

        [[nodiscard]] constexpr DerivedUnit operator+() const noexcept { return DerivedUnit(+value); }
        [[nodiscard]] constexpr DerivedUnit operator-() const noexcept { return DerivedUnit(-value); }

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator+(const OtherUnit& other) const noexcept
        {
            // Promote result type to avoid precision loss and determine the result unit type
            using result_value_type = std::common_type_t<value_type, typename OtherUnit::value_type, double>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;

            // Get the pre-calculated relative conversion values
            constexpr auto conv = relative_conversion<result_unit_type, OtherUnit>();

            // Convert 'other' to the native representation of the result type and add
            const auto result_native =
                static_cast<result_value_type>(this->value) + (static_cast<result_value_type>(other.value) * conv.factor + conv.offset);

            return result_unit_type(result_native);
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator-(const OtherUnit& other) const noexcept
        {
            using result_value_type = std::common_type_t<value_type, typename OtherUnit::value_type, double>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;

            constexpr auto conv = relative_conversion<result_unit_type, OtherUnit>();

            const auto result_native =
                static_cast<result_value_type>(this->value) - (static_cast<result_value_type>(other.value) * conv.factor + conv.offset);

            return result_unit_type(result_native);
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator*(const OtherUnit& other) const noexcept
        {
            using result_value_type = std::common_type_t<value_type, typename OtherUnit::value_type, double>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;

            // Get the pre-calculated relative conversion values
            constexpr auto conv = relative_conversion<result_unit_type, OtherUnit>();

            // Convert 'other' to the native representation of the result type and add
            const auto result_native =
                static_cast<result_value_type>(this->value) * (static_cast<result_value_type>(other.value) * conv.factor + conv.offset);

            return result_unit_type(result_native);
        }

        template <typename OtherUnit>
        [[nodiscard]] constexpr auto operator/(const OtherUnit& other) const noexcept
        {
            using result_value_type = std::common_type_t<value_type, typename OtherUnit::value_type, double>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;

            constexpr auto conv = relative_conversion<result_unit_type, OtherUnit>();

            const auto result_native =
                static_cast<result_value_type>(this->value) / (static_cast<result_value_type>(other.value) * conv.factor + conv.offset);

            return result_unit_type(result_native);
        }

        // Performed in the native domain for performance. The underlying value_type is promoted.
        template <typename Scalar>
        [[nodiscard]] constexpr auto operator*(const Scalar scalar) const noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>, "Can only multiply a unit by a scalar (arithmetic) value.");
            using result_value_type = std::common_type_t<value_type, Scalar>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;
            return result_unit_type(static_cast<result_value_type>(value) * scalar);
        }

        template <typename Scalar>
        [[nodiscard]] constexpr auto operator/(const Scalar scalar) const noexcept
        {
            static_assert(std::is_arithmetic_v<Scalar>, "Can only divide a unit by a scalar (arithmetic) value.");
            using result_value_type = std::common_type_t<value_type, Scalar>;
            using result_unit_type = typename DerivedUnit::template rebind<result_value_type>;
            return result_unit_type(static_cast<result_value_type>(value) / scalar);
        }

        template <typename OtherUnit>
        constexpr DerivedUnit& operator+=(const OtherUnit& other) noexcept
        {
            static_assert(std::is_same_v<dimension, typename OtherUnit::dimension>);
            *this = *this + other;
            return static_cast<DerivedUnit&>(*this);
        }

        template <typename OtherUnit>
        constexpr DerivedUnit& operator-=(const OtherUnit& other) noexcept
        {
            static_assert(std::is_same_v<dimension, typename OtherUnit::dimension>);
            *this = *this - other;
            return static_cast<DerivedUnit&>(*this);
        }

        constexpr DerivedUnit& operator*=(const value_type scalar) noexcept
        {
            value *= scalar;
            return static_cast<DerivedUnit&>(*this);
        }

        constexpr DerivedUnit& operator/=(const value_type scalar) noexcept
        {
            value /= scalar;
            return static_cast<DerivedUnit&>(*this);
        }

        // Performed by converting both operands to SI on the fly.
        [[nodiscard]] constexpr auto operator<=>(const auto& other) const noexcept
            requires(std::is_same_v<dimension, typename std::decay_t<decltype(other)>::dimension>)
        {
            return this->as_si() <=> other.as_si();
        }

        [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept { return this->as_si() == other.as_si(); }
        [[nodiscard]] constexpr bool operator!=(const auto& other) const noexcept { return this->as_si() != other.as_si(); }
    };

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
