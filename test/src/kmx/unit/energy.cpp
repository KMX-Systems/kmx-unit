/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/energy.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/energy.hpp>

namespace kmx::unit::energy
{
    TEST_CASE("Energy Unit Tests", "[energy]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilojoule<>>(_J(3.6e6)).as_native(), approx(3600.0));
            REQUIRE_THAT(convert<kilowatt_hour<>>(_J(3.6e6)).as_native(), approx(1.0));
            REQUIRE_THAT(convert<megawatt_hour<>>(_kWh(2000.0)).as_native(), approx(2.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<kilowatt_hour<int>>(_MWh(1)).as_native() == 1000);
            REQUIRE(fp_convert<kilojoule<long>>(_MWh(1)).as_native() == 3600000);
            REQUIRE(fp_convert<kilowatt_hour<int>>(_kJ(7200)).as_native() == 2);
        }
    }
}
