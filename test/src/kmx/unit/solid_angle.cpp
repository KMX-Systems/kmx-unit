/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/solid_angle.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

namespace kmx::unit::solid_angle
{
    TEST_CASE("Solid Angle Unit Tests", "[solid_angle]")
    {
        SECTION("Conversions")
        {
            REQUIRE_THAT(convert<millisteradian<>>(0.5_sr).as_native(), approx(500.0));
            REQUIRE_THAT(convert<steradian<>>(1.0_deg2).as_native(), approx(0.00030462));

            // The whole sphere is 4·pi steradians.
            REQUIRE_THAT(steradian<> {4.0 * std::numbers::pi}.as_si(), approx(12.566371));
        }

        SECTION("A solid angle is dimensionless, and told apart by its tag")
        {
            static_assert(std::is_same_v<steradian<>::dimension_t, dimension::dimensionless_t>);
            static_assert(solid_angle_unit<steradian<double>>);
            static_assert(!solid_angle_unit<angle::radian<double>>);
            static_assert(!angle::angle_unit<steradian<double>>);
        }
    }
}
