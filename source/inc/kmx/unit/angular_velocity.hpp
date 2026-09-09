/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/angular_velocity.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/angle.hpp>
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of angular velocity. The base SI unit of the family is the radian per second.
/// @details An angle is dimensionless, so this family shares its dimension with a frequency; the two
/// are told apart by the unit type, never by the dimension.
namespace kmx::unit::angular_velocity
{
    /// @brief The base SI unit of angular velocity, one radian of rotation per second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(radian_per_second, dimension::angular_velocity_t, scale::one, std::ratio<0>, "rad/s",
                       static constexpr bool angular_rate = true;)

    /// @brief One degree of rotation per second.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(degree_per_second, dimension::angular_velocity_t, angle::radian_per_degree, std::ratio<0>, "deg/s",
                       static constexpr bool angular_rate = true;)
}

/// @brief The literal suffixes building angular velocity values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(rad_s, unit::angular_velocity::radian_per_second)
    KMX_UNIT_LITERALS(deg_s, unit::angular_velocity::degree_per_second)
}
