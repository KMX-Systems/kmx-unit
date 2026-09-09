/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/math.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

/// @brief The tests of the mathematical functions that carry a unit through.
namespace kmx::unit::maths
{
    using distance::meter;
    using distance::yard;

    static_assert(abs(-3.5_m).as_native() == 3.5);
    static_assert(abs(3.5_m).as_native() == 3.5);
    static_assert(abs(meter<std::int32_t> {-3}).as_native() == 3u);
    static_assert(abs(meter<std::int32_t> {std::numeric_limits<std::int32_t>::min()}).as_native() ==
                  static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) + 1u);
    static_assert(std::is_same_v<decltype(abs(meter<std::int32_t> {})), meter<std::uint32_t>>);
    static_assert(pow<2>(3.0_m).as_native() == 9.0);
    static_assert(pow<0>(3.0_m).as_native() == 1.0);
    static_assert(pow<-1>(4.0_s).as_native() == 0.25);
    static_assert(pow<10>(2.0_m).as_native() == 1024.0);
    static_assert(std::is_same_v<decltype(pow<2>(1.0_m))::dimension_t, dimension::area_t>);
    static_assert(std::is_same_v<decltype(pow<3>(1.0_m))::dimension_t, dimension::volume_t>);
    static_assert(std::is_same_v<decltype(sqrt(1.0_m * 1.0_m))::dimension_t, dimension::length_t>);
    static_assert(std::is_same_v<decltype(cbrt(pow<3>(1.0_m)))::dimension_t, dimension::length_t>);

    // An absolute reading has no magnitude, no root and no power: all three follow the origin of its scale.
    static_assert(!can_abs<temperature::celsius<double>>);
    static_assert(!can_sqrt<temperature::celsius<double>>);
    static_assert(!can_pow<temperature::celsius<double>>);
    static_assert(can_abs<temperature::delta_celsius<double>>);
    static_assert(can_sqrt<distance::meter<double>>);
    static_assert(can_pow<distance::meter<double>>);

    TEST_CASE("Mathematical Function Tests", "[math]")
    {
        SECTION("Magnitude")
        {
            REQUIRE_THAT(abs(-3.5_m).as_native(), approx(3.5));
            REQUIRE_THAT(abs(-4.0_delta_degC).as_native(), approx(4.0));
            REQUIRE(abs(meter<std::int32_t> {std::numeric_limits<std::int32_t>::min()}).as_native() ==
                    static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) + 1u);
            static_assert(std::is_same_v<decltype(abs(1.0_m)), meter<double>>);
        }

        SECTION("Smallest and largest, across units of one dimension")
        {
            // The result is expressed in the unit of the left operand, whichever value wins.
            REQUIRE_THAT(min(2.0_m, 1.0_yd).as_native(), approx(0.9144));
            REQUIRE_THAT(max(2.0_m, 3.0_yd).as_native(), approx(2.7432));
            REQUIRE_THAT(min(3.0_yd, 2.0_m).as_native(), approx(2.1872));
            static_assert(std::is_same_v<decltype(min(1.0_m, 1.0_yd)), meter<double>>);

            // Two readings order the same way, and a reading never mixes with a difference.
            REQUIRE_THAT(max(20.0_degC, 80.0_degF).as_native(), approx(26.6667));
            static_assert(!can_min<temperature::celsius<double>, temperature::delta_celsius<double>>);
        }

        SECTION("Confining a value to a range")
        {
            REQUIRE_THAT(clamp(5.0_m, 1.0_m, 3.0_yd).as_native(), approx(2.7432));
            REQUIRE_THAT(clamp(0.5_m, 1.0_m, 3.0_m).as_native(), approx(1.0));
            REQUIRE_THAT(clamp(2.0_m, 1.0_m, 3.0_m).as_native(), approx(2.0));
        }

        SECTION("Hypotenuse")
        {
            REQUIRE_THAT(hypot(3.0_m, 4.0_m).as_native(), approx(5.0));
            REQUIRE_THAT(hypot(3.0_m, 4.0_yd).as_native(), approx(4.730530));
            static_assert(!can_hypot<temperature::celsius<double>, temperature::celsius<double>>);

            // The two sides must be the same KIND of quantity and not merely the same dimension, exactly as
            // min and max require. A torque and an energy shared a dimension and used to pass here alone.
            static_assert(can_hypot<energy::joule<double>, energy::joule<double>>);
            static_assert(can_hypot<torque::newton_meter<double>, torque::newton_meter<double>>);
            static_assert(!can_hypot<energy::joule<double>, torque::newton_meter<double>>);
            static_assert(!can_min<energy::joule<double>, torque::newton_meter<double>>);
            static_assert(!can_hypot<angle::radian<double>, solid_angle::steradian<double>>);
            static_assert(!can_hypot<frequency::hertz<double>, angular_velocity::radian_per_second<double>>);
        }

        SECTION("Roots and powers carry the dimension with them")
        {
            const auto side = sqrt(3.0_m * 12.0_m);
            REQUIRE_THAT(convert<meter<>>(side).as_native(), approx(6.0));

            const auto edge = cbrt(2.0_m * 2.0_m * 2.0_m);
            REQUIRE_THAT(convert<meter<>>(edge).as_native(), approx(2.0));

            REQUIRE_THAT(pow<3>(2.0_m).as_native(), approx(8.0));
            REQUIRE_THAT(pow<-1>(4.0_s).as_native(), approx(0.25));

            // The square root of a volume is a length to the power 3/2, an exact dimension rather than an
            // approximation, because the exponents are ratios.
            using half_powered_t = decltype(sqrt(pow<3>(1.0_m)))::dimension_t;
            static_assert(std::is_same_v<dimension::params<half_powered_t>::l_t, std::ratio<3, 2>>);
        }
    }

    TEST_CASE("Compound Assignment Tests", "[math]")
    {
        SECTION("A value accumulates in its own unit")
        {
            auto total = 10.0_m;
            total += 100.0_yd;
            REQUIRE_THAT(total.as_native(), approx(101.44));
            total -= 0.1_km;
            REQUIRE_THAT(total.as_native(), approx(1.44));
            static_assert(std::is_same_v<decltype(total), meter<double>>);
        }

        SECTION("A reading takes a difference, and only a difference")
        {
            auto reading = 20.0_degC;
            reading += 5.0_delta_degC;
            REQUIRE_THAT(reading.as_native(), approx(25.0));
            reading -= 9.0_delta_degF;
            REQUIRE_THAT(reading.as_native(), approx(20.0));

            static_assert(!can_add_assign<temperature::celsius<>, temperature::celsius<>>);
            static_assert(!can_add_assign<temperature::delta_celsius<>, temperature::celsius<>>);
            static_assert(can_add_assign<temperature::celsius<>, temperature::delta_celsius<>>);
            static_assert(!can_subtract_assign<temperature::celsius<>, temperature::celsius<>>);
        }

        SECTION("An integral value type keeps its own type")
        {
            auto steps = meter<std::int32_t> {10};
            steps += meter<std::int32_t> {5};
            REQUIRE(steps.as_native() == 15);
            static_assert(std::is_same_v<decltype(steps), meter<std::int32_t>>);
        }
    }
}
