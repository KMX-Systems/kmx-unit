/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/area.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/area.hpp>

namespace kmx::unit::area
{
    TEST_CASE("Area Unit Tests", "[area]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<hectare<>>(_m2(15000.0)).as_native(), approx(1.5));
            REQUIRE_THAT(convert<are<>>(_m2(15000.0)).as_native(), approx(150.0));
            REQUIRE_THAT(convert<square_meter<>>(_ha(1.5)).as_native(), approx(15000.0));
            REQUIRE_THAT(convert<are<>>(_ha(1.5)).as_native(), approx(150.0));
            REQUIRE_THAT(convert<square_meter<>>(_a(150.0)).as_native(), approx(15000.0));
            REQUIRE_THAT(convert<square_meter<>>(_ha(5.7)).as_native(), approx(57000.0));
            REQUIRE_THAT(convert<hectare<>>(_a(150.0)).as_native(), approx(1.5));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<square_meter<int>>(_ha(2)).as_native() == 20000);
            REQUIRE(fp_convert<are<int>>(_ha(2)).as_native() == 200);
        }
    }
}
