/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/power.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/power.hpp>

namespace kmx::unit::power
{
    TEST_CASE("Power Unit Tests", "[power]")
    {
        SECTION("Float Conversions (convert)")
        {
            auto val = _kW(3.5);
            REQUIRE_THAT(convert<watt<>>(val).as_native(), approx(3500.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            auto val = kilowatt<int>(4);
            REQUIRE(fp_convert<watt<int>>(val).as_native() == 4000);
        }
    }
}
