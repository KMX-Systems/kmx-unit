/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/electromagnetism.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/electromagnetism.hpp>

namespace kmx::unit::electromagnetism
{
    TEST_CASE("Current Unit Tests", "[current]")
    {
        using namespace current;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<current::milliampere<>>(_A(1.5)).as_native(), approx(1500.0));
            REQUIRE_THAT(convert<current::kiloampere<>>(_mA(2500.0)).as_native(), approx(0.0025));
            REQUIRE_THAT(convert<current::ampere<>>(_kA(0.5)).as_native(), approx(500.0));
        }
    }

    TEST_CASE("Voltage Unit Tests", "[voltage]")
    {
        using namespace voltage;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<voltage::millivolt<>>(_V(12.0)).as_native(), approx(12000.0));
            REQUIRE_THAT(convert<voltage::volt<>>(_kV(0.4)).as_native(), approx(400.0));
        }
    }

    TEST_CASE("Resistance Unit Tests", "[resistance]")
    {
        using namespace resistance;

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<resistance::kiloohm<>>(_Ohm(2200.0)).as_native(), approx(2.2));
            REQUIRE_THAT(convert<resistance::ohm<>>(_kOhm(3.3)).as_native(), approx(3300.0));
        }
    }
}
