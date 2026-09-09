/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/electromagnetism.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/electromagnetism.hpp>

using namespace kmx::literals;

namespace kmx::unit::electromagnetism
{
    TEST_CASE("Current Unit Tests", "[current]")
    {
        using namespace current;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<milliampere<>>(1.5_A).as_native(), approx(1500.0));
            REQUIRE_THAT(convert<kiloampere<>>(2500.0_mA).as_native(), approx(0.0025));
            REQUIRE_THAT(convert<ampere<>>(0.5_kA).as_native(), approx(500.0));
        }
    }

    TEST_CASE("Voltage Unit Tests", "[voltage]")
    {
        using namespace voltage;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<millivolt<>>(12.0_V).as_native(), approx(12000.0));
            REQUIRE_THAT(convert<volt<>>(0.4_kV).as_native(), approx(400.0));
        }
    }

    TEST_CASE("Resistance Unit Tests", "[resistance]")
    {
        using namespace resistance;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kiloohm<>>(2200.0_Ohm).as_native(), approx(2.2));
            REQUIRE_THAT(convert<ohm<>>(3.3_kOhm).as_native(), approx(3300.0));
        }
    }
}
