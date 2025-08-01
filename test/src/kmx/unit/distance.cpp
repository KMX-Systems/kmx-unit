/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/distance.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/distance.hpp>

namespace kmx::unit::distance
{
    TEST_CASE("Distance Unit Tests", "[distance]")
    {
        SECTION("Comparisons")
        {
            REQUIRE(_m(1609.344) == _mi(1.0));
            REQUIRE(_km(1.609344) == _mi(1.0));
        }

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilometer<>>(_m(1609.344)).as_native(), approx(1.609344));
            REQUIRE_THAT(convert<mile<>>(_km(1.609344)).as_native(), approx(1.0));
            REQUIRE_THAT(convert<mile<>>(_ft(5280.0)).as_native(), approx(1.0));
            REQUIRE_THAT(convert<mile<>>(_yd(1760.0)).as_native(), approx(1.0));
            REQUIRE_THAT(convert<foot<>>(_mi(1.0)).as_native(), approx(5280.0));
            REQUIRE_THAT(convert<meter<>>(_nmi(1.0)).as_native(), approx(1852.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<meter<int>>(_mi(2)).as_native() == 3219);
            REQUIRE(fp_convert<kilometer<int>>(_mi(2)).as_native() == 3);
            REQUIRE(fp_convert<foot<int>>(_mi(2)).as_native() == 10560);
            REQUIRE(fp_convert<yard<int>>(_mi(2)).as_native() == 3520);
        }
    }
}
