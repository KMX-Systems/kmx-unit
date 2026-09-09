/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/acceleration.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/acceleration.hpp>

using namespace kmx::literals;

namespace kmx::unit::acceleration
{
    TEST_CASE("Acceleration Unit Tests", "[acceleration]")
    {
        SECTION("Native And SI Values")
        {
            const auto a1 = 9.8_mps2;
            REQUIRE_THAT(a1.as_native(), approx(9.8));
            REQUIRE_THAT(a1.as_si(), approx(9.8));
        }

        SECTION("Foot Per Second Squared Conversions")
        {
            REQUIRE_THAT((1.0_fps2).as_si(), approx(0.3048));
            REQUIRE_THAT(convert<feet_per_second_squared<>>(10.0_mps2).as_native(), approx(32.8083989501));
            REQUIRE_THAT(convert<meters_per_second_squared<>>(32.0_fps2).as_native(), approx(9.7536));
            REQUIRE(1.0_fps2 == 0.3048_mps2);
        }

        SECTION("Standard Gravity")
        {
            // The single definition every caller refers to, rather than a literal repeated at each site.
            REQUIRE_THAT(standard_gravity.as_native(), approx(9.80665));
            REQUIRE_THAT(standard_gravity.as_si(), approx(standard_gravity_si));
            REQUIRE_THAT(convert<feet_per_second_squared<>>(standard_gravity).as_native(), approx(32.1740485564));
            static_assert(standard_gravity.as_native() == standard_gravity_si);
        }
    }
}
