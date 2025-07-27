/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/data_rate.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/information.hpp>
#endif

namespace kmx::unit::data_rate
{
    /// @brief A unique dimension tag for units of data rate (information/time).
    struct dimension
    {
    };

    // Base Rate (bits per second)
    template <typename T = double>
    struct bits_per_second: base<bits_per_second<T>, dimension, T>
    {
        using base<bits_per_second<T>, dimension, T>::base;

        template <typename U>
        using rebind = bits_per_second<U>;
    };

    // Decimal
    template <typename T = double>
    struct megabits_per_second: base<megabits_per_second<T>, dimension, T, static_cast<double>(information::mega)>
    {
        using base<megabits_per_second<T>, dimension, T, static_cast<double>(information::mega)>::base;
        template <typename U>
        using rebind = megabits_per_second<U>;
    };

    template <typename T = double>
    struct megabytes_per_second:
        base<megabytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::mega)>
    {
        using base<megabytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::mega)>::base;
        template <typename U>
        using rebind = megabytes_per_second<U>;
    };

    template <typename T = double>
    struct gigabits_per_second: base<gigabits_per_second<T>, dimension, T, static_cast<double>(information::giga)>
    {
        using base<gigabits_per_second<T>, dimension, T, static_cast<double>(information::giga)>::base;
        template <typename U>
        using rebind = gigabits_per_second<U>;
    };

    template <typename T = double>
    struct gigabytes_per_second:
        base<gigabytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::giga)>
    {
        using base<gigabytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::giga)>::base;
        template <typename U>
        using rebind = gigabytes_per_second<U>;
    };

    // Binary
    template <typename T = double>
    struct mebibytes_per_second:
        base<mebibytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::mebi)>
    {
        using base<mebibytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::mebi)>::base;
        template <typename U>
        using rebind = mebibytes_per_second<U>;
    };

    template <typename T = double>
    struct gibibytes_per_second:
        base<gibibytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::gibi)>
    {
        using base<gibibytes_per_second<T>, dimension, T, static_cast<double>(information::bits_in_byte* information::gibi)>::base;
        template <typename U>
        using rebind = gibibytes_per_second<U>;
    };
}

namespace kmx::unit
{
    KMX_UNIT_FACTORY_FUNCTIONS(_bps, data_rate::bits_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_Mbps, data_rate::megabits_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_MBps, data_rate::megabytes_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_Gbps, data_rate::gigabits_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_GBps, data_rate::gigabytes_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_MiBps, data_rate::mebibytes_per_second)
    KMX_UNIT_FACTORY_FUNCTIONS(_GiBps, data_rate::gibibytes_per_second)

    // SPECIALIZED CROSS-DIMENSION OPERATOR (Information / Time)
    template <typename InfoUnit, typename TimeUnit>
    [[nodiscard]] constexpr auto operator/(const InfoUnit& lhs, const TimeUnit& rhs) noexcept
        requires(std::is_same_v<typename InfoUnit::dimension, information::dimension> &&
                 std::is_same_v<typename TimeUnit::dimension, dimension::time>)
    {
        using result_value_type = std::common_type_t<typename InfoUnit::value_type, typename TimeUnit::value_type, double>;

        // Data rate is always in terms of [information] per second.
        // The value is (info_in_bits) / (time_in_seconds)
        const auto val_bps = lhs.template as_si<result_value_type>() / rhs.template as_si<result_value_type>();

        return data_rate::bits_per_second<result_value_type>(val_bps);
    }
}
