/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/temperature.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/temperature.hpp>

namespace kmx::unit::temperature
{
    TEST_CASE("Temperature Unit Tests", "[temperature]")
    {
        SECTION("Float Conversions at known points")
        {
            REQUIRE_THAT(convert<kelvin<>>(_degC(0.0)).as_native(), approx(273.15));
            REQUIRE_THAT(convert<fahrenheit<>>(_degC(0.0)).as_native(), approx(32.0));
            REQUIRE_THAT(convert<celsius<>>(_degF(32.0)).as_native(), approx(0.0));
            REQUIRE_THAT(convert<kelvin<>>(_degF(212.0)).as_native(), approx(373.15));
            REQUIRE_THAT(convert<celsius<>>(_K(0.0)).as_native(), approx(-273.15));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<fahrenheit<int>>(_degC(10)).as_native() == 50);
            REQUIRE(fp_convert<celsius<int>>(_degF(50)).as_native() == 10);
            REQUIRE(fp_convert<celsius<int>>(_K(300)).as_native() == 27);
        }
    }
}
