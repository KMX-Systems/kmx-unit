/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/force.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/force.hpp>

namespace kmx::unit::force
{
    TEST_CASE("Force Unit Tests", "[force]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilonewton<>>(_N(980.665)).as_native(), approx(0.980665));
            REQUIRE_THAT(convert<kilogram_force<>>(_N(980.665)).as_native(), approx(100.0));
            REQUIRE_THAT(convert<newton<>>(_kN(2.5)).as_native(), approx(2500.0));
            REQUIRE_THAT(convert<kilogram_force<>>(_kN(2.5)).as_native(), approx(254.929));
            REQUIRE_THAT(convert<newton<>>(_kgf(10.0)).as_native(), approx(98.0665));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<newton<int>>(_kN(3)).as_native() == 3000);
            REQUIRE(fp_convert<kilogram_force<int>>(_kN(3)).as_native() == 306);
        }
    }
}
