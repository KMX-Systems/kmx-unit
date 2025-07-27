/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/velocity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/velocity.hpp>

namespace kmx::unit::velocity
{
    TEST_CASE("Velocity Unit Tests", "[velocity]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<miles_per_hour<>>(_mps(100.0)).as_native(), approx(223.693629205));
            REQUIRE_THAT(convert<knot<>>(_mps(100.0)).as_native(), approx(194.3845));
            REQUIRE_THAT(convert<kilometers_per_hour<>>(_mps(10.0)).as_native(), approx(36.0));

            REQUIRE_THAT(convert<meters_per_second<>>(_mph(60.0)).as_native(), approx(26.8224));
            REQUIRE_THAT(convert<knot<>>(_mph(60.0)).as_native(), approx(52.1385));

            REQUIRE_THAT(convert<meters_per_second<>>(_kn(100.0)).as_native(), approx(51.4444));
            REQUIRE_THAT(convert<miles_per_hour<>>(_kn(100.0)).as_native(), approx(115.078));

            REQUIRE_THAT(convert<meters_per_second<>>(_kmh(100.0)).as_native(), approx(27.7778));
            REQUIRE_THAT(convert<miles_per_hour<>>(_kmh(100.0)).as_native(), approx(62.1371));
        }

        SECTION("Comparisons")
        {
            REQUIRE(_kmh(36.0) == _mps(10.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<miles_per_hour<int>>(_mps(30)).as_native() == 67);
            REQUIRE(fp_convert<knot<int>>(_mps(30)).as_native() == 58);
            REQUIRE(fp_convert<kilometers_per_hour<int>>(_mps(20)).as_native() == 72);

            REQUIRE(fp_convert<meters_per_second<int>>(_mph(100)).as_native() == 45);
            REQUIRE(fp_convert<knot<int>>(_mph(100)).as_native() == 87);

            REQUIRE(fp_convert<meters_per_second<int>>(_kmh(120)).as_native() == 33);
        }
    }
}
