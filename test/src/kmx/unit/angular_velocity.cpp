/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/angular_velocity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/angular_velocity.hpp>

using namespace kmx::literals;

namespace kmx::unit::angular_velocity
{
    TEST_CASE("Angular Velocity Unit Tests", "[angular_velocity]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<degree_per_second<>>(radian_per_second<> {std::numbers::pi}).as_native(), approx(180.0));
            REQUIRE_THAT(convert<radian_per_second<>>(90.0_deg_s).as_native(), approx(std::numbers::pi / 2.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<degree_per_second<int>>(1_rad_s).as_native() == 57);
            REQUIRE(fp_convert<radian_per_second<int>>(180_deg_s).as_native() == 3);
        }
    }
}
