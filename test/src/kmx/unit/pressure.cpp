/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/pressure.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/pressure.hpp>

namespace kmx::unit::pressure
{
    TEST_CASE("Pressure Unit Tests", "[pressure]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilopascal<>>(_Pa(1.5e6)).as_native(), approx(1500.0));
            REQUIRE_THAT(convert<megapascal<>>(_Pa(1.5e6)).as_native(), approx(1.5));
            REQUIRE_THAT(convert<pascal<>>(_kPa(250.0)).as_native(), approx(250000.0));
            REQUIRE_THAT(convert<megapascal<>>(_kPa(250.0)).as_native(), approx(0.25));
            REQUIRE_THAT(convert<pascal<>>(_MPa(0.75)).as_native(), approx(750000.0));
        }
    }
}
