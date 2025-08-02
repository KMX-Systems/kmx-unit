/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/base.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <ostream>
#endif

namespace kmx::unit::detail
{
    // SFINAE helper to detect if a unit has a `text` member
    template <typename T, typename = void>
    struct has_text: std::false_type
    {
    };

    template <typename T>
    struct has_text<T, std::void_t<decltype(T::text)>>: std::true_type
    {
    };

    // Helper to format an exponent if it is not 1.
    template <typename Ratio>
    void stream_exponent(std::ostream& os)
    {
        if constexpr ((Ratio::num != 1) || (Ratio::den != 1))
        {
            os << '^' << Ratio::num;
            if constexpr (Ratio::den != 1)
                os << '/' << Ratio::den;
        }
    }

    // Helper to stream a single dimensional component (e.g., "kg", "m^2", "s^-1").
    template <typename Ratio>
    void stream_component(std::ostream& os, std::string_view symbol, bool& is_first)
    {
        if constexpr (Ratio::num != 0)
        {
            if (!is_first)
                os << "·"; // Use a middle dot as a separator

            os << symbol;
            stream_exponent<Ratio>(os);
            is_first = false;
        }
    }
}

namespace kmx::unit
{
    /// @brief Provides a string representation for any named unit type.
    template <typename Unit>
    auto operator<<(std::ostream& os, const Unit& unit) -> std::enable_if_t<detail::has_text<Unit>::value, std::ostream&>
    {
        return os << unit.as_native() << " " << Unit::text;
    }

    /// @brief Provides a descriptive, symbolic string representation for a generic SI unit.
    template <typename Dimension, typename T>
    std::ostream& operator<<(std::ostream& os, const si_unit<Dimension, T>& unit)
    {
        os << unit.as_native();

        // Check if the unit is dimensionless
        if constexpr (std::is_same_v<Dimension, dimension::dimensionless>)
            return os; // Just print the number

        os << ' '; // Add a space after the number
        bool is_first_component = true;
        using P = dimension::params<Dimension>;

        // Stream components in the conventional order (kg, m, s, A, K, mol, cd)
        detail::stream_component<typename P::m>(os, "kg", is_first_component);
        detail::stream_component<typename P::l>(os, "m", is_first_component);
        detail::stream_component<typename P::t>(os, "s", is_first_component);
        detail::stream_component<typename P::i>(os, "A", is_first_component);
        detail::stream_component<typename P::th>(os, "K", is_first_component);
        detail::stream_component<typename P::n>(os, "mol", is_first_component);
        detail::stream_component<typename P::j>(os, "cd", is_first_component);

        return os;
    }
}
