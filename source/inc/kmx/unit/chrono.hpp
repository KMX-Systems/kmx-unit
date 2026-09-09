/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/chrono.hpp
/// @brief The conversion between the time units of this library and std::chrono::duration.
#pragma once
#ifndef PCH
    #include <kmx/unit/duration.hpp>
    #include <chrono>
#endif

namespace kmx::unit
{
    // kmx::unit::time_unit is declared in kmx/unit/duration.hpp, which this header includes. A concept
    // cannot be redeclared, identically or otherwise, so there is exactly one declaration of it.

    /// @brief Builds a unit value of this library from a std::chrono::duration.
    /// @details The tick period of a std::chrono::duration is a std::ratio of seconds, so the conversion
    /// factor is known at compile time and the result costs one multiplication.
    /// @tparam TargetUnit The unit of the result.
    /// @tparam Rep The arithmetic type of the duration.
    /// @tparam Period The tick period of the duration, a std::ratio of seconds.
    /// @param value The duration to convert.
    /// @return The duration expressed in @p TargetUnit.
    template <typename TargetUnit = duration::second<double>, typename Rep, typename Period>
    [[nodiscard]] constexpr TargetUnit from_chrono(const std::chrono::duration<Rep, Period>& value) noexcept
        requires time_unit<TargetUnit>
    {
        using compute_t = computation_t<typename TargetUnit::value_t>;
        constexpr auto seconds_per_tick = static_cast<compute_t>(Period::num) / static_cast<compute_t>(Period::den);
        return from_si<TargetUnit>(static_cast<compute_t>(value.count()) * seconds_per_tick);
    }

    /// @brief Builds an integral unit value from std::chrono, returning no value when it cannot be held.
    /// @details This is the checked counterpart to kmx::unit::from_chrono. It returns no value for NaN,
    /// infinity or a result outside the target value type's range, while preserving normal rounding toward
    /// zero for the existing conversion path.
    /// @tparam TargetUnit The integral unit of the result.
    /// @tparam Rep The arithmetic type of the duration.
    /// @tparam Period The tick period of the duration, a std::ratio of seconds.
    /// @param value The duration to convert.
    /// @return The converted duration, or std::nullopt when the result is not representable.
    template <typename TargetUnit = duration::second<double>, typename Rep, typename Period>
    [[nodiscard]] constexpr std::optional<TargetUnit> checked_from_chrono(
        const std::chrono::duration<Rep, Period>& value) noexcept
        requires time_unit<TargetUnit>
    {
        static_assert(std::is_integral_v<typename TargetUnit::value_t>,
                      "checked_from_chrono requires an integral target value type.");
        using compute_t = computation_t<typename TargetUnit::value_t>;
        constexpr auto seconds_per_tick = static_cast<compute_t>(Period::num) / static_cast<compute_t>(Period::den);
        using inverse_t = scale::quotient<scale::one, typename TargetUnit::magnitude_t>;
        constexpr auto inverse_factor = static_cast<compute_t>(inverse_t::value);
        constexpr auto offset = static_cast<compute_t>(TargetUnit::offset_to_si);
        auto native = static_cast<compute_t>(value.count()) * seconds_per_tick;
        if constexpr (offset != static_cast<compute_t>(0))
            native -= offset;

        if constexpr (inverse_factor != static_cast<compute_t>(1))
            native *= inverse_factor;

        return detail::checked_integral_value<TargetUnit>(native);
    }

    /// @brief Converts a time value of this library into a std::chrono::duration.
    /// @details The default result is a duration of `double` seconds, which represents every value of the
    /// family without loss. Asking for an integral duration such as std::chrono::milliseconds truncates,
    /// exactly as a narrowing std::chrono::duration_cast does.
    /// @tparam ChronoDuration The std::chrono::duration type of the result.
    /// @tparam SourceUnit The unit of the source, deduced.
    /// @param value The value to convert.
    /// @return The value as a std::chrono::duration.
    template <typename ChronoDuration = std::chrono::duration<double>, typename SourceUnit>
    [[nodiscard]] constexpr ChronoDuration to_chrono(const SourceUnit& value) noexcept
        requires time_unit<SourceUnit>
    {
        using rep_t = typename ChronoDuration::rep;
        using period_t = typename ChronoDuration::period;
        using compute_t = computation_t<rep_t>;
        constexpr auto ticks_per_second = static_cast<compute_t>(period_t::den) / static_cast<compute_t>(period_t::num);
        return ChronoDuration(static_cast<rep_t>(value.template as_si<compute_t>() * ticks_per_second));
    }

    /// @brief Converts a time value into std::chrono, returning no value when the target rep cannot hold it.
    /// @details This is the checked counterpart to kmx::unit::to_chrono. It preserves duration_cast-style
    /// truncation for integral reps, while rejecting NaN, infinity and values outside the target rep range.
    /// @tparam ChronoDuration The std::chrono::duration type of the result.
    /// @tparam SourceUnit The source time unit, deduced.
    /// @param value The value to convert.
    /// @return The converted duration, or std::nullopt when the result is not representable.
    template <typename ChronoDuration = std::chrono::duration<double>, typename SourceUnit>
    [[nodiscard]] constexpr std::optional<ChronoDuration> checked_to_chrono(const SourceUnit& value) noexcept
        requires time_unit<SourceUnit>
    {
        using rep_t = typename ChronoDuration::rep;
        static_assert(std::is_arithmetic_v<rep_t>, "checked_to_chrono requires an arithmetic duration rep.");
        using period_t = typename ChronoDuration::period;
        using compute_t = computation_t<rep_t>;
        constexpr auto ticks_per_second = static_cast<compute_t>(period_t::den) / static_cast<compute_t>(period_t::num);
        const auto ticks = value.template as_si<compute_t>() * ticks_per_second;
        if constexpr (std::is_integral_v<rep_t>)
        {
            using tick_unit_t = typename SourceUnit::template rebind_t<rep_t>;
            const auto checked_ticks = detail::checked_integral_value<tick_unit_t>(ticks);
            if (!checked_ticks)
                return std::nullopt;

            return ChronoDuration(checked_ticks->as_native());
        }
        else
        {
            if (!std::isfinite(ticks))
                return std::nullopt;
            return ChronoDuration(static_cast<rep_t>(ticks));
        }
    }
}
