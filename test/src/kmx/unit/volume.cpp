/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/volume.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/volume.hpp>

using namespace kmx::literals;

namespace kmx::unit::volume
{
    TEST_CASE("Volume Unit Tests", "[volume]")
    {
        SECTION("Float Conversions")
        {
            REQUIRE_THAT(convert<cubic_meter<>>(2500.0_L).as_native(), approx(2.5));
            REQUIRE_THAT(convert<liter<>>(1.5_m3).as_native(), approx(1500.0));
        }
    }
}
