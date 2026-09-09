/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/force.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/force.hpp>

using namespace kmx::literals;

namespace kmx::unit::force
{
    TEST_CASE("Force Unit Tests", "[force]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilonewton<>>(980.665_N).as_native(), approx(0.980665));
            REQUIRE_THAT(convert<kilogram_force<>>(980.665_N).as_native(), approx(100.0));
            REQUIRE_THAT(convert<newton<>>(2.5_kN).as_native(), approx(2500.0));
            REQUIRE_THAT(convert<kilogram_force<>>(2.5_kN).as_native(), approx(254.929));
            REQUIRE_THAT(convert<newton<>>(10.0_kgf).as_native(), approx(98.0665));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<newton<int>>(3_kN).as_native() == 3000);
            REQUIRE(fp_convert<kilogram_force<int>>(3_kN).as_native() == 306);
        }
    }
}
