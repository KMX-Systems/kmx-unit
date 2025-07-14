/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/angular_velocity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/angular_velocity.hpp>

namespace kmx::unit::angular_velocity
{
    TEST_CASE("Angular Velocity Unit Tests", "[angular_velocity]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<degree_per_second<>>(_rad_s(std::numbers::pi)).as_native(), approx(180.0));
            REQUIRE_THAT(convert<radian_per_second<>>(_deg_s(90.0)).as_native(), approx(std::numbers::pi / 2.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<degree_per_second<int>>(_rad_s(1)).as_native() == 57);
            REQUIRE(fp_convert<radian_per_second<int>>(_deg_s(180)).as_native() == 3);
        }
    }
}
