/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/angle.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <numbers>
#endif

/// @brief Units of plane angle. The base SI unit of the family is the radian.
/// @details An angle is dimensionless by definition, so the dimension system alone cannot tell an angle
/// from a plain ratio. Every unit of this family therefore carries the `angular` tag and the
/// kmx::unit::angle::angle_unit concept selects them, which is what the trigonometric helpers below use.
/// @note The three units named after the same idea of a small angle are deliberately kept apart:
/// kmx::unit::angle::moa is a true minute of angle (1/60 of a degree), kmx::unit::angle::milliradian is
/// exactly one thousandth of a radian, and kmx::unit::angle::mil is the NATO mil, that is 1/6400 of a turn.
/// They are distinct types with distinct sizes and none of them is an alias of another.
namespace kmx::unit::angle
{
    /// @brief The size of one degree in radians, exactly pi over 180.
    /// @details Every size of this family is a rational multiple of pi, and each is kept as one. That is
    /// what lets a degree and an arcminute divide into the exact 60 that their definitions imply, where two
    /// sizes rounded to a `double` first divided into 59.999999999999993.
    using radian_per_degree = scale::pi_multiple<1, 180>;

    /// @brief The size of one arcminute, and equally of one minute of angle, in radians.
    using radian_per_arcminute = scale::divide_t<radian_per_degree, scale::ratio<60>>;

    /// @brief The size of one arcsecond in radians.
    using radian_per_arcsecond = scale::divide_t<radian_per_arcminute, scale::ratio<60>>;

    /// @brief The size of one NATO mil in radians: the turn is divided into 6400 parts.
    /// @note Other conventions divide the turn into 6000 or 6300 parts; this library implements the NATO one.
    using radian_per_nato_mil = scale::pi_multiple<2, 6400>;

    /// @brief The size of one degree in radians, as a number.
    inline constexpr double radian_per_degree_si = scale::value_of<double, radian_per_degree>();

    /// @brief The base SI unit of plane angle.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(radian, dimension::angle_t, scale::one, std::ratio<0>, "rad", static constexpr bool angular = true;)

    /// @brief One thousandth of a radian, the unit of the milliradian reticle and turret.
    /// @note This is the true milliradian, not the NATO mil; see kmx::unit::angle::mil.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(milliradian, dimension::angle_t, scale::milli, std::ratio<0>, "mrad", static constexpr bool angular = true;)

    /// @brief The NATO mil, one 6400th of a full turn.
    /// @note A NATO mil is about 0.98 mrad, so it is close to but not the same as a milliradian.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(mil, dimension::angle_t, radian_per_nato_mil, std::ratio<0>, "mil", static constexpr bool angular = true;)

    /// @brief One 360th of a full turn.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(degree, dimension::angle_t, radian_per_degree, std::ratio<0>, "deg", static constexpr bool angular = true;)

    /// @brief One arcsecond, that is one 3600th of a degree.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(arcsecond, dimension::angle_t, radian_per_arcsecond, std::ratio<0>, "arcsec", static constexpr bool angular = true;)

    /// @brief One arcminute, that is one 60th of a degree, used for angular measurement in general.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(arcminute, dimension::angle_t, radian_per_arcminute, std::ratio<0>, "arcmin", static constexpr bool angular = true;)

    /// @brief The minute of angle, one 60th of a degree, used for sight corrections.
    /// @details It has the same size as kmx::unit::angle::arcminute but is a distinct type, so a sight
    /// correction never silently mixes with a plain angular measurement and prints as "MOA".
    /// @note This is the true minute of angle. It is not the "shooter's MOA" of one inch per hundred yards,
    /// which is about 1.0472 times smaller and is not a unit but an approximation.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE_EX(moa, dimension::angle_t, radian_per_arcminute, std::ratio<0>, "MOA", static constexpr bool angular = true;)

    /// @brief Satisfied by the angle units of this library, which plain dimensionless ratios do not satisfy.
    /// @tparam T Candidate type.
    template <typename T>
    concept angle_unit = unit_type<T> && std::is_same_v<typename T::dimension_t, dimension::angle_t> && requires { T::angular; };

    /// @brief Normalizes an angle to radians, the form every trigonometric kernel expects.
    /// @tparam Angle An angle unit type.
    /// @param value The angle.
    /// @return The angle in radians, as a plain scalar.
    template <angle_unit Angle>
    [[nodiscard]] constexpr auto radians(const Angle& value) noexcept
    {
        using result_t = std::common_type_t<typename Angle::value_t, double>;
        return value.template as_si<result_t>();
    }

    /// @brief Sine of an angle given in any angle unit.
    /// @tparam Angle An angle unit type.
    /// @param value The angle.
    /// @return The sine of @p value.
    template <angle_unit Angle>
    [[nodiscard]] auto sin(const Angle& value) noexcept
    {
        return std::sin(radians(value));
    }

    /// @brief Cosine of an angle given in any angle unit.
    /// @tparam Angle An angle unit type.
    /// @param value The angle.
    /// @return The cosine of @p value.
    template <angle_unit Angle>
    [[nodiscard]] auto cos(const Angle& value) noexcept
    {
        return std::cos(radians(value));
    }

    /// @brief Tangent of an angle given in any angle unit.
    /// @tparam Angle An angle unit type.
    /// @param value The angle.
    /// @return The tangent of @p value.
    template <angle_unit Angle>
    [[nodiscard]] auto tan(const Angle& value) noexcept
    {
        return std::tan(radians(value));
    }

    /// @brief Arc sine, as an angle rather than a bare number.
    /// @param value The sine value.
    /// @return The angle in radians.
    [[nodiscard]] inline radian<double> asin(const double value) noexcept
    {
        return radian<double>(std::asin(value));
    }

    /// @brief Arc cosine, as an angle rather than a bare number.
    /// @param value The cosine value.
    /// @return The angle in radians.
    [[nodiscard]] inline radian<double> acos(const double value) noexcept
    {
        return radian<double>(std::acos(value));
    }

    /// @brief Arc tangent, as an angle rather than a bare number.
    /// @param value The tangent value.
    /// @return The angle in radians.
    [[nodiscard]] inline radian<double> atan(const double value) noexcept
    {
        return radian<double>(std::atan(value));
    }

    /// @brief Arc tangent of @p y / @p x, using the signs of both to select the quadrant.
    /// @param y The ordinate.
    /// @param x The abscissa.
    /// @return The angle in radians, in the range (-pi, pi].
    [[nodiscard]] inline radian<double> atan2(const double y, const double x) noexcept
    {
        return radian<double>(std::atan2(y, x));
    }
}

/// @brief The literal suffixes building angle values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(rad, unit::angle::radian)
    KMX_UNIT_LITERALS(mrad, unit::angle::milliradian)
    KMX_UNIT_LITERALS(mil, unit::angle::mil)
    KMX_UNIT_LITERALS(deg, unit::angle::degree)
    KMX_UNIT_LITERALS(arcmin, unit::angle::arcminute)
    KMX_UNIT_LITERALS(arcsec, unit::angle::arcsecond)
    KMX_UNIT_LITERALS(moa, unit::angle::moa)
}
