/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/area.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/area.hpp>

using namespace kmx::literals;

namespace kmx::unit::area
{
    TEST_CASE("Area Unit Tests", "[area]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<hectare<>>(15000.0_m2).as_native(), approx(1.5));
            REQUIRE_THAT(convert<are<>>(15000.0_m2).as_native(), approx(150.0));
            REQUIRE_THAT(convert<square_meter<>>(1.5_ha).as_native(), approx(15000.0));
            REQUIRE_THAT(convert<are<>>(1.5_ha).as_native(), approx(150.0));
            REQUIRE_THAT(convert<square_meter<>>(150.0_a).as_native(), approx(15000.0));
            REQUIRE_THAT(convert<square_meter<>>(5.7_ha).as_native(), approx(57000.0));
            REQUIRE_THAT(convert<hectare<>>(150.0_a).as_native(), approx(1.5));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<square_meter<int>>(2_ha).as_native() == 20000);
            REQUIRE(fp_convert<are<int>>(2_ha).as_native() == 200);
        }
    }
}
