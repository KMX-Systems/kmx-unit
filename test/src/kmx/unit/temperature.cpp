/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/temperature.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/distance.hpp>
#include <kmx/unit/duration.hpp>
#include <kmx/unit/energy.hpp>
#include <kmx/unit/temperature.hpp>

using namespace kmx::literals;

namespace kmx::unit::temperature
{
    TEST_CASE("Temperature Unit Tests", "[temperature]")
    {
        SECTION("Float Conversions at known points")
        {
            REQUIRE_THAT(convert<kelvin<>>(0.0_degC).as_native(), approx(273.15));
            REQUIRE_THAT(convert<fahrenheit<>>(0.0_degC).as_native(), approx(32.0));
            REQUIRE_THAT(convert<celsius<>>(32.0_degF).as_native(), approx(0.0));
            REQUIRE_THAT(convert<kelvin<>>(212.0_degF).as_native(), approx(373.15));
            REQUIRE_THAT(convert<celsius<>>(0.0_K).as_native(), approx(-273.15));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<fahrenheit<int>>(10_degC).as_native() == 50);
            REQUIRE(fp_convert<celsius<int>>(50_degF).as_native() == 10);
            REQUIRE(fp_convert<celsius<int>>(300_K).as_native() == 27);
        }

        SECTION("A Reading Normalizes Through Its Origin")
        {
            REQUIRE_THAT((15.0_degC).as_si(), approx(288.15));
            REQUIRE_THAT((59.0_degF).as_si(), approx(288.15));
            REQUIRE_THAT((288.15_K).as_si(), approx(288.15));
            REQUIRE_THAT(from_si<celsius<>>(288.15).as_native(), approx(15.0));
            REQUIRE_THAT(from_si<fahrenheit<>>(288.15).as_native(), approx(59.0));
        }

        SECTION("The two scales convert onto their whole numbers")
        {
            // The size and the origin of each scale are exact, so a conversion between two of them lands on
            // the whole number the definition gives rather than beside it.
            static_assert(convert<celsius<>>(32.0_degF).as_native() == 0.0);
            static_assert(convert<celsius<>>(212.0_degF).as_native() == 100.0);
            static_assert(convert<fahrenheit<>>(0.0_degC).as_native() == 32.0);
            static_assert(convert<fahrenheit<>>(15.0_degC).as_native() == 59.0);
            static_assert(convert<fahrenheit<>>(100.0_degC).as_native() == 212.0);
            static_assert(convert<fahrenheit<>>(-40.0_degC).as_native() == -40.0);
            static_assert(convert<kelvin<>>(0.0_degC).as_native() == 273.15);
            static_assert(convert<celsius<>>(273.15_K).as_native() == 0.0);

            // Two readings compare in one of their own scales, so the same pairs compare equal.
            static_assert(59.0_degF == 15.0_degC);
            static_assert(15.0_degC == 59.0_degF); // the reversed candidate is the same computation
            static_assert(212.0_degF == 100.0_degC);
            static_assert(-40.0_degF == -40.0_degC);
            static_assert(0.0_degC == 273.15_K);
        }

        SECTION("A Difference Normalizes As A Step")
        {
            // One degree Celsius of difference is one kelvin of difference, unlike a reading_t.
            REQUIRE_THAT((10.0_delta_degC).as_si(), approx(10.0));
            REQUIRE_THAT((10.0_delta_K).as_si(), approx(10.0));
            REQUIRE_THAT((9.0_delta_degF).as_si(), approx(5.0));
            REQUIRE(5.0_delta_degC == 9.0_delta_degF);
            REQUIRE_THAT(convert<delta_fahrenheit<>>(100.0_delta_degC).as_native(), approx(180.0));
            REQUIRE_THAT(convert<delta_kelvin<>>(9.0_delta_degF).as_native(), approx(5.0));
        }

        SECTION("Subtracting Two Readings Yields A Difference")
        {
            const auto step_t = 25.0_degC - 10.0_degC;
            static_assert(std::is_same_v<std::remove_const_t<decltype(step_t)>, delta_celsius<double>>);
            REQUIRE_THAT(step_t.as_native(), approx(15.0));

            const auto mixed = 25.0_degC - 50.0_degF;
            static_assert(std::is_same_v<std::remove_const_t<decltype(mixed)>, delta_celsius<double>>);
            REQUIRE_THAT(mixed.as_native(), approx(15.0));

            const auto in_kelvin = 300.0_K - 20.0_degC;
            static_assert(std::is_same_v<std::remove_const_t<decltype(in_kelvin)>, delta_kelvin<double>>);
            REQUIRE_THAT(in_kelvin.as_native(), approx(6.85));

            const auto in_fahrenheit = 80.0_degF - 50.0_degF;
            static_assert(std::is_same_v<std::remove_const_t<decltype(in_fahrenheit)>, delta_fahrenheit<double>>);
            REQUIRE_THAT(in_fahrenheit.as_native(), approx(30.0));
            REQUIRE_THAT(in_fahrenheit.as_si(), approx(16.6666667));
        }

        SECTION("A Reading Shifted By A Difference Stays A Reading")
        {
            const auto warmer = 15.0_degC + 10.0_delta_degC;
            static_assert(std::is_same_v<std::remove_const_t<decltype(warmer)>, celsius<double>>);
            REQUIRE_THAT(warmer.as_native(), approx(25.0));
            REQUIRE_THAT(warmer.as_si(), approx(298.15));

            const auto cooler = 15.0_degC - 9.0_delta_degF;
            static_assert(std::is_same_v<std::remove_const_t<decltype(cooler)>, celsius<double>>);
            REQUIRE_THAT(cooler.as_native(), approx(10.0));

            // The difference may come first; the result is still the reading_t.
            const auto commuted = 10.0_delta_degC + 15.0_degC;
            static_assert(std::is_same_v<std::remove_const_t<decltype(commuted)>, celsius<double>>);
            REQUIRE_THAT(commuted.as_native(), approx(25.0));

            const auto span = 10.0_delta_degC + 9.0_delta_degF;
            static_assert(std::is_same_v<std::remove_const_t<decltype(span)>, delta_celsius<double>>);
            REQUIRE_THAT(span.as_native(), approx(15.0));
        }

        SECTION("Readings And Differences Never Mix Silently")
        {
            using reading_t = celsius<double>;
            using step_t = delta_celsius<double>;

            // Adding two readings, scaling a reading_t, or comparing a reading_t with a difference is rejected.
            static_assert(!can_add<reading_t, reading_t>);
            static_assert(!can_add<kelvin<double>, fahrenheit<double>>);
            static_assert(!can_scale<reading_t>);
            static_assert(!can_descale<reading_t>);
            static_assert(!can_prescale<reading_t>);
            static_assert(!can_compare<reading_t, step_t>);
            static_assert(!can_order<step_t, reading_t>);
            static_assert(!can_subtract<step_t, reading_t>);

            // The arithmetic that carries a meaning on a scale with an origin stays available.
            static_assert(can_subtract<reading_t, reading_t>);
            static_assert(can_add<reading_t, step_t>);
            static_assert(can_add<step_t, reading_t>);
            static_assert(can_scale<step_t>);
            static_assert(can_compare<step_t, delta_fahrenheit<double>>);
            static_assert(can_compare<reading_t, fahrenheit<double>>);

            // A reading_t stays out of a product and of a quotient with another dimension too. Its value
            // carries the origin of its scale, so the result would follow the choice of origin rather than
            // the quantity: 20 degC times 2 s is 40 or 586.3 depending on where the scale starts.
            static_assert(!can_multiply<reading_t, duration::second<double>>);
            static_assert(!can_multiply<duration::second<double>, reading_t>);
            static_assert(!can_multiply<reading_t, reading_t>);
            static_assert(!can_divide<reading_t, distance::meter<double>>);
            static_assert(!can_divide<energy::joule<double>, reading_t>);
            static_assert(!can_divide<kelvin<double>, fahrenheit<double>>);

            // A difference carries no origin, so it combines with any dimension. This is the arithmetic a
            // heat capacity or a lapse rate is written with, and it stays available.
            static_assert(can_multiply<step_t, duration::second<double>>);
            static_assert(can_multiply<duration::second<double>, step_t>);
            static_assert(can_divide<energy::joule<double>, delta_kelvin<double>>);
            static_assert(can_divide<step_t, distance::meter<double>>);

            // The kinds are told apart by the trait, not by the dimension, which they share.
            static_assert(is_absolute_scale_v<celsius<>>);
            static_assert(!is_absolute_scale_v<delta_celsius<>>);
            static_assert(std::is_same_v<celsius<>::dimension_t, delta_celsius<>::dimension_t>);
            static_assert(std::is_same_v<difference_unit_t<fahrenheit<double>>, delta_fahrenheit<double>>);
        }
    }
}
