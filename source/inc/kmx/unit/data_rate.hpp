/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/data_rate.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/duration.hpp>
    #include <kmx/unit/information.hpp>
#endif

/// @brief Units of data rate. The base unit of the family is the bit per second.
/// @details A rate is information divided by time, and the dimension of the family says exactly that,
/// so a rate arises from an ordinary division and yields information back when multiplied by a time.
namespace kmx::unit::data_rate
{
    /// @brief The dimension of the family: information divided by time, whose base unit is the bit per second.
    /// @details It is an alias of kmx::unit::dimension::data_rate_t rather than a tag of its own, so a rate is
    /// an ordinary quotient of the two dimensions it is built from and needs no special case to behave.
    using dimension_t = ::kmx::unit::dimension::data_rate_t;

    /// @brief The base unit of the family, one bit transferred per second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(bits_per_second, dimension_t, scale::one, "bps")

    /// @brief One million bits per second, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megabits_per_second, dimension_t, scale::mega, "Mbps")

    /// @brief One million bytes per second, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megabytes_per_second, dimension_t, information::bytes_of<scale::mega>, "MBps")

    /// @brief One thousand million bits per second, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigabits_per_second, dimension_t, scale::giga, "Gbps")

    /// @brief One thousand million bytes per second, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigabytes_per_second, dimension_t, information::bytes_of<scale::giga>, "GBps")

    /// @brief One mebibyte, that is 1048576 bytes, per second; the binary prefix of the IEC.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(mebibytes_per_second, dimension_t, information::bytes_of<scale::mebi>, "MiBps")

    /// @brief One gibibyte, that is 1073741824 bytes, per second; the binary prefix of the IEC.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gibibytes_per_second, dimension_t, information::bytes_of<scale::gibi>, "GiBps")
}

namespace kmx::unit
{
    /// @brief Satisfied by the units measuring an amount of information.
    /// @tparam U Candidate type.
    template <typename U>
    concept information_unit = unit_type<U> && std::is_same_v<typename U::dimension_t, dimension::information_t>;

    /// @brief Satisfied by the units measuring an amount of information per unit of time.
    /// @tparam U Candidate type.
    template <typename U>
    concept data_rate_unit = unit_type<U> && std::is_same_v<typename U::dimension_t, dimension::data_rate_t>;

    // The three operators below are the generic cross-dimension arithmetic of kmx/unit/base.hpp, narrowed
    // to report a named unit of this family rather than an anonymous kmx::unit::si_unit. They compute the
    // same value; only the type of the result differs, so that it streams and formats as "bps", "b" or "s".
    // Their constraints include kmx::unit::product_units, which is what makes them subsume the generic
    // operators and win overload resolution rather than tie with them.

    /// @brief Divides an amount of information by a time.
    /// @param lhs The amount of information.
    /// @param rhs The time.
    /// @return The rate, in bits per second.
    template <typename InfoUnit, typename TimeUnit>
    [[nodiscard]] constexpr auto operator/(const InfoUnit& lhs, const TimeUnit& rhs) noexcept
        requires(product_units<InfoUnit, TimeUnit> && information_unit<InfoUnit> && time_unit<TimeUnit>)
    {
        using result_value_t = std::common_type_t<typename InfoUnit::value_t, typename TimeUnit::value_t, double>;
        const auto value_bps = lhs.template as_si<result_value_t>() / rhs.template as_si<result_value_t>();
        return data_rate::bits_per_second<result_value_t>(value_bps);
    }

    /// @brief Divides an amount of information by a rate, that is the time such a transfer takes.
    /// @param lhs The amount of information.
    /// @param rhs The rate.
    /// @return The time, in seconds.
    template <typename InfoUnit, typename RateUnit>
    [[nodiscard]] constexpr auto operator/(const InfoUnit& lhs, const RateUnit& rhs) noexcept
        requires(product_units<InfoUnit, RateUnit> && information_unit<InfoUnit> && data_rate_unit<RateUnit>)
    {
        using result_value_t = std::common_type_t<typename InfoUnit::value_t, typename RateUnit::value_t, double>;
        const auto value_seconds = lhs.template as_si<result_value_t>() / rhs.template as_si<result_value_t>();
        return duration::second<result_value_t>(value_seconds);
    }

    /// @brief Multiplies a rate by a time, that is the amount such a transfer carries.
    /// @param lhs The rate.
    /// @param rhs The time.
    /// @return The amount of information, in bits.
    template <typename RateUnit, typename TimeUnit>
    [[nodiscard]] constexpr auto operator*(const RateUnit& lhs, const TimeUnit& rhs) noexcept
        requires(product_units<RateUnit, TimeUnit> && data_rate_unit<RateUnit> && time_unit<TimeUnit>)
    {
        using result_value_t = std::common_type_t<typename RateUnit::value_t, typename TimeUnit::value_t, double>;
        const auto value_bits = lhs.template as_si<result_value_t>() * rhs.template as_si<result_value_t>();
        return information::bit<result_value_t>(value_bits);
    }

    /// @brief Multiplies a time by a rate; see the commutative overload.
    /// @param lhs The time.
    /// @param rhs The rate.
    /// @return The amount of information, in bits.
    template <typename TimeUnit, typename RateUnit>
    [[nodiscard]] constexpr auto operator*(const TimeUnit& lhs, const RateUnit& rhs) noexcept
        requires(product_units<TimeUnit, RateUnit> && time_unit<TimeUnit> && data_rate_unit<RateUnit>)
    {
        return rhs * lhs;
    }
}

/// @brief The literal suffixes building data rate values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(bps, unit::data_rate::bits_per_second)
    KMX_UNIT_LITERALS(Mbps, unit::data_rate::megabits_per_second)
    KMX_UNIT_LITERALS(MBps, unit::data_rate::megabytes_per_second)
    KMX_UNIT_LITERALS(Gbps, unit::data_rate::gigabits_per_second)
    KMX_UNIT_LITERALS(GBps, unit::data_rate::gigabytes_per_second)
    KMX_UNIT_LITERALS(MiBps, unit::data_rate::mebibytes_per_second)
    KMX_UNIT_LITERALS(GiBps, unit::data_rate::gibibytes_per_second)
}
