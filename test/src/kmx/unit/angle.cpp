/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/angle.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/angle.hpp>

namespace kmx::unit::angle
{
    TEST_CASE("Angle Unit Tests", "[angle]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<degree<>>(_rad(std::numbers::pi)).as_native(), approx(180.0));
            REQUIRE_THAT(convert<radian<>>(_deg(90.0)).as_native(), approx(std::numbers::pi / 2.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<degree<int>>(_rad(1)).as_native() == 57);
            REQUIRE(fp_convert<radian<int>>(_deg(180)).as_native() == 3);
        }
    }
}
