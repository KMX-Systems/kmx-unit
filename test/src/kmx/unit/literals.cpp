/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/literals.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

namespace kmx::unit
{
    TEST_CASE("Literal Suffix Tests", "[literals]")
    {
        SECTION("Suffix And Factory Agree")
        {
            // The suffix form is the factory form; neither reads the number as a base SI value.
            REQUIRE(9.8_mps2 == 9.8_mps2);
            REQUIRE(300.0_yd == 300.0_yd);
            REQUIRE(168.0_gr == 168.0_gr);
            REQUIRE(2700.0_fps == 2700.0_fps);
        }

        SECTION("Value Type Is Double For Both Overloads")
        {
            // A literal operator receives only long double or unsigned long long, so the suffix cannot
            // carry the value type the way a factory argument does. Both overloads settle on double.
            STATIC_REQUIRE(std::is_same_v<decltype(9.8_mps2), acceleration::meters_per_second_squared<double>>);
            STATIC_REQUIRE(std::is_same_v<decltype(10_mps2), acceleration::meters_per_second_squared<double>>);
            STATIC_REQUIRE(std::is_same_v<decltype(1_m), distance::meter<double>>);
        }

        SECTION("Usable In Constant Expressions")
        {
            STATIC_REQUIRE(1.0_km == 1000.0_m);
            STATIC_REQUIRE(1.0_kg == 1000.0_g);
            STATIC_REQUIRE(1.0_min == 60.0_s);
            STATIC_REQUIRE(1.0_kN == 1000.0_N);
            STATIC_REQUIRE(1.0_kHz == 1000.0_Hz);
            STATIC_REQUIRE(1.0_MB == 1000.0_kB);
            STATIC_REQUIRE(1.0_fps2 == 0.3048_mps2);
        }

        SECTION("Arithmetic And Conversion")
        {
            REQUIRE(2.0_mps2 + 1.0_mps2 == 3.0_mps2);
            REQUIRE_THAT(convert<acceleration::feet_per_second_squared<>>(9.8_mps2).as_native(), approx(32.1522309711));
            REQUIRE_THAT((10.0_kg * 9.8_mps2).as_si(), approx(98.0));
            REQUIRE_THAT((9.8_mps2 * 3.0_s).as_si(), approx(29.4));
        }

        SECTION("Member Access Needs Parentheses")
        {
            // A ud-suffix lexes greedily: `1.0_K.as_si()` is read as the suffix `_K.as_si`, so a member
            // call on a literal has to parenthesize it.
            REQUIRE_THAT((1.0_K).as_si(), approx(1.0));
            REQUIRE_THAT((0.0_degC).as_si(), approx(273.15));
        }
    }
}
