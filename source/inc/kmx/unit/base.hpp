/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/base.hpp
#pragma once
#ifndef PCH
    #include <cmath>
    #include <concepts>
    #include <cstdint>
    #include <limits>
    #include <numbers>
    #include <optional>
    #include <numeric>
    #include <ratio>
    #include <string_view>
    #include <type_traits>
#endif

/// @brief Defines the user-defined literals building values of a unit type.
/// @details The generated literals are constexpr, noexcept and allocation-free, and are the terse way to
/// write a value of the unit: `9.8_mps2`. The number is always interpreted as the native value of the unit,
/// never as a value in base SI units.
/// @note A literal operator receives only `long double` or `unsigned long long`, so the argument type cannot
/// carry the value type the way a constructor argument does. Both overloads therefore produce a
/// `double`-valued unit, which keeps `10_mps2` and `9.8_mps2` the same type; a caller wanting another value
/// type names it, as in `meters_per_second_squared<float> {9.8f}`.
/// @note The suffix MUST begin with an underscore: a literal suffix that does not is reserved for future
/// standardization. That leading underscore is also what makes `_MB` legal, where a variable of that name
/// would not be, the ud-suffix of a literal operator being exempt from the reserved-identifier rule.
/// @note `bool` is excluded as a value type by the unit template itself; see KMX_UNIT_DEFINE_EX.
/// @note The literals are declared in namespace kmx::literals rather than kmx, so that including a unit
/// header does not bring 130-odd suffixes into scope: a caller opts in with `using namespace kmx::literals`.
/// @param suffix The literal suffix WITHOUT its leading underscore, as in `mps2` for `9.8_mps2`.
/// @param type_name The unit class template the literals instantiate.
#define KMX_UNIT_LITERALS(suffix, type_name)                                                \
    [[nodiscard]] constexpr auto operator""_##suffix(const long double val) noexcept        \
    {                                                                                       \
        return type_name<double>(static_cast<double>(val));                                 \
    }                                                                                       \
                                                                                            \
    [[nodiscard]] constexpr auto operator""_##suffix(const unsigned long long val) noexcept \
    {                                                                                       \
        return type_name<double>(static_cast<double>(val));                                 \
    }

/// @brief The dimensions a unit may carry, and the algebra combining them.
/// @brief Defines a unit type of the library.
/// @details A unit definition carries exactly four facts: the name of the type, the dimension it measures,
/// the size of one of its units in base SI units, and the symbol it prints as. Everything else a unit needs
/// is mechanical, and spelling it out per unit made the name of the type appear five times, so that a single
/// mistyped occurrence produced a unit that compiled and measured the wrong thing.
/// @param type_name The name of the generated class template.
/// @param dimension_type The dimension the unit measures, a kmx::unit::dimension::item instantiation.
/// @param magnitude The size of one native unit in base SI units, a kmx::unit::scale::magnitude.
/// @param symbol The text the unit prints as.
#define KMX_UNIT_DEFINE(type_name, dimension_type, magnitude, symbol) \
    KMX_UNIT_DEFINE_EX(type_name, dimension_type, magnitude, std::ratio<0>, symbol)

/// @brief Defines a unit type that has an origin offset, extra members, or both.
/// @details This is the general form of KMX_UNIT_DEFINE. The trailing arguments are inserted verbatim into
/// the body of the generated class, which is how a unit declares an `absolute_scale` tag, an `angular` tag
/// or a `difference_t` alias.
/// @param type_name The name of the generated class template.
/// @param dimension_type The dimension the unit measures, a kmx::unit::dimension::item instantiation.
/// @note The value type is constrained to an arithmetic type other than `bool`. The constraint is on the
/// class template rather than on any one entry point, so it holds for every way of naming the type, the
/// explicit `meter<T> {value}` included, and a violation is detectable rather than a hard error.
/// @param magnitude The size of one native unit in base SI units, a kmx::unit::scale::magnitude.
/// @param offset The SI value matching a native value of zero, a std::ratio; non-zero only for absolute
/// scales. It is exact for the same reason a magnitude is: the origin of the Celsius scale is 5463/20 K,
/// and rounding it once per unit would put a conversion beside the whole number it should land on.
/// @param symbol The text the unit prints as.
#define KMX_UNIT_DEFINE_EX(type_name, dimension_type, magnitude, offset, symbol, ...) \
    template <typename T = double>                                                    \
        requires(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)                 \
    struct type_name: base<type_name<T>, dimension_type, T, magnitude, offset>        \
    {                                                                                 \
        using base<type_name<T>, dimension_type, T, magnitude, offset>::base;         \
                                                                                      \
        template <typename U>                                                         \
        using rebind_t = type_name<U>;                                                \
                                                                                      \
        static constexpr std::string_view text = symbol;                              \
                                                                                      \
        __VA_ARGS__                                                                   \
    };

namespace kmx::unit::dimension
{
    /// @brief Represents the dimension of a unit as a product of powers of the 7 base SI dimensions and of
    /// information_t.
    /// @details The template parameters are std::ratio types representing the exponent for each base dimension.
    /// Information is not an SI dimension, but it behaves as an independent one: it never mixes with a
    /// length_t or a mass_t, and it divides by a time_t exactly as any other dimension does. Giving it a slot of
    /// its own is what lets a data rate be an ordinary quotient rather than a special case. The base unit of
    /// the information_t dimension is the bit.
    template <typename L = std::ratio<0>,     // Length
              typename M = std::ratio<0>,     // Mass
              typename T = std::ratio<0>,     // Time
              typename I = std::ratio<0>,     // Electric Current
              typename Theta = std::ratio<0>, // Thermodynamic Temperature
              typename N = std::ratio<0>,     // Amount of Substance
              typename J = std::ratio<0>,     // Luminous Intensity
              typename B = std::ratio<0>      // Information
              >
    struct item
    {
    };

    // Helper to extract the dimension parameters from a dimension type.
    template <typename>
    struct params;

    template <typename L, typename M, typename T, typename I, typename Th, typename N, typename J, typename B>
    struct params<item<L, M, T, I, Th, N, J, B>>
    {
        using l_t = L;
        using m_t = M;
        using t_t = T;
        using i_t = I;
        using th_t = Th;
        using n_t = N;
        using j_t = J;
        using b_t = B;
    };

    /// @brief A type alias for multiplying two dimensions (adds their exponents).
    template <typename D1, typename D2>
    using multiply_t = item<std::ratio_add<typename params<D1>::l_t, typename params<D2>::l_t>,
                            std::ratio_add<typename params<D1>::m_t, typename params<D2>::m_t>,
                            std::ratio_add<typename params<D1>::t_t, typename params<D2>::t_t>,
                            std::ratio_add<typename params<D1>::i_t, typename params<D2>::i_t>,
                            std::ratio_add<typename params<D1>::th_t, typename params<D2>::th_t>,
                            std::ratio_add<typename params<D1>::n_t, typename params<D2>::n_t>,
                            std::ratio_add<typename params<D1>::j_t, typename params<D2>::j_t>,
                            std::ratio_add<typename params<D1>::b_t, typename params<D2>::b_t>>;

    /// @brief A dimension raised to an integer power (multiplies its exponents).
    /// @note The name is a verb on purpose: `power_t` is already the dimension of a power.
    /// @tparam D The dimension.
    /// @tparam N The exponent.
    template <typename D, std::intmax_t N>
    using raise_t =
        item<std::ratio_multiply<typename params<D>::l_t, std::ratio<N>>, std::ratio_multiply<typename params<D>::m_t, std::ratio<N>>,
             std::ratio_multiply<typename params<D>::t_t, std::ratio<N>>, std::ratio_multiply<typename params<D>::i_t, std::ratio<N>>,
             std::ratio_multiply<typename params<D>::th_t, std::ratio<N>>, std::ratio_multiply<typename params<D>::n_t, std::ratio<N>>,
             std::ratio_multiply<typename params<D>::j_t, std::ratio<N>>, std::ratio_multiply<typename params<D>::b_t, std::ratio<N>>>;

    /// @brief The Nth root of a dimension (divides its exponents).
    /// @details The exponents are std::ratio values, so a root that does not divide evenly is still an
    /// exact dimension rather than an approximation: the square root of a volume is a length to the 3/2.
    /// @tparam D The dimension.
    /// @tparam N The degree of the root.
    template <typename D, std::intmax_t N>
    using root_t =
        item<std::ratio_divide<typename params<D>::l_t, std::ratio<N>>, std::ratio_divide<typename params<D>::m_t, std::ratio<N>>,
             std::ratio_divide<typename params<D>::t_t, std::ratio<N>>, std::ratio_divide<typename params<D>::i_t, std::ratio<N>>,
             std::ratio_divide<typename params<D>::th_t, std::ratio<N>>, std::ratio_divide<typename params<D>::n_t, std::ratio<N>>,
             std::ratio_divide<typename params<D>::j_t, std::ratio<N>>, std::ratio_divide<typename params<D>::b_t, std::ratio<N>>>;

    /// @brief A type alias for dividing two dimensions (subtracts their exponents).
    template <typename D1, typename D2>
    using divide_t = item<std::ratio_subtract<typename params<D1>::l_t, typename params<D2>::l_t>,
                          std::ratio_subtract<typename params<D1>::m_t, typename params<D2>::m_t>,
                          std::ratio_subtract<typename params<D1>::t_t, typename params<D2>::t_t>,
                          std::ratio_subtract<typename params<D1>::i_t, typename params<D2>::i_t>,
                          std::ratio_subtract<typename params<D1>::th_t, typename params<D2>::th_t>,
                          std::ratio_subtract<typename params<D1>::n_t, typename params<D2>::n_t>,
                          std::ratio_subtract<typename params<D1>::j_t, typename params<D2>::j_t>,
                          std::ratio_subtract<typename params<D1>::b_t, typename params<D2>::b_t>>;

    // Base and Common Dimension Type Aliases
    using dimensionless_t = item<>;
    using length_t = item<std::ratio<1>>;
    using mass_t = item<std::ratio<0>, std::ratio<1>>;
    using time_t = item<std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using current_t = item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using temperature_t = item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using amount_of_substance_t = item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using luminous_intensity_t =
        item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
    using information_t =
        item<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;

    // Derived Dimension Type Aliases
    using area_t = multiply_t<length_t, length_t>;        // L²
    using volume_t = multiply_t<area_t, length_t>;        // L³
    using velocity_t = divide_t<length_t, time_t>;        // L/T
    using acceleration_t = divide_t<velocity_t, time_t>;  // L/T²
    using force_t = multiply_t<mass_t, acceleration_t>;   // M·L/T²
    using pressure_t = divide_t<force_t, area_t>;         // M/L·T²
    using energy_t = multiply_t<force_t, length_t>;       // M·L²/T²
    using power_t = divide_t<energy_t, time_t>;           // M·L²/T³
    using angle_t = dimensionless_t;                      // Angles are fundamentally dimensionless_t (ratio of arc length_t to radius)
    using angular_velocity_t = divide_t<angle_t, time_t>; // 1/T
    using charge_t = multiply_t<current_t, time_t>;       // I·T
    using voltage_t = divide_t<energy_t, charge_t>;       // M·L²/I·T³
    using resistance_t = divide_t<voltage_t, current_t>;  // M·L²/I²·T³
    using capacitance_t = divide_t<charge_t, voltage_t>;  // I²·T⁴/M·L²
    using inductance_t = divide_t<multiply_t<energy_t, time_t>, multiply_t<charge_t, charge_t>>; // M·L²/I²·T²
    using illuminance_t = divide_t<luminous_intensity_t, area_t>;
    using frequency_t = divide_t<dimensionless_t, time_t>;             // 1/T, the dimension an angular velocity also has
    using solid_angle_t = dimensionless_t;                             // a solid angle is a ratio of an area to a squared radius
    using torque_t = energy_t;                                         // M·L²/T², the dimension an energy also has
    using magnetic_flux_t = multiply_t<voltage_t, time_t>;             // M·L²/I·T²
    using magnetic_flux_density_t = divide_t<magnetic_flux_t, area_t>; // M/I·T²
    using data_rate_t = divide_t<information_t, time_t>;               // B/T
}

/// @brief The exact size of a unit, and the algebra combining sizes.
namespace kmx::unit::scale
{
    /// @brief The size of one unit expressed in base SI units, as a rational number times a power of pi.
    /// @details A size written as a `double` is already rounded when the unit is defined, so a conversion
    /// between two such units divides one rounded number by another and lands beside the exact answer: a
    /// pound is exactly seven thousand grains, yet the quotient of the two rounded factors is
    /// 6999.9999999999991. Keeping the size exact defers every rounding to the one place it cannot be
    /// avoided, the moment a value is actually scaled, which is what makes the conversion between two
    /// rationally related units exact.
    /// @note A rational alone would not cover the library: an angle is a fraction of pi and a square degree
    /// a fraction of pi squared. The exponent of pi is therefore carried beside the ratio, which keeps a
    /// degree exact as pi/180 rather than as the rounded 0.017453292519943295, and lets a degree and an
    /// arcminute divide into the exact 60 that both of them being fractions of pi implies.
    /// @tparam Ratio The rational part, a std::ratio instantiation and so always in lowest terms.
    /// @tparam PiPower The exponent of pi; zero for every unit that measures no angle.
    template <typename Ratio, std::intmax_t PiPower = 0>
    struct magnitude
    {
        using ratio_t = Ratio;
        static constexpr std::intmax_t pi_power = PiPower;
    };

    /// @brief A magnitude that is a plain rational number.
    /// @tparam Num The numerator.
    /// @tparam Den The denominator.
    template <std::intmax_t Num, std::intmax_t Den = 1>
    using ratio = magnitude<std::ratio<Num, Den>>;

    /// @brief A magnitude that is a rational multiple of a power of pi.
    /// @details `pi_multiple<1, 180>` is the size of a degree and `pi_multiple<1, 32400, 2>` that of a
    /// square degree.
    /// @tparam Num The numerator of the rational part.
    /// @tparam Den The denominator of the rational part.
    /// @tparam Power The exponent of pi.
    template <std::intmax_t Num = 1, std::intmax_t Den = 1, std::intmax_t Power = 1>
    using pi_multiple = magnitude<std::ratio<Num, Den>, Power>;

    // The decimal prefixes of the SI, which name most of the sizes in this library.
    using nano = ratio<1, 1000000000>;
    using micro = ratio<1, 1000000>;
    using milli = ratio<1, 1000>;
    using centi = ratio<1, 100>;
    using one = ratio<1>;
    using hecto = ratio<100>;
    using kilo = ratio<1000>;
    using mega = ratio<1000000>;
    using giga = ratio<1000000000>;
    using tera = ratio<1000000000000>;
    using peta = ratio<1000000000000000>;

    // The binary prefixes of the IEC, which the information family needs beside the decimal ones.
    using kibi = ratio<1024>;
    using mebi = ratio<1048576>;
    using gibi = ratio<1073741824>;
    using tebi = ratio<1099511627776>;
    using pebi = ratio<1125899906842624>;

    /// @brief The product of two magnitudes.
    /// @tparam M1 Left magnitude.
    /// @tparam M2 Right magnitude.
    template <typename M1, typename M2>
    using multiply_t = magnitude<std::ratio_multiply<typename M1::ratio_t, typename M2::ratio_t>, M1::pi_power + M2::pi_power>;

    /// @brief The quotient of two magnitudes.
    /// @tparam M1 Left magnitude.
    /// @tparam M2 Right magnitude.
    template <typename M1, typename M2>
    using divide_t = magnitude<std::ratio_divide<typename M1::ratio_t, typename M2::ratio_t>, M1::pi_power - M2::pi_power>;

    /// @brief Tells whether a magnitude is exactly one, so that applying it is not an operation at all.
    /// @tparam Magnitude The magnitude to test.
    template <typename Magnitude>
    inline constexpr bool is_one = (Magnitude::ratio_t::num == 1) && (Magnitude::ratio_t::den == 1) && (Magnitude::pi_power == 0);

    /// @brief Tells whether a magnitude carries no power of pi and so is a plain rational number.
    /// @tparam Magnitude The magnitude to test.
    template <typename Magnitude>
    inline constexpr bool is_rational = (Magnitude::pi_power == 0);

    /// @brief Tells whether a magnitude is a whole number or the reciprocal of one.
    /// @details Such a magnitude is what a comparison needs to be exact: it says that one of the two units
    /// is a whole multiple of the other, so that the coarser value scales into the finer unit by an integer
    /// and neither operand has to pass through a rounded factor. Twelve inches make a foot and seven
    /// thousand grains a pound; a pound and a kilogram, on the other hand, divide into no such whole number.
    /// @tparam Magnitude The magnitude to test.
    template <typename Magnitude>
    inline constexpr bool is_whole_multiple = is_rational<Magnitude> && ((Magnitude::ratio_t::num == 1) || (Magnitude::ratio_t::den == 1));

    /// @brief The value of a magnitude, as the arithmetic type a computation applies it in.
    /// @details This is the single point at which an exact size becomes a rounded number. The rational part
    /// is divided once, so the result is the correctly rounded value of the whole ratio rather than the
    /// accumulation of a rounding per factor the ratio was built from.
    /// @tparam T The floating point type of the result.
    /// @tparam Magnitude The magnitude to evaluate.
    /// @return The magnitude as a value of @p T.
    template <typename T, typename Magnitude>
        requires std::is_floating_point_v<T>
    [[nodiscard]] consteval T value_of() noexcept
    {
        auto result = static_cast<T>(Magnitude::ratio_t::num) / static_cast<T>(Magnitude::ratio_t::den);
        constexpr auto power = Magnitude::pi_power;
        constexpr auto count = (power < 0) ? -power : power;
        for (std::intmax_t i {}; i != count; ++i)
        {
            if constexpr (power < 0)
                result /= std::numbers::pi_v<T>;
            else
                result *= std::numbers::pi_v<T>;
        }

        return result;
    }
    /// @brief Tells whether the product of two whole numbers fits in a std::intmax_t.
    /// @param left Left operand, non-negative.
    /// @param right Right operand, non-negative.
    /// @return True when the product is representable.
    [[nodiscard]] consteval bool product_fits(const std::intmax_t left, const std::intmax_t right) noexcept
    {
        return (left == 0) || (right == 0) || (left <= (std::numeric_limits<std::intmax_t>::max() / right));
    }

    /// @brief Tells whether multiplying two ratios keeps both halves of the result in a std::intmax_t.
    /// @details It mirrors the reduction std::ratio_multiply performs, cancelling each numerator against
    /// the opposite denominator before multiplying, and answers the question rather than asking for the
    /// product: a std::ratio that does not fit is a hard error and cannot be recovered from afterwards.
    /// @tparam R1 Left ratio.
    /// @tparam R2 Right ratio.
    /// @return True when the reduced product is representable.
    template <typename R1, typename R2>
    [[nodiscard]] consteval bool ratio_multiply_fits() noexcept
    {
        constexpr auto magnitude_of = [](const std::intmax_t value) { return (value < 0) ? -value : value; };
        constexpr auto first = std::gcd(R1::num, R2::den);
        constexpr auto second = std::gcd(R2::num, R1::den);
        return product_fits(magnitude_of(R1::num / first), magnitude_of(R2::num / second)) &&
               product_fits(R1::den / second, R2::den / first);
    }

    /// @brief The size of a combination of two magnitudes, kept exact wherever it is representable.
    /// @details Two exact sizes do not always combine into a third: a foot-pound is 3389544870828501 over
    /// 2500000000000000 joules, and dividing that by a kilowatt hour asks for a denominator of nine
    /// thousand million million million. The pair is perfectly ordinary and its size is an ordinary
    /// `double`, so the combination falls back to that rather than refusing to compile. What is lost is
    /// only the exactness: kmx::unit::scale::combination::is_exact says so, and the comparison that relies
    /// on it steps back to base SI units for that pair alone.
    /// @tparam Magnitude The exact combined size, where one exists.
    /// @tparam Exact Whether that size is representable; never named by a caller.
    /// @tparam Fallback The rounded value used when it is not; never named by a caller.
    template <typename Magnitude, bool Exact, double Fallback>
    struct combination
    {
        /// @brief Tells whether the combined size is held exactly.
        static constexpr bool is_exact = Exact;

        /// @brief Tells whether the combined size is exactly one, so that applying it is not an operation.
        static constexpr bool is_unity = Exact && is_one<Magnitude>;

        /// @brief Tells whether one of the two sizes is a whole multiple of the other.
        static constexpr bool is_whole_multiple = Exact && scale::is_whole_multiple<Magnitude>;

        /// @brief The numerator of the exact size, or one when there is none.
        static constexpr std::intmax_t num = Exact ? Magnitude::ratio_t::num : 1;

        /// @brief The denominator of the exact size, or one when there is none.
        static constexpr std::intmax_t den = Exact ? Magnitude::ratio_t::den : 1;

        /// @brief The combined size as the number a computation multiplies by.
        static constexpr double value = Fallback;
    };

    /// @brief The product of two magnitudes; see kmx::unit::scale::combination.
    /// @tparam M1 Left magnitude.
    /// @tparam M2 Right magnitude.
    /// @tparam Exact Whether the exact product is representable; never named by a caller.
    template <typename M1, typename M2, bool Exact = ratio_multiply_fits<typename M1::ratio_t, typename M2::ratio_t>()>
    struct product: combination<one, false, value_of<double, M1>() * value_of<double, M2>()>
    {
    };

    template <typename M1, typename M2>
    struct product<M1, M2, true>: combination<multiply_t<M1, M2>, true, value_of<double, multiply_t<M1, M2>>()>
    {
        /// @brief The exact product.
        using magnitude_t = multiply_t<M1, M2>;
    };

    /// @brief The quotient of two magnitudes; see kmx::unit::scale::combination.
    /// @tparam M1 Left magnitude.
    /// @tparam M2 Right magnitude.
    /// @tparam Exact Whether the exact quotient is representable; never named by a caller.
    template <typename M1, typename M2,
              bool Exact = ratio_multiply_fits<typename M1::ratio_t, std::ratio<M2::ratio_t::den, M2::ratio_t::num>>()>
    struct quotient: combination<one, false, value_of<double, M1>() / value_of<double, M2>()>
    {
    };

    template <typename M1, typename M2>
    struct quotient<M1, M2, true>: combination<divide_t<M1, M2>, true, value_of<double, divide_t<M1, M2>>()>
    {
        /// @brief The exact quotient.
        using magnitude_t = divide_t<M1, M2>;
    };

}

/// @brief The unit types of the library, the traits and concepts describing them, and the arithmetic,
/// comparison and conversion operating on them.
namespace kmx::unit
{
    /// @brief The arithmetic type a scale factor is applied in.
    /// @details A scale factor is a `double`, so applying it in an integral type would truncate it, and a
    /// factor below one would truncate to zero. An integral type therefore computes in `double` and the
    /// result is converted back only once, at the end.
    /// @tparam T The type the caller asked the result in.
    template <typename T>
    using computation_t = std::conditional_t<std::is_floating_point_v<T>, T, double>;

    /// @brief The arithmetic type an operation on two unit values computes in, and the value type of its
    /// result.
    /// @details Two floating point operands keep their own common type, so a computation on `float` values
    /// stays in `float` rather than widening to `double` behind the caller's back. The widening was not
    /// only a surprise in the type of the result, it also stopped such a value from being stored back into
    /// the unit it came from, and halved the width a loop over those values vectorizes at. An integral
    /// operand is still promoted to `double`, because a scale factor is a `double` and integer arithmetic
    /// would truncate it.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    using arithmetic_t =
        std::conditional_t<std::is_floating_point_v<typename U1::value_t> && std::is_floating_point_v<typename U2::value_t>,
                           std::common_type_t<typename U1::value_t, typename U2::value_t>,
                           std::common_type_t<typename U1::value_t, typename U2::value_t, double>>;

    /// @brief Tells whether rescaling between two units is the identity, so that no factor is ever applied.
    /// @details Two units of one dimension that share a size and an origin rescale into one another by
    /// multiplying by exactly one, which kmx::unit::detail::rescale leaves out altogether. The sizes are
    /// compared as the exact magnitudes they are rather than as the numbers they round to, so two units
    /// that merely round to the same `double` are not mistaken for one.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    inline constexpr bool rescales_exactly = std::is_same_v<typename U1::magnitude_t, typename U2::magnitude_t> &&
                                             std::ratio_equal_v<typename U1::offset_t, typename U2::offset_t>;

    /// @brief The arithmetic type an addition or a subtraction of two unit values computes in.
    /// @details This differs from kmx::unit::arithmetic_t in one case, and it is the common one: two values
    /// of the SAME unit need no scale factor at all, so an integral value type is exact and is kept rather
    /// than promoted. Promoting it was not merely surprising in the type of the result, it lost values: a
    /// `meter<std::int64_t>` above two to the fifty-third does not survive the trip through `double`.
    /// A factor is still applied whenever the two units differ, and an integral operand is promoted then,
    /// because the factor is a `double` and integer arithmetic would truncate it.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    using additive_t =
        std::conditional_t<rescales_exactly<U1, U2>, std::common_type_t<typename U1::value_t, typename U2::value_t>, arithmetic_t<U1, U2>>;

    /// @brief Satisfied by every unit value type of this library.
    /// @tparam T Candidate type.
    template <typename T>
    concept unit_type = requires
    {
        typename T::dimension_t;
        typename T::value_t;
    };

    /// @brief Satisfied by the arithmetic types that may scale a unit value.
    /// @details `bool` is arithmetic but is excluded, for the reason the unit template excludes it as a
    /// value type: its arithmetic is a conversion to zero or one rather than a scale, so `10.0_m * true`
    /// reads as a factor and is a copy, while `10.0_m * false` silently discards the value.
    /// @tparam T Candidate type.
    template <typename T>
    concept scalar_type = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

    /// @brief Satisfied when two unit types measure the same physical dimension.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept same_dimension = unit_type<U1> && unit_type<U2> && std::is_same_v<typename U1::dimension_t, typename U2::dimension_t>;

    /// @brief The kind of quantity a unit measures, where the dimension alone cannot tell two apart.
    /// @details The SI gives several distinct quantities the same dimension: a plane angle, a solid angle
    /// and a plain number are all dimensionless; a torque and an energy are both M·L²/T²; a frequency and
    /// an angular velocity are both 1/T. Converting between two of them is not a change of unit but a
    /// change of quantity, and doing it by scale factor alone yields a wrong number rather than an error:
    /// an angular velocity is a frequency times 2 pi, which no factor in this library expresses. The kind
    /// is what keeps them apart, and a unit declares it with a tag in its definition.
    enum class quantity_kind : std::uint8_t
    {
        generic,     ///< The result of arithmetic, which adopts the kind of whatever it meets.
        plain,       ///< The default: a number, an energy, a frequency, a length.
        angle,       ///< A plane angle, tagged `angular`.
        solid_angle, ///< A solid angle, tagged `solid`.
        torque,      ///< A moment of force, tagged `torsional`.
        angular_rate ///< An angular velocity, tagged `angular_rate`.
    };

    /// @brief The tag type for the generic quantity kind.
    /// @details A generic unit is the wildcard kind: it participates in conversions and comparisons with
    /// any specific quantity kind sharing its dimension.
    struct generic_quantity_kind_tag
    {
    };

    /// @brief The tag type for plain quantities.
    struct plain_quantity_kind_tag
    {
    };

    /// @brief The tag type for plane angles.
    struct angle_quantity_kind_tag
    {
    };

    /// @brief The tag type for solid angles.
    struct solid_angle_quantity_kind_tag
    {
    };

    /// @brief The tag type for torque quantities.
    struct torque_quantity_kind_tag
    {
    };

    /// @brief The tag type for angular velocity quantities.
    struct angular_rate_quantity_kind_tag
    {
    };

    /// @brief The kind of quantity a unit measures.
    /// @tparam Unit The unit type.
    template <typename Unit>
    [[nodiscard]] consteval quantity_kind deduce_quantity_kind() noexcept
    {
        if constexpr (requires { Unit::generic_kind; })
            return quantity_kind::generic;
        else if constexpr (requires { Unit::angular; })
            return quantity_kind::angle;
        else if constexpr (requires { Unit::solid; })
            return quantity_kind::solid_angle;
        else if constexpr (requires { Unit::torsional; })
            return quantity_kind::torque;
        else if constexpr (requires { Unit::angular_rate; })
            return quantity_kind::angular_rate;
        else
            return quantity_kind::plain;
    }

    /// @brief Maps one quantity-kind enumerator to its corresponding tag type.
    /// @tparam Kind Quantity-kind enumerator.
    template <quantity_kind Kind>
    struct quantity_kind_to_tag;

    template <>
    struct quantity_kind_to_tag<quantity_kind::generic>
    {
        using type_t = generic_quantity_kind_tag;
    };

    template <>
    struct quantity_kind_to_tag<quantity_kind::plain>
    {
        using type_t = plain_quantity_kind_tag;
    };

    template <>
    struct quantity_kind_to_tag<quantity_kind::angle>
    {
        using type_t = angle_quantity_kind_tag;
    };

    template <>
    struct quantity_kind_to_tag<quantity_kind::solid_angle>
    {
        using type_t = solid_angle_quantity_kind_tag;
    };

    template <>
    struct quantity_kind_to_tag<quantity_kind::torque>
    {
        using type_t = torque_quantity_kind_tag;
    };

    template <>
    struct quantity_kind_to_tag<quantity_kind::angular_rate>
    {
        using type_t = angular_rate_quantity_kind_tag;
    };

    /// @brief Convenience variable template for kmx::unit::deduce_quantity_kind.
    /// @tparam Unit The unit type.
    template <typename Unit>
    inline constexpr quantity_kind quantity_kind_of = deduce_quantity_kind<Unit>();

    /// @brief The quantity-kind tag of a unit, open to user customization.
    /// @details A unit may provide `using quantity_kind_t = my_kind_tag;` to define a custom quantity kind
    /// that stays distinct from every built-in one. Without that alias, the built-in marker tags
    /// (`angular`, `solid`, `torsional`, `angular_rate`, `generic_kind`) map to the built-in kinds.
    /// @tparam Unit The unit type.
    /// @tparam Enable Detection parameter.
    template <typename Unit, typename Enable = void>
    struct quantity_kind_tag
    {
        using type_t = typename quantity_kind_to_tag<quantity_kind_of<Unit>>::type_t;
    };

    template <typename Unit>
    struct quantity_kind_tag<Unit, std::void_t<typename Unit::quantity_kind_t>>
    {
        using type_t = typename Unit::quantity_kind_t;
    };

    /// @brief Convenience alias naming the quantity-kind tag of a unit.
    /// @tparam Unit The unit type.
    template <typename Unit>
    using quantity_kind_tag_t = typename quantity_kind_tag<Unit>::type_t;

    /// @brief Satisfied when two unit types measure the same kind of quantity.
    /// @details A kmx::unit::si_unit is generic: it is what arithmetic produces, it carries no convention
    /// of its own, and it therefore meets any kind of its dimension. That is what lets the product of a
    /// force and a distance be read back as either an energy or a torque, while keeping a joule and a
    /// newton metre from being mistaken for one another.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept same_kind = std::is_same_v<quantity_kind_tag_t<U1>, generic_quantity_kind_tag> ||
                        std::is_same_v<quantity_kind_tag_t<U2>, generic_quantity_kind_tag> ||
                        std::is_same_v<quantity_kind_tag_t<U1>, quantity_kind_tag_t<U2>>;

    /// @brief Satisfied when two unit types measure the same quantity, that is the same dimension AND kind.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept same_quantity = same_dimension<U1, U2> && same_kind<U1, U2>;

    /// @brief Detects units that denote an absolute position on a scale with a conventional origin.
    /// @details Such units are point-like: a difference of two of them is a value of a different kind.
    /// A unit opts in by declaring `static constexpr bool absolute_scale = true;`. Temperature readings
    /// (kelvin, degree Celsius, degree Fahrenheit) are the only absolute-scale units of this library.
    /// @tparam Unit Candidate unit type.
    template <typename Unit, typename = void>
    struct is_absolute_scale: std::false_type
    {
    };

    template <typename Unit>
    struct is_absolute_scale<Unit, std::void_t<decltype(Unit::absolute_scale)>>: std::bool_constant<Unit::absolute_scale>
    {
    };

    /// @brief Convenience variable template for kmx::unit::is_absolute_scale.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    inline constexpr bool is_absolute_scale_v = is_absolute_scale<Unit>::value;

    /// @brief Maps an absolute-scale unit onto the unit that expresses a difference of two of its values.
    /// @details A unit opts in by declaring a `difference_t` member alias. For every other unit a
    /// difference is expressed in the unit itself.
    /// @tparam Unit Unit type to map.
    template <typename Unit, typename = void>
    struct difference_unit
    {
        using type_t = Unit;
    };

    template <typename Unit>
    struct difference_unit<Unit, std::void_t<typename Unit::difference_t>>
    {
        using type_t = typename Unit::difference_t;
    };

    /// @brief Convenience alias for kmx::unit::difference_unit.
    /// @tparam Unit Unit type to map.
    template <typename Unit>
    using difference_unit_t = typename difference_unit<Unit>::type_t;

    /// @brief Satisfied when two same-dimension values denote the same kind of quantity and may be compared.
    /// @details An absolute reading and a difference are never comparable, even though both carry the same dimension.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept comparable_units = same_quantity<U1, U2> && (is_absolute_scale_v<U1> == is_absolute_scale_v<U2>);

    /// @brief Satisfied when an addition is meaningful, that is when at most one operand is an absolute reading.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept addable_units = same_quantity<U1, U2> && !(is_absolute_scale_v<U1> && is_absolute_scale_v<U2>);

    /// @brief Satisfied when a subtraction is meaningful, that is when an absolute reading is not subtracted from a difference.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept subtractable_units = same_quantity<U1, U2> && (is_absolute_scale_v<U1> || !is_absolute_scale_v<U2>);

    /// @brief Satisfied when a value of @p U2 may be added to, or subtracted from, a value of @p U1 in
    /// place.
    /// @details The right operand may never be an absolute reading: adding two readings has no meaning, and
    /// subtracting one yields a difference, which is not the kind of the left operand and so cannot be
    /// assigned back to it.
    /// @tparam U1 Left unit type, the one assigned to.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept compound_assignable_units = same_quantity<U1, U2> && !is_absolute_scale_v<U2>;

    /// @brief Satisfied when a product or a quotient of two values is meaningful, that is when neither
    /// operand is an absolute reading.
    /// @details A reading is a position on a scale with a conventional origin, so its numeric value carries
    /// that origin and multiplying or dividing it yields a number that depends on the choice of origin
    /// rather than on the physical quantity. Combine a difference instead, or ask for the normalized value
    /// explicitly with kmx::unit::base::as_si.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    concept product_units = unit_type<U1> && unit_type<U2> && !is_absolute_scale_v<U1> && !is_absolute_scale_v<U2>;

    /// @brief The generic CRTP base for all units.
    /// @details This class stores the unit's value in its NATIVE representation for performance.
    /// Arithmetic between same-dimension units is optimized to avoid SI round-trips.
    /// Comparison and cross-dimension arithmetic are performed via a common SI representation.
    /// The class holds exactly one value member, so construction and conversion never allocate.
    /// @tparam DerivedUnit The concrete unit type deriving from this template.
    /// @tparam Dimension The dimension of the unit, a kmx::unit::dimension::item instantiation.
    /// @tparam ValueType The arithmetic type holding the native value.
    /// @tparam Magnitude The size of one native unit in base SI units, a kmx::unit::scale::magnitude.
    /// @tparam Offset The SI value matching a native value of zero, a std::ratio; non-zero only for
    /// absolute scales.
    template <typename DerivedUnit, typename Dimension, typename ValueType, typename Magnitude = scale::one,
              typename Offset = std::ratio<0>>
    struct base
    {
        using value_t = ValueType;
        using dimension_t = Dimension;
        using magnitude_t = Magnitude;
        using offset_t = Offset;

        // A scale with an origin is placed on the SI scale by its offset, and the offset is expressed in
        // units of the scale itself, so reading it back exactly needs the size of that unit to be rational.
        // Every absolute scale of this library is, and one that is not would be a temperature in radians.
        static_assert((Offset::num == 0) || scale::is_rational<Magnitude>, "A unit with an origin offset must have a rational magnitude.");

        /// @brief The size of one native unit in base SI units, rounded to a `double`.
        /// @details The exact size is kmx::unit::base::magnitude_t; this is what a computation applies, and
        /// it is derived rather than declared so that the two can never disagree.
        static constexpr double factor_to_si = scale::value_of<double, Magnitude>();

        /// @brief The SI value matching a native value of zero, rounded to a `double`.
        static constexpr double offset_to_si = static_cast<double>(Offset::num) / static_cast<double>(Offset::den);

        constexpr base() noexcept = default;

        /// @brief Builds a value from its native representation.
        /// @param val_native The value expressed in this unit, never in SI.
        [[nodiscard]] constexpr explicit base(const value_t val_native) noexcept: value(val_native) {}

        /// @brief Returns the value as stored, expressed in this unit.
        /// @return The native value.
        [[nodiscard]] constexpr auto as_native() const noexcept { return value; }

        /// @brief Returns the value normalized to base SI units, the form expected by computation kernels.
        /// @tparam T The arithmetic type of the result.
        /// @return The value expressed in base SI units.
        template <typename T = double>
        [[nodiscard]] constexpr T as_si() const noexcept
        {
            // Only a handful of units have an origin offset, and one in three is the SI unit of its own
            // dimension. Neither `x + 0.0` nor `x * 1.0` is an identity a compiler may remove on its own,
            // because both change a negative zero, so the operation has to be left out here rather than
            // folded later. Whether it can be is read off the exact magnitude rather than off the number it
            // rounds to, so a size that merely rounds to one is still applied.
            using compute_t = computation_t<T>;
            auto result = static_cast<compute_t>(as_native());
            if constexpr (!scale::is_one<Magnitude>)
                result *= static_cast<compute_t>(factor_to_si);

            if constexpr (Offset::num != 0)
                result += static_cast<compute_t>(offset_to_si);

            return static_cast<T>(result);
        }

        [[nodiscard]] constexpr DerivedUnit operator+() const noexcept { return DerivedUnit(+value); }
        [[nodiscard]] constexpr DerivedUnit operator-() const noexcept { return DerivedUnit(-value); }

        // SCALAR ARITHMETIC

        /// @brief Scales the value by a dimensionless factor.
        /// @tparam Scalar An arithmetic type.
        /// @param scalar The factor.
        /// @return The scaled value, in the same unit.
        /// @note Absolute readings such as a temperature cannot be scaled; scale a difference instead.
        template <typename Scalar>
        [[nodiscard]] constexpr auto operator*(const Scalar scalar) const noexcept
            requires(scalar_type<Scalar> && !is_absolute_scale_v<DerivedUnit>)
        {
            using result_value_t = std::common_type_t<value_t, Scalar>;
            using result_unit_t = typename DerivedUnit::template rebind_t<result_value_t>;
            return result_unit_t(static_cast<result_value_t>(value) * scalar);
        }

        /// @brief Divides the value by a dimensionless factor.
        /// @tparam Scalar An arithmetic type.
        /// @param scalar The divisor.
        /// @return The divided value, in the same unit.
        /// @note Absolute readings such as a temperature cannot be divided; divide a difference instead.
        template <typename Scalar>
        [[nodiscard]] constexpr auto operator/(const Scalar scalar) const noexcept
            requires(scalar_type<Scalar> && !is_absolute_scale_v<DerivedUnit>)
        {
            using result_value_t = std::common_type_t<value_t, Scalar>;
            using result_unit_t = typename DerivedUnit::template rebind_t<result_value_t>;
            return result_unit_t(static_cast<result_value_t>(value) / scalar);
        }

        // COMPOUND ASSIGNMENT (SCALAR)

        /// @brief Scales the value in place by a dimensionless factor.
        /// @details The product is formed in the common type of the value type and the scalar, so a factor
        /// below one is applied before anything is stored. An integral value type then truncates on the way
        /// back, which is the most an operation that has to keep the type of its left operand can do:
        /// `meter<int> {10} *= 0.25` is 2, where the non-assigning `meter<int> {10} * 0.25` widens the value
        /// type and yields 2.5. Reach for the non-assigning form whenever the result is not meant to narrow.
        /// @tparam Scalar An arithmetic type.
        /// @param scalar The factor.
        /// @return This value.
        template <typename Scalar>
        constexpr DerivedUnit& operator*=(const Scalar scalar) noexcept
            requires(scalar_type<Scalar> && !is_absolute_scale_v<DerivedUnit>)
        {
            using compute_t = std::common_type_t<value_t, Scalar>;
            value = static_cast<value_t>(static_cast<compute_t>(value) * scalar);
            return static_cast<DerivedUnit&>(*this);
        }

        /// @brief Divides the value in place by a dimensionless factor.
        /// @details The quotient narrows the way a product does; see kmx::unit::base::operator*=.
        /// @tparam Scalar An arithmetic type.
        /// @param scalar The divisor.
        /// @return This value.
        template <typename Scalar>
        constexpr DerivedUnit& operator/=(const Scalar scalar) noexcept
            requires(scalar_type<Scalar> && !is_absolute_scale_v<DerivedUnit>)
        {
            using compute_t = std::common_type_t<value_t, Scalar>;
            value = static_cast<value_t>(static_cast<compute_t>(value) / scalar);
            return static_cast<DerivedUnit&>(*this);
        }

    protected:
        /// @brief The value, in the native scale of the unit.
        /// @details It is not public: a unit value that can be written through without naming a unit is
        /// exactly the mistake this library exists to prevent. Read it with kmx::unit::base::as_native.
        value_t value {};
    };

    /// @brief A generic unit type representing a value in base SI units.
    /// @details This is the return type for cross-dimensional arithmetic.
    /// @tparam Dimension The dimension of the value.
    /// @tparam T The arithmetic type holding the value.
    template <typename Dimension, typename T = double>
    struct si_unit: base<si_unit<Dimension, T>, Dimension, T>
    {
        using dimension_t = Dimension;

        /// @brief Marks the type as carrying no quantity convention; see kmx::unit::quantity_kind.
        static constexpr bool generic_kind = true;

        using base<si_unit<Dimension, T>, Dimension, T>::base;

        template <typename U>
        using rebind_t = si_unit<Dimension, U>;
    };

    /// @brief Calculates the relative size and offset for direct unit-to-unit conversion at compile time.
    /// @details Both are worked out on the exact magnitudes and only then rounded, which is the whole point
    /// of carrying a size as a ratio: a pound over a grain reduces to exactly 7000, where the quotient of
    /// the two rounded sizes is 6999.9999999999991 and a converted value lands beside the whole number it
    /// should be.
    /// @tparam TargetUnit The unit the value is converted to.
    /// @tparam SourceUnit The unit the value is converted from.
    template <typename TargetUnit, typename SourceUnit>
    struct relative_conversion
    {
        static_assert(std::is_same_v<typename TargetUnit::dimension_t, typename SourceUnit::dimension_t>);

        /// @brief The size of one source unit measured in target units, exact wherever it fits.
        using scale_t = scale::quotient<typename SourceUnit::magnitude_t, typename TargetUnit::magnitude_t>;

        /// @brief The exact difference of the two origins, measured in target units.
        using offset_t = std::ratio_divide<std::ratio_subtract<typename SourceUnit::offset_t, typename TargetUnit::offset_t>,
                                           typename TargetUnit::magnitude_t::ratio_t>;

        static constexpr double factor = scale_t::value;
        static constexpr double offset = static_cast<double>(offset_t::num) / static_cast<double>(offset_t::den);
    };
}

/// @brief The implementation details of the library. Nothing here is part of the public interface.
namespace kmx::unit::detail
{
    /// @brief Rescales a native value of @p SourceUnit into the native scale of @p TargetUnit.
    /// @details The origin offset is applied only when both units are absolute readings. A difference
    /// carries no origin, so it is rescaled by the unit size alone.
    /// @tparam TargetUnit The unit whose scale the result is expressed in.
    /// @tparam SourceUnit The unit the value is expressed in.
    /// @tparam Value The arithmetic type of the value, always a floating-point type in library use.
    /// @param source_native The value expressed in @p SourceUnit.
    /// @return The value expressed in the native scale of @p TargetUnit.
    template <typename TargetUnit, typename SourceUnit, typename Value>
    [[nodiscard]] constexpr Value rescale(const Value source_native) noexcept
    {
        using conversion_t = relative_conversion<TargetUnit, SourceUnit>;
        auto result = source_native;

        // Whether a factor is the identity is read off the exact relative magnitude, so two units of the
        // same size skip the multiplication however their sizes were spelled, and no other pair skips it.
        if constexpr (!conversion_t::scale_t::is_unity)
            result *= static_cast<Value>(conversion_t::factor);

        if constexpr (is_absolute_scale_v<TargetUnit> && is_absolute_scale_v<SourceUnit> && (conversion_t::offset_t::num != 0))
            result += static_cast<Value>(conversion_t::offset);

        return result;
    }

    /// @brief Narrows a finite floating-point value into an integral unit when the value is representable.
    /// @details The caller chooses the rounding mode before calling this helper. Keeping the range check in
    /// one place ensures every checked conversion rejects the same non-finite and out-of-range values.
    /// @tparam TargetUnit The integral unit of the result.
    /// @tparam Value The floating-point value to narrow.
    /// @param value The already converted value.
    /// @return The unit value, or std::nullopt when @p value cannot be represented.
    template <typename TargetUnit, typename Value>
    [[nodiscard]] constexpr std::optional<TargetUnit> checked_integral_value(const Value value) noexcept
        requires(std::is_integral_v<typename TargetUnit::value_t> && std::is_floating_point_v<Value>)
    {
        if (value != value)
            return std::nullopt;

        const auto value_as_long_double = static_cast<long double>(value);
        const auto minimum = static_cast<long double>(std::numeric_limits<typename TargetUnit::value_t>::lowest());
        const auto upper_exclusive = static_cast<long double>(std::numeric_limits<typename TargetUnit::value_t>::max()) + 1.0L;
        if ((value_as_long_double < minimum) || (value_as_long_double >= upper_exclusive))
            return std::nullopt;

        return TargetUnit(static_cast<typename TargetUnit::value_t>(value));
    }

    /// @brief Tells whether a unit normalizes to base SI units by a scale factor alone.
    /// @details Only a scale with a conventional origin has a non-zero offset, and such a unit never
    /// reaches a product or a quotient; see kmx::unit::product_units. The check is still made rather than
    /// assumed, so that a unit added later cannot silently take the folded path it does not belong on.
    /// @tparam Unit The unit to test.
    template <typename Unit>
    inline constexpr bool scales_without_origin = (Unit::offset_t::num == 0);

    /// @brief Combines two values already reduced to a common arithmetic type and applies a scale factor.
    /// @details This is what lets a product or a quotient of two scaled units cost one multiplication less
    /// than normalizing both operands first: the sizes of the two units meet in a single compile time
    /// constant, and floating point arithmetic is not associative, so a compiler cannot make that
    /// substitution on its own. The two meet as exact magnitudes and are rounded once, so a product of
    /// units whose sizes cancel, such as a length over a length, carries no factor at all.
    /// @tparam Combination The combined size of the two units, a kmx::unit::scale::product or quotient.
    /// @tparam Value The arithmetic type of the operands and of the result.
    /// @param combined The product or quotient of the two native values.
    /// @return The value expressed in base SI units.
    template <typename Combination, typename Value>
    [[nodiscard]] constexpr Value apply_factor(const Value combined) noexcept
    {
        if constexpr (Combination::is_unity)
            return combined;
        else
            return combined * static_cast<Value>(Combination::value);
    }

    /// @brief The size of one @p U1 measured in @p U2, exact wherever it fits.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    using relative_scale_t = scale::quotient<typename U1::magnitude_t, typename U2::magnitude_t>;

    /// @brief Tells whether two units compare exactly, by scaling the coarser of them into the finer.
    /// @details Where one unit is a whole multiple of the other, both values reach a common size through an
    /// exact integer multiplication, and the operand already expressed in the finer unit is not touched at
    /// all. That is what makes `1.0_lb == 7000.0_gr` true where normalizing both to kilograms, each through
    /// its own rounded factor, made it false. Every other pair still compares in base SI units.
    /// @note An origin offset excludes the pair: an origin is carried by the value rather than by the size,
    /// so it cannot be reached by scaling alone. Only a temperature reading has one.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    inline constexpr bool compares_by_whole_multiple =
        relative_scale_t<U1, U2>::is_whole_multiple && scales_without_origin<U1> && scales_without_origin<U2>;

    /// @brief Tells whether two readings compare through the exact conversion between their two scales.
    /// @details An origin cannot be reached by scaling, so a pair of readings cannot meet at the finer of
    /// their units the way kmx::unit::detail::compares_by_whole_multiple describes. Converting one reading
    /// into the scale of the other instead applies the exact relative size and the exact difference of the
    /// two origins, and lands on the whole number a reader expects: 15 degC converts to exactly 59 degF,
    /// where normalizing both to kelvin left them one unit apart in the last place.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    inline constexpr bool compares_by_relative_conversion = is_absolute_scale_v<U1> && is_absolute_scale_v<U2>;

    /// @brief Tells which of two scales with origins the pair is compared in.
    /// @details The finer of the two, so that the conversion multiplies rather than divides; where the two
    /// are equally fine, the one whose origin is lower. Both tests read the exact magnitudes and offsets,
    /// so the answer follows from the pair of units alone and not from the order they are written in, which
    /// is what keeps `a == b` and `b == a` the same computation.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    template <typename U1, typename U2>
    inline constexpr bool compares_in_left_unit =
        std::ratio_less_v<typename U1::magnitude_t::ratio_t, typename U2::magnitude_t::ratio_t> ||
        (std::ratio_equal_v<typename U1::magnitude_t::ratio_t, typename U2::magnitude_t::ratio_t> &&
         std::ratio_less_v<typename U1::offset_t, typename U2::offset_t>);

    /// @brief Brings the left operand of a comparison to the size the two are compared at.
    /// @note The factor depends on the pair of units and not on which side of the operator either sits, so
    /// `a == b` and `b == a` remain the same computation. C++20 may pick either through the reversed
    /// candidate, so that has to hold.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @tparam Value The arithmetic type the two are compared in.
    /// @param native The left value, in its own unit.
    /// @return The left value at the common size.
    template <typename U1, typename U2, typename Value>
    [[nodiscard]] constexpr Value common_left(const Value native) noexcept
    {
        constexpr auto factor = static_cast<Value>(relative_scale_t<U1, U2>::num);
        if constexpr (factor == static_cast<Value>(1))
            return native;
        else
            return native * factor;
    }

    /// @brief Brings the right operand of a comparison to the size the two are compared at.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @tparam Value The arithmetic type the two are compared in.
    /// @param native The right value, in its own unit.
    /// @return The right value at the common size.
    template <typename U1, typename U2, typename Value>
    [[nodiscard]] constexpr Value common_right(const Value native) noexcept
    {
        constexpr auto factor = static_cast<Value>(relative_scale_t<U1, U2>::den);
        if constexpr (factor == static_cast<Value>(1))
            return native;
        else
            return native * factor;
    }
}

namespace kmx::unit
{
    // COMPARISON
    //
    // The comparisons are free functions rather than members on purpose. A member comparison taking a
    // generic other unit makes the rewritten candidate of the right operand tie with the non-rewritten
    // candidate of the left one, which is the C++20 ambiguous reversed operator; some compilers reject the
    // comparison outright inside a SFINAE context, as a test framework does when it decomposes an assertion.
    //
    // Two values are compared at a size both can reach. Where one unit is a whole multiple of the other,
    // that size is the finer of the two units and reaching it is exact; otherwise it is base SI units, and
    // each operand is scaled by its own rounded factor. See kmx::unit::detail::compares_by_whole_multiple.

    /// @brief Orders two values of the same dimension and of the same kind.
    /// @details Values of the same unit compare on their native value; otherwise both are brought to the
    /// size described above.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The ordering of the two values.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator<=>(const U1& lhs, const U2& rhs) noexcept
        requires comparable_units<U1, U2>
    {
        if constexpr (std::is_same_v<U1, U2>)
            return lhs.as_native() <=> rhs.as_native();
        else if constexpr (detail::compares_by_whole_multiple<U1, U2>)
        {
            using value_t = arithmetic_t<U1, U2>;
            return detail::common_left<U1, U2>(static_cast<value_t>(lhs.as_native())) <=>
                   detail::common_right<U1, U2>(static_cast<value_t>(rhs.as_native()));
        }
        else if constexpr (detail::compares_by_relative_conversion<U1, U2>)
        {
            // Two readings meet in one of their own scales rather than in kelvin; see the helper.
            using value_t = arithmetic_t<U1, U2>;
            if constexpr (detail::compares_in_left_unit<U1, U2>)
                return static_cast<value_t>(lhs.as_native()) <=> detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native()));
            else
                return detail::rescale<U2, U1>(static_cast<value_t>(lhs.as_native())) <=> static_cast<value_t>(rhs.as_native());
        }
        else
            return lhs.as_si() <=> rhs.as_si();
    }

    /// @brief Tells whether two values of the same dimension and of the same kind are equal.
    /// @details Values of the same unit compare on their native value; otherwise both are brought to the
    /// size described above.
    /// @note Whichever size is chosen, each operand is scaled by a factor that follows from the pair of
    /// units alone and not from which side of the operator it sits on, which is what makes the operator
    /// symmetric: rescaling one operand into the unit of the other would make `a == b` and `b == a` two
    /// different computations, and C++20 may pick either of them through the reversed candidate.
    /// @note Two units that divide into one another exactly compare exactly, so `1.0_lb == 7000.0_gr` is
    /// true: a pound is seven thousand grains, and both values reach grains by an integer multiplication.
    /// Two units that do not, such as a pound and a kilogram, meet in base SI units instead and are each
    /// scaled by a rounded factor, so their equality is an ordinary floating point comparison and is exact
    /// only when both factors land on the same binary value. Compare with kmx::unit::near whenever that is
    /// not so.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return True when both denote the same quantity.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr bool operator==(const U1& lhs, const U2& rhs) noexcept
        requires comparable_units<U1, U2>
    {
        if constexpr (std::is_same_v<U1, U2>)
            return lhs.as_native() == rhs.as_native();
        else if constexpr (detail::compares_by_whole_multiple<U1, U2>)
        {
            using value_t = arithmetic_t<U1, U2>;
            return detail::common_left<U1, U2>(static_cast<value_t>(lhs.as_native())) ==
                   detail::common_right<U1, U2>(static_cast<value_t>(rhs.as_native()));
        }
        else if constexpr (detail::compares_by_relative_conversion<U1, U2>)
        {
            // Two readings meet in one of their own scales rather than in kelvin; see the helper.
            using value_t = arithmetic_t<U1, U2>;
            if constexpr (detail::compares_in_left_unit<U1, U2>)
                return static_cast<value_t>(lhs.as_native()) == detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native()));
            else
                return detail::rescale<U2, U1>(static_cast<value_t>(lhs.as_native())) == static_cast<value_t>(rhs.as_native());
        }
        else
            return lhs.as_si() == rhs.as_si();
    }

    /// @brief Tells whether two values of the same dimension and of the same kind agree within a tolerance.
    /// @details This is the comparison to reach for whenever the two units do not divide into one another
    /// exactly, which kmx::unit::operator== describes. It answers the question exact equality cannot:
    /// `near(1.0_lb, 0.45359237_kg, 1.0_ug)` is true.
    /// @note The tolerance is a typed value rather than a bare number, so it can never be read in a unit
    /// other than the one it was written in. It may not be an absolute reading: a tolerance is a width on a
    /// scale, not a position on one, so a temperature is compared to within a `delta_celsius` and never a
    /// `celsius`. A negative tolerance admits nothing.
    /// @note A pair that kmx::unit::operator== calls equal is accepted whatever canonical form that
    /// comparison used, so the two never disagree about such a pair and a tolerance of zero is never
    /// stricter than exact equality. Every other pair is measured in base SI units.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @tparam Tolerance The unit the tolerance is expressed in.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @param tolerance The largest difference still counted as equality.
    /// @return True when the two values differ by at most @p tolerance.
    template <typename U1, typename U2, typename Tolerance>
    [[nodiscard]] constexpr bool near(const U1& lhs, const U2& rhs, const Tolerance& tolerance) noexcept
        requires(comparable_units<U1, U2> && same_dimension<U1, Tolerance> && !is_absolute_scale_v<Tolerance>)
    {
        if (tolerance.as_native() < typename Tolerance::value_t {})
            return false;

        if (lhs == rhs)
            return true;

        // An origin offset is carried by both operands and cancels in the difference, so a reading on a
        // scale with an origin needs no separate treatment here.
        const auto difference = lhs.as_si() - rhs.as_si();
        const auto magnitude = (difference < 0.0) ? -difference : difference;
        return magnitude <= tolerance.as_si();
    }

    /// @brief Adds two values of the same dimension.
    /// @details At most one operand may be an absolute reading; the result is then expressed in the unit of
    /// that reading. Otherwise the result is expressed in the unit of the left operand.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The sum, in the unit described above.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator+(const U1& lhs, const U2& rhs) noexcept
        requires addable_units<U1, U2>
    {
        using value_t = additive_t<U1, U2>;
        if constexpr (is_absolute_scale_v<U2>)
        {
            using result_t = typename U2::template rebind_t<value_t>;
            return result_t(static_cast<value_t>(rhs.as_native()) + detail::rescale<U2, U1>(static_cast<value_t>(lhs.as_native())));
        }
        else
        {
            using result_t = typename U1::template rebind_t<value_t>;
            return result_t(static_cast<value_t>(lhs.as_native()) + detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native())));
        }
    }

    /// @brief Subtracts two values of the same dimension.
    /// @details The difference of two absolute readings is expressed in the difference unit of the left
    /// operand, so that it can never be mistaken for a reading. Every other subtraction yields the unit of
    /// the left operand.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The difference, in the unit described above.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator-(const U1& lhs, const U2& rhs) noexcept
        requires subtractable_units<U1, U2>
    {
        using value_t = additive_t<U1, U2>;
        using result_unit_t = std::conditional_t<is_absolute_scale_v<U1> && is_absolute_scale_v<U2>, difference_unit_t<U1>, U1>;
        using result_t = typename result_unit_t::template rebind_t<value_t>;
        return result_t(static_cast<value_t>(lhs.as_native()) - detail::rescale<U1, U2>(static_cast<value_t>(rhs.as_native())));
    }

    /// @brief Adds a value of the same dimension in place.
    /// @details The right operand may never be an absolute reading; see kmx::unit::compound_assignable_units.
    /// The sum is computed in the wider type and converted back into the value type of the left operand,
    /// which is what makes the operation an assignment rather than a new value of a different type.
    /// @tparam U1 Left unit type, the one assigned to.
    /// @tparam U2 Right unit type.
    /// @param lhs The value added to.
    /// @param rhs The value added.
    /// @return @p lhs.
    template <typename U1, typename U2>
    constexpr U1& operator+=(U1& lhs, const U2& rhs) noexcept
        requires compound_assignable_units<U1, U2>
    {
        lhs = U1(static_cast<typename U1::value_t>((lhs + rhs).as_native()));
        return lhs;
    }

    /// @brief Subtracts a value of the same dimension in place.
    /// @details The right operand may never be an absolute reading; see kmx::unit::compound_assignable_units.
    /// @tparam U1 Left unit type, the one assigned to.
    /// @tparam U2 Right unit type.
    /// @param lhs The value subtracted from.
    /// @param rhs The value subtracted.
    /// @return @p lhs.
    template <typename U1, typename U2>
    constexpr U1& operator-=(U1& lhs, const U2& rhs) noexcept
        requires compound_assignable_units<U1, U2>
    {
        lhs = U1(static_cast<typename U1::value_t>((lhs - rhs).as_native()));
        return lhs;
    }

    // CROSS-DIMENSION ARITHMETIC (MULTIPLICATION/DIVISION)

    /// @brief Multiplies two values of any dimensions.
    /// @details Neither operand may be an absolute reading; see kmx::unit::product_units.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The product, as a kmx::unit::si_unit of the product dimension.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator*(const U1& lhs, const U2& rhs) noexcept
        requires product_units<U1, U2>
    {
        using result_dimension_t = dimension::multiply_t<typename U1::dimension_t, typename U2::dimension_t>;
        using result_value_t = arithmetic_t<U1, U2>;
        using result_t = si_unit<result_dimension_t, result_value_t>;
        if constexpr (detail::scales_without_origin<U1> && detail::scales_without_origin<U2>)
        {
            // (a * fa) * (b * fb) is (a * b) * (fa * fb), and the second factor is a compile time constant.
            const auto product = static_cast<result_value_t>(lhs.as_native()) * static_cast<result_value_t>(rhs.as_native());
            using combined_t = scale::product<typename U1::magnitude_t, typename U2::magnitude_t>;
            return result_t(detail::apply_factor<combined_t>(product));
        }
        else
        {
            const auto result_si_value = lhs.template as_si<result_value_t>() * rhs.template as_si<result_value_t>();
            return result_t(result_si_value);
        }
    }

    /// @brief Divides two values of any dimensions.
    /// @details Neither operand may be an absolute reading; see kmx::unit::product_units.
    /// @tparam U1 Left unit type.
    /// @tparam U2 Right unit type.
    /// @param lhs Left operand.
    /// @param rhs Right operand.
    /// @return The quotient, as a kmx::unit::si_unit of the quotient dimension.
    template <typename U1, typename U2>
    [[nodiscard]] constexpr auto operator/(const U1& lhs, const U2& rhs) noexcept
        requires product_units<U1, U2>
    {
        using result_dimension_t = dimension::divide_t<typename U1::dimension_t, typename U2::dimension_t>;
        using result_value_t = arithmetic_t<U1, U2>;
        using result_t = si_unit<result_dimension_t, result_value_t>;
        if constexpr (detail::scales_without_origin<U1> && detail::scales_without_origin<U2>)
        {
            // (a * fa) / (b * fb) is (a / b) * (fa / fb); two units of one dimension cancel the factor
            // entirely, so a quotient such as a distance over a distance costs the division alone.
            const auto quotient = static_cast<result_value_t>(lhs.as_native()) / static_cast<result_value_t>(rhs.as_native());
            using combined_t = scale::quotient<typename U1::magnitude_t, typename U2::magnitude_t>;
            return result_t(detail::apply_factor<combined_t>(quotient));
        }
        else
        {
            const auto result_si_value = lhs.template as_si<result_value_t>() / rhs.template as_si<result_value_t>();
            return result_t(result_si_value);
        }
    }

    /// @brief Commutative scalar multiplication.
    /// @param scalar The factor.
    /// @param unit The value to scale.
    /// @return The scaled value, in the unit of @p unit.
    template <typename Scalar, typename DerivedUnit, typename Dimension, typename ValueType, typename Magnitude, typename Offset>
    [[nodiscard]] constexpr auto operator*(const Scalar scalar,
                                           const base<DerivedUnit, Dimension, ValueType, Magnitude, Offset>& unit) noexcept
        requires(scalar_type<Scalar> && !is_absolute_scale_v<DerivedUnit>)
    {
        return unit * scalar;
    }

    /// @brief Divides a dimensionless number by a value, that is the reciprocal of that value scaled.
    /// @details The member operator divides a value BY a number and keeps its unit; this is the other
    /// direction, and it inverts the dimension, so `1.0 / period` is how a frequency is written. The result
    /// is a kmx::unit::si_unit of the inverted dimension, exactly as a quotient of two values would be.
    /// @tparam Scalar An arithmetic type.
    /// @tparam Unit The unit type of the divisor.
    /// @param scalar The dividend.
    /// @param unit The divisor.
    /// @return The quotient, as a kmx::unit::si_unit of the inverted dimension.
    template <typename Scalar, typename Unit>
    [[nodiscard]] constexpr auto operator/(const Scalar scalar, const Unit& unit) noexcept
        requires(scalar_type<Scalar> && unit_type<Unit> && !is_absolute_scale_v<Unit>)
    {
        using result_dimension_t = dimension::divide_t<dimension::dimensionless_t, typename Unit::dimension_t>;
        using result_value_t = std::conditional_t<std::is_floating_point_v<Scalar> && std::is_floating_point_v<typename Unit::value_t>,
                                                  std::common_type_t<Scalar, typename Unit::value_t>,
                                                  std::common_type_t<Scalar, typename Unit::value_t, double>>;
        return si_unit<result_dimension_t, result_value_t>(static_cast<result_value_t>(scalar) / unit.template as_si<result_value_t>());
    }

    // CONVERSION

    /// @brief Converts a value into another unit of the same dimension and of the same kind.
    /// @details An absolute reading converts only into another absolute scale, and a difference only into
    /// another difference unit, so a temperature can never be silently reinterpreted as a temperature step.
    /// @tparam TargetUnit The unit of the result.
    /// @tparam SourceUnit The unit of the source, deduced.
    /// @param source The value to convert.
    /// @return The value expressed in @p TargetUnit.
    template <typename TargetUnit, typename SourceUnit>
    [[nodiscard]] constexpr TargetUnit convert(const SourceUnit& source) noexcept
    {
        static_assert(std::is_same_v<typename TargetUnit::dimension_t, typename SourceUnit::dimension_t>,
                      "A conversion requires both units to share the same dimension.");
        static_assert(same_kind<TargetUnit, SourceUnit>,
                      "A conversion cannot change the kind of quantity: a plane angle is not a solid angle, "
                      "a torque is not an energy, and an angular velocity is not a frequency.");
        static_assert(is_absolute_scale_v<TargetUnit> == is_absolute_scale_v<SourceUnit>,
                      "An absolute reading and a difference cannot be converted into one another.");

        // The relative factor and offset of the pair of units are compile time constants, so the whole
        // conversion is one multiplication and, for a scale with an origin, one addition. Normalizing to SI
        // first and dividing by the factor of the target would compute the same value with a run time
        // division, which costs several times a multiplication and does not vectorize as well.
        using compute_t = computation_t<std::common_type_t<typename SourceUnit::value_t, typename TargetUnit::value_t>>;
        const auto target_native = detail::rescale<TargetUnit, SourceUnit>(static_cast<compute_t>(source.as_native()));
        return TargetUnit(static_cast<typename TargetUnit::value_t>(target_native));
    }

    /// @brief Converts a value into another unit, rounding the result to the nearest representable value.
    /// @details Intended for target units whose value type is an integral type.
    /// @tparam TargetUnit The unit of the result.
    /// @tparam SourceUnit The unit of the source, deduced.
    /// @tparam FloatType The floating-point type used for the intermediate SI value.
    /// @param source The value to convert.
    /// @return The rounded value expressed in @p TargetUnit.
    template <typename TargetUnit, typename SourceUnit, typename FloatType = double>
    [[nodiscard]] constexpr TargetUnit fp_convert(const SourceUnit& source) noexcept
    {
        static_assert(std::is_floating_point_v<FloatType>);
        static_assert(std::is_same_v<typename TargetUnit::dimension_t, typename SourceUnit::dimension_t>,
                      "A conversion requires both units to share the same dimension.");
        static_assert(same_kind<TargetUnit, SourceUnit>,
                      "A conversion cannot change the kind of quantity: a plane angle is not a solid angle, "
                      "a torque is not an energy, and an angular velocity is not a frequency.");
        static_assert(is_absolute_scale_v<TargetUnit> == is_absolute_scale_v<SourceUnit>,
                      "An absolute reading and a difference cannot be converted into one another.");
        const auto target_native = detail::rescale<TargetUnit, SourceUnit>(static_cast<FloatType>(source.as_native()));
        return TargetUnit(static_cast<typename TargetUnit::value_t>(std::round(target_native)));
    }

    /// @brief Converts a value to an integral unit, returning no value when rounding cannot be represented.
    /// @details This is the checked counterpart to kmx::unit::fp_convert. It returns no value for NaN,
    /// infinity or a rounded result outside the target value type's range. The existing fp_convert remains
    /// available for callers that deliberately rely on its unchecked narrowing conversion.
    /// @tparam TargetUnit The integral unit of the result.
    /// @tparam SourceUnit The unit of the source, deduced.
    /// @tparam FloatType The floating-point type used for the intermediate conversion.
    /// @param source The value to convert.
    /// @return The rounded value, or std::nullopt when it cannot be represented.
    template <typename TargetUnit, typename SourceUnit, typename FloatType = double>
    [[nodiscard]] constexpr std::optional<TargetUnit> checked_fp_convert(const SourceUnit& source) noexcept
    {
        static_assert(std::is_floating_point_v<FloatType>);
        static_assert(std::is_integral_v<typename TargetUnit::value_t>,
                      "checked_fp_convert requires an integral target value type.");
        static_assert(std::is_same_v<typename TargetUnit::dimension_t, typename SourceUnit::dimension_t>,
                      "A conversion requires both units to share the same dimension.");
        static_assert(same_kind<TargetUnit, SourceUnit>,
                      "A conversion cannot change the kind of quantity: a plane angle is not a solid angle, "
                      "a torque is not an energy, and an angular velocity is not a frequency.");
        static_assert(is_absolute_scale_v<TargetUnit> == is_absolute_scale_v<SourceUnit>,
                      "An absolute reading and a difference cannot be converted into one another.");

        const auto target_native = std::round(detail::rescale<TargetUnit, SourceUnit>(static_cast<FloatType>(source.as_native())));
        return detail::checked_integral_value<TargetUnit>(target_native);
    }

    /// @brief Builds a unit value from a value already normalized to base SI units.
    /// @details This is the explicit inverse of kmx::unit::base::as_si and the intended way to turn a
    /// computation kernel result back into a reporting unit.
    /// @tparam TargetUnit The unit of the result.
    /// @tparam T The arithmetic type of the SI value.
    /// @param si_value The value expressed in base SI units.
    /// @return The value expressed in @p TargetUnit.
    template <typename TargetUnit, typename T = double>
    [[nodiscard]] constexpr TargetUnit from_si(const T si_value) noexcept
    {
        // The reciprocal of the scale factor is a compile time constant, so this costs a multiplication
        // rather than a division. Computing in `double` for an integral T also keeps a factor below one
        // from truncating to zero.
        using compute_t = computation_t<T>;
        using inverse_t = scale::quotient<scale::one, typename TargetUnit::magnitude_t>;
        constexpr auto inverse_factor = static_cast<compute_t>(inverse_t::value);
        constexpr auto offset = static_cast<compute_t>(TargetUnit::offset_to_si);
        auto result = static_cast<compute_t>(si_value);
        if constexpr (offset != static_cast<compute_t>(0))
            result -= offset;

        if constexpr (inverse_factor != static_cast<compute_t>(1))
            result *= inverse_factor;

        return TargetUnit(static_cast<typename TargetUnit::value_t>(result));
    }

    /// @brief Builds an integral unit value from SI units, returning no value when rounding is impossible.
    /// @details This is the checked counterpart to kmx::unit::from_si. It returns no value for NaN,
    /// infinity or a result outside the target value type's range. The SI input is rounded to the nearest
    /// native integral value, matching checked_fp_convert.
    /// @tparam TargetUnit The integral unit of the result.
    /// @tparam T The arithmetic type of the SI value.
    /// @param si_value The value expressed in base SI units.
    /// @return The rounded value, or std::nullopt when it cannot be represented.
    template <typename TargetUnit, typename T = double>
    [[nodiscard]] constexpr std::optional<TargetUnit> checked_from_si(const T si_value) noexcept
    {
        static_assert(std::is_integral_v<typename TargetUnit::value_t>,
                      "checked_from_si requires an integral target value type.");
        using compute_t = computation_t<T>;
        using inverse_t = scale::quotient<scale::one, typename TargetUnit::magnitude_t>;
        constexpr auto inverse_factor = static_cast<compute_t>(inverse_t::value);
        constexpr auto offset = static_cast<compute_t>(TargetUnit::offset_to_si);
        auto result = static_cast<compute_t>(si_value);
        if constexpr (offset != static_cast<compute_t>(0))
            result -= offset;

        if constexpr (inverse_factor != static_cast<compute_t>(1))
            result *= inverse_factor;

        return detail::checked_integral_value<TargetUnit>(std::round(result));
    }
}

/// @brief The literal suffixes of the library, each named after the symbol of the unit it builds preceded
/// by an underscore, such as `_m`, `_K` or `_MB`.
/// @details They are the terse way to write a value: `10_kV` rather than a named constructor. They live in
/// a namespace of their own so that including a unit header does not bring the whole set into scope; a
/// caller opts in with `using namespace kmx::literals`. A value built from anything but a literal names its
/// type instead, as in `distance::meter<double> {measured}`, which is also how a value type other than
/// `double` is chosen.
namespace kmx::literals
{
}
