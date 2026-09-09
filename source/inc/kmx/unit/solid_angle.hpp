/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/solid_angle.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of solid angle. The base SI unit of the family is the steradian.
/// @details A solid angle is the ratio of an area to the square of a radius, so it is dimensionless, just
/// as a plane angle is. Every unit of the family carries the `solid` tag, which is what tells a solid
/// angle from a plane angle and from a plain ratio.
namespace kmx::unit::solid_angle
{
    /// @brief The size of one square degree in steradians.
    using steradian_per_square_degree = scale::pi_multiple<1, 32400, 2>;

    /// @brief The base SI unit of solid angle.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(steradian, dimension::solid_angle_t, scale::one, std::ratio<0>, "sr", static constexpr bool solid = true;)

    /// @brief One thousandth of a steradian.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(millisteradian, dimension::solid_angle_t, scale::milli, std::ratio<0>, "msr", static constexpr bool solid = true;)

    /// @brief The solid angle subtended by one square degree.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(square_degree, dimension::solid_angle_t, steradian_per_square_degree, std::ratio<0>, "deg2",
                       static constexpr bool solid = true;)

    /// @brief Satisfied by the solid angle units of this library.
    /// @tparam T Candidate type.
    template <typename T>
    concept solid_angle_unit = unit_type<T> && std::is_same_v<typename T::dimension_t, dimension::solid_angle_t> && requires { T::solid; };
}

/// @brief The literal suffixes building solid angle values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(sr, unit::solid_angle::steradian)
    KMX_UNIT_LITERALS(msr, unit::solid_angle::millisteradian)
    KMX_UNIT_LITERALS(deg2, unit::solid_angle::square_degree)
}
