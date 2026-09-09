/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/velocity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/velocity.hpp>

using namespace kmx::literals;

namespace kmx::unit::velocity
{
    TEST_CASE("Velocity Unit Tests", "[velocity]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<miles_per_hour<>>(100.0_mps).as_native(), approx(223.693629205));
            REQUIRE_THAT(convert<knot<>>(100.0_mps).as_native(), approx(194.3845));
            REQUIRE_THAT(convert<kilometers_per_hour<>>(10.0_mps).as_native(), approx(36.0));

            REQUIRE_THAT(convert<meters_per_second<>>(60.0_mph).as_native(), approx(26.8224));
            REQUIRE_THAT(convert<knot<>>(60.0_mph).as_native(), approx(52.1385));

            REQUIRE_THAT(convert<meters_per_second<>>(100.0_kn).as_native(), approx(51.4444));
            REQUIRE_THAT(convert<miles_per_hour<>>(100.0_kn).as_native(), approx(115.078));

            REQUIRE_THAT(convert<meters_per_second<>>(100.0_kmh).as_native(), approx(27.7778));
            REQUIRE_THAT(convert<miles_per_hour<>>(100.0_kmh).as_native(), approx(62.1371));
        }

        SECTION("Foot Per Second Conversions")
        {
            REQUIRE_THAT((1.0_fps).as_si(), approx(0.3048));
            REQUIRE_THAT(convert<meters_per_second<>>(2700.0_fps).as_native(), approx(822.96));
            REQUIRE_THAT(convert<feet_per_second<>>(100.0_mps).as_native(), approx(328.083989501));
            REQUIRE_THAT(convert<feet_per_second<>>(60.0_mph).as_native(), approx(88.0));
            REQUIRE_THAT((88.0_fps).as_si(), approx_rel((60.0_mph).as_si()));
        }

        SECTION("Comparisons")
        {
            REQUIRE(36.0_kmh == 10.0_mps);
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<miles_per_hour<int>>(30_mps).as_native() == 67);
            REQUIRE(fp_convert<knot<int>>(30_mps).as_native() == 58);
            REQUIRE(fp_convert<kilometers_per_hour<int>>(20_mps).as_native() == 72);

            REQUIRE(fp_convert<meters_per_second<int>>(100_mph).as_native() == 45);
            REQUIRE(fp_convert<knot<int>>(100_mph).as_native() == 87);

            REQUIRE(fp_convert<meters_per_second<int>>(120_kmh).as_native() == 33);
        }
    }
}
