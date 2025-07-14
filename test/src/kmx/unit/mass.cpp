/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/mass.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/mass.hpp>

namespace kmx::unit::mass
{
    TEST_CASE("Mass Unit Tests", "[mass]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<gram<>>(_kg(1.2)).as_native(), approx(1200.0));
            REQUIRE_THAT(convert<milligram<>>(_kg(1.2)).as_native(), approx(1200000.0));
            REQUIRE_THAT(convert<kilogram<>>(_g(2500.0)).as_native(), approx(2.5));
            REQUIRE_THAT(convert<kilogram<>>(_mg(500000.0)).as_native(), approx(0.5));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<gram<int>>(_kg(2)).as_native() == 2000);
            REQUIRE(fp_convert<milligram<long>>(_kg(2)).as_native() == 2000000);
        }
    }
}
