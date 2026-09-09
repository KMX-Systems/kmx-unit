/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/scale.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

/// @brief The tests of the exact sizes the units are built from, and of the algebra combining them.
namespace kmx::unit::scales
{
    /// @brief Converts and compares one unit against every unit of its family.
    /// @details Both operations form the relative size of the pair, which is where an exact ratio can fail
    /// to fit in a std::intmax_t. That failure is a hard error inside std::ratio rather than a wrong
    /// answer, so it can only be caught by naming every pair. Addition is left out because two readings on
    /// scales with origins do not add, and because it rescales exactly as a conversion does.
    /// @tparam From The unit converted from.
    /// @tparam To The units converted to.
    /// @return True; the point of the function is that it compiles.
    template <typename From, typename... To>
    [[nodiscard]] constexpr bool from_one() noexcept
    {
        return (... && ((convert<To>(From {1.0}).as_native() == convert<To>(From {1.0}).as_native()) &&
                        ((From {1.0} == To {1.0}) || true) && ((From {1.0} < To {2.0}) || true)));
    }

    /// @brief Converts and compares every ordered pair drawn from one family.
    /// @tparam Units The units of the family.
    /// @return True; the point of the function is that it compiles.
    template <typename... Units>
    [[nodiscard]] constexpr bool all_pairs() noexcept
    {
        return (... && from_one<Units, Units...>());
    }

    // Every pair of every family converts, compares and orders. A foot-pound over a kilowatt hour
    // asks for a denominator of nine thousand million million million, which no std::ratio holds; the pair
    // falls back to a rounded size rather than refusing to compile, and these assertions say that it does.
    static_assert(all_pairs<pressure::pascal<>, pressure::hectopascal<>, pressure::kilopascal<>, pressure::megapascal<>, pressure::bar<>,
                            pressure::psi<>, pressure::inch_of_mercury<>, pressure::millimeter_of_mercury<>>());
    static_assert(all_pairs<distance::millimeter<>, distance::meter<>, distance::kilometer<>, distance::inch<>, distance::foot<>,
                            distance::yard<>, distance::mile<>, distance::nautical_mile<>>());
    static_assert(all_pairs<mass::kilogram<>, mass::gram<>, mass::milligram<>, mass::microgram<>, mass::grain<>, mass::pound<>>());
    static_assert(all_pairs<duration::second<>, duration::millisecond<>, duration::microsecond<>, duration::nanosecond<>,
                            duration::minute<>, duration::hour<>, duration::day<>>());
    static_assert(all_pairs<energy::joule<>, energy::kilojoule<>, energy::foot_pound<>, energy::kilowatt_hour<>, energy::megawatt_hour<>,
                            energy::gigawatt_hour<>>());
    static_assert(all_pairs<torque::newton_meter<>, torque::kilonewton_meter<>, torque::pound_force_foot<>, torque::pound_force_inch<>>());
    static_assert(
        all_pairs<angle::radian<>, angle::milliradian<>, angle::mil<>, angle::degree<>, angle::arcsecond<>, angle::arcminute<>>());
    static_assert(all_pairs<velocity::meters_per_second<>, velocity::feet_per_second<>, velocity::kilometers_per_hour<>,
                            velocity::miles_per_hour<>, velocity::knot<>>());
    static_assert(all_pairs<frequency::hertz<>, frequency::kilohertz<>, frequency::megahertz<>, frequency::gigahertz<>,
                            frequency::revolutions_per_minute<>>());
    static_assert(
        all_pairs<information::bit<>, information::byte<>, information::kilobit<>, information::kilobyte<>, information::megabit<>,
                  information::megabyte<>, information::gigabit<>, information::gigabyte<>, information::terabit<>, information::terabyte<>,
                  information::petabit<>, information::petabyte<>, information::kibibit<>, information::kibibyte<>, information::mebibit<>,
                  information::mebibyte<>, information::gibibit<>, information::gibibyte<>, information::tebibit<>, information::tebibyte<>,
                  information::pebibit<>, information::pebibyte<>>());
    static_assert(all_pairs<data_rate::bits_per_second<>, data_rate::megabits_per_second<>, data_rate::megabytes_per_second<>,
                            data_rate::gigabits_per_second<>, data_rate::gigabytes_per_second<>, data_rate::mebibytes_per_second<>,
                            data_rate::gibibytes_per_second<>>());
    static_assert(all_pairs<temperature::delta_kelvin<>, temperature::delta_celsius<>, temperature::delta_fahrenheit<>>());
    static_assert(all_pairs<temperature::kelvin<>, temperature::celsius<>, temperature::fahrenheit<>>());

    /// @brief Multiplies and divides one value by every value of a list.
    /// @details A product and a quotient combine the two sizes, which is the other place an exact ratio can
    /// fail to fit. The list crosses families on purpose: a product carries no dimension restriction, so
    /// the two coarsest sizes of the library can meet in one.
    /// @tparam A The left unit.
    /// @tparam B The right units.
    /// @return True; the point of the function is that it compiles.
    template <typename A, typename... B>
    [[nodiscard]] constexpr bool one_against_all() noexcept
    {
        return (... && (((A {2.0} * B {3.0}).as_native() == (A {2.0} * B {3.0}).as_native()) &&
                        ((A {2.0} / B {3.0}).as_native() == (A {2.0} / B {3.0}).as_native())));
    }

    /// @brief Multiplies and divides every ordered pair drawn from a list of units.
    /// @tparam Units The units to cross.
    /// @return True; the point of the function is that it compiles.
    template <typename... Units>
    [[nodiscard]] constexpr bool cross_product() noexcept
    {
        return (... && one_against_all<Units, Units...>());
    }

    static_assert(cross_product<energy::foot_pound<>, energy::gigawatt_hour<>, energy::kilowatt_hour<>, torque::pound_force_inch<>,
                                torque::pound_force_foot<>, pressure::psi<>, pressure::inch_of_mercury<>, pressure::millimeter_of_mercury<>,
                                information::pebibyte<>, information::petabyte<>, data_rate::gibibytes_per_second<>, duration::nanosecond<>,
                                duration::day<>, distance::mile<>, distance::millimeter<>, mass::microgram<>, mass::grain<>, mass::pound<>,
                                power::petawatt<>, frequency::gigahertz<>, angle::arcsecond<>, solid_angle::square_degree<>,
                                electromagnetism::magnetic_flux_density::gauss<>, velocity::knot<>, area::hectare<>, volume::liter<>>());

    TEST_CASE("Exact Size Tests", "[scale]")
    {
        SECTION("A size is a ratio times a power of pi")
        {
            static_assert(scale::is_one<scale::one>);
            static_assert(!scale::is_one<scale::kilo>);
            static_assert(scale::is_rational<scale::kilo>);
            static_assert(!scale::is_rational<angle::radian_per_degree>);

            // The prefixes are the sizes they are named after.
            static_assert(scale::value_of<double, scale::kilo>() == 1000.0);
            static_assert(scale::value_of<double, scale::milli>() == 0.001);
            static_assert(scale::value_of<double, scale::kibi>() == 1024.0);
            static_assert(scale::value_of<double, scale::mebi>() == 1048576.0);

            // The algebra composes them exactly: a foot is twelve inches and a yard three feet.
            static_assert(std::ratio_equal_v<distance::meter_per_foot::ratio_t, std::ratio<381, 1250>>);
            static_assert(std::ratio_equal_v<distance::meter_per_yard::ratio_t, std::ratio<1143, 1250>>);
            static_assert(std::ratio_equal_v<distance::meter_per_mile::ratio_t, std::ratio<201168, 125>>);

            // The exponent of pi is carried beside the ratio, so it cancels between two angle units.
            static_assert(angle::radian_per_degree::pi_power == 1);
            static_assert(solid_angle::steradian_per_square_degree::pi_power == 2);
            static_assert(scale::divide_t<angle::radian_per_degree, angle::radian_per_arcminute>::pi_power == 0);
        }

        SECTION("A size that does not fit falls back to a rounded number")
        {
            // A foot-pound over a kilowatt hour needs a denominator no std::intmax_t holds.
            using huge_t = scale::quotient<energy::foot_pound<>::magnitude_t, energy::kilowatt_hour<>::magnitude_t>;
            static_assert(!huge_t::is_exact);
            static_assert(!huge_t::is_whole_multiple);
            static_assert(!huge_t::is_unity);

            // The pair still converts, and to the right number.
            REQUIRE_THAT(convert<energy::foot_pound<>>(1.0_kWh).as_native(), approx_rel(2655223.7373981555));
            REQUIRE_THAT(convert<energy::kilowatt_hour<>>(energy::foot_pound<> {1.0e6}).as_native(), approx_rel(0.3766160967594167));
            REQUIRE_THAT(convert<torque::pound_force_inch<>>(torque::kilonewton_meter<> {1.0}).as_native(), approx_rel(8850.745791327185));

            // A pair that does fit keeps its exact size.
            using exact_t = scale::quotient<mass::pound<>::magnitude_t, mass::grain<>::magnitude_t>;
            static_assert(exact_t::is_exact);
            static_assert(exact_t::is_whole_multiple);
            static_assert(exact_t::num == 7000);
            static_assert(exact_t::den == 1);
        }

        SECTION("A conversion between units that divide is exact")
        {
            static_assert(convert<mass::grain<>>(1.0_lb).as_native() == 7000.0);
            static_assert(convert<distance::inch<>>(1.0_ft).as_native() == 12.0);
            static_assert(convert<distance::yard<>>(1.0_mi).as_native() == 1760.0);
            static_assert(convert<duration::second<>>(1.0_h).as_native() == 3600.0);
            static_assert(convert<duration::millisecond<>>(1.0_s).as_native() == 1000.0);
            static_assert(convert<angle::arcsecond<>>(1.0_deg).as_native() == 3600.0);
            static_assert(convert<information::byte<>>(1.0_GiB).as_native() == 1073741824.0);
            static_assert(convert<temperature::fahrenheit<>>(100.0_degC).as_native() == 212.0);
        }

        SECTION("A comparison is symmetric whichever size the pair meets at")
        {
            // Each of the three paths, in both directions: the reversed candidate must compute the same.
            static_assert((1.0_lb == 7000.0_gr) == (7000.0_gr == 1.0_lb));         // a whole multiple
            static_assert((59.0_degF == 15.0_degC) == (15.0_degC == 59.0_degF));   // two scales with origins
            static_assert((1.0_lb == 0.45359237_kg) == (0.45359237_kg == 1.0_lb)); // base SI units
            static_assert((1.0_ftlbf == 1.0_J) == (1.0_J == 1.0_ftlbf));

            // Ordering agrees with equality in each of them.
            static_assert(1.0_lb <= 7000.0_gr);
            static_assert(1.0_lb >= 7000.0_gr);
            static_assert(!(1.0_lb < 7000.0_gr));
            static_assert(2.0_lb > 7000.0_gr);
            static_assert(59.0_degF <= 15.0_degC);
            static_assert(60.0_degF > 15.0_degC);
        }
    }
}
