/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/distance.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/distance.hpp>

using namespace kmx::literals;

namespace kmx::unit::distance
{
    TEST_CASE("Distance Unit Tests", "[distance]")
    {
        SECTION("Comparisons")
        {
            REQUIRE(1609.344_m == 1.0_mi);
            REQUIRE(1.609344_km == 1.0_mi);
        }

        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilometer<>>(1609.344_m).as_native(), approx(1.609344));
            REQUIRE_THAT(convert<mile<>>(1.609344_km).as_native(), approx(1.0));
            REQUIRE_THAT(convert<mile<>>(5280.0_ft).as_native(), approx(1.0));
            REQUIRE_THAT(convert<mile<>>(1760.0_yd).as_native(), approx(1.0));
            REQUIRE_THAT(convert<foot<>>(1.0_mi).as_native(), approx(5280.0));
            REQUIRE_THAT(convert<meter<>>(1.0_nmi).as_native(), approx(1852.0));
        }

        SECTION("Millimeter and Inch Conversions")
        {
            REQUIRE(1.0_in == 25.4_mm);
            REQUIRE_THAT((1.0_ft).as_si(), approx_rel((12.0_in).as_si()));
            REQUIRE(1.0_yd == 36.0_in);
            REQUIRE(1.0_m == 1000.0_mm);

            REQUIRE_THAT(convert<millimeter<>>(1.0_m).as_native(), approx(1000.0));
            REQUIRE_THAT(convert<inch<>>(1.0_ft).as_native(), approx(12.0));
            REQUIRE_THAT(convert<meter<>>(100.0_in).as_native(), approx(2.54));
            REQUIRE_THAT(convert<inch<>>(25.4_mm).as_native(), approx(1.0));
            REQUIRE_THAT((1.0_in).as_si(), approx(0.0254));
            REQUIRE_THAT((1.0_mm).as_si(), approx(0.001));
        }

        SECTION("Reporting Units From Kernel Output")
        {
            REQUIRE_THAT(from_si<inch<>>(0.0254).as_native(), approx(1.0));
            REQUIRE_THAT(from_si<millimeter<>>(1.0).as_native(), approx(1000.0));
            REQUIRE_THAT(from_si<yard<>>((300.0_yd).as_si()).as_native(), approx(300.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<meter<int>>(2_mi).as_native() == 3219);
            REQUIRE(fp_convert<millimeter<int>>(1_in).as_native() == 25);
            REQUIRE(fp_convert<inch<int>>(3_ft).as_native() == 36);
            REQUIRE(fp_convert<kilometer<int>>(2_mi).as_native() == 3);
            REQUIRE(fp_convert<foot<int>>(2_mi).as_native() == 10560);
            REQUIRE(fp_convert<yard<int>>(2_mi).as_native() == 3520);
        }
    }
}
