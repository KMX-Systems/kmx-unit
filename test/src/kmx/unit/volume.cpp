/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/volume.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/volume.hpp>

namespace kmx::unit::volume
{
    TEST_CASE("Volume Unit Tests", "[volume]")
    {
        SECTION("Float Conversions")
        {
            REQUIRE_THAT(convert<cubic_meter<>>(_L(2500.0)).as_native(), approx(2.5));
            REQUIRE_THAT(convert<liter<>>(_m3(1.5)).as_native(), approx(1500.0));
        }
    }
}
