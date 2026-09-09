/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/mass.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/mass.hpp>

using namespace kmx::literals;

namespace kmx::unit::mass
{
    TEST_CASE("Mass Unit Tests", "[mass]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<gram<>>(1.2_kg).as_native(), approx(1200.0));
            REQUIRE_THAT(convert<milligram<>>(1.2_kg).as_native(), approx(1200000.0));
            REQUIRE_THAT(convert<kilogram<>>(2500.0_g).as_native(), approx(2.5));
            REQUIRE_THAT(convert<kilogram<>>(500000.0_mg).as_native(), approx(0.5));
        }

        SECTION("Grain And Pound Conversions")
        {
            // Both are exact by definition: a pound is 0.45359237 kg and a grain is a 7000th of it.
            REQUIRE_THAT((1.0_lb).as_si(), approx_rel((7000.0_gr).as_si()));
            REQUIRE_THAT((1.0_lb).as_si(), approx_rel(0.45359237));
            REQUIRE_THAT((1.0_gr).as_si(), approx_rel(0.45359237 / 7000.0));
            REQUIRE_THAT(convert<milligram<>>(1.0_gr).as_native(), approx(64.79891));
            REQUIRE_THAT(convert<gram<>>(168.0_gr).as_native(), approx(10.88621688));
            REQUIRE_THAT(convert<grain<>>(1.0_kg).as_native(), approx(15432.3583529));
            REQUIRE_THAT(convert<kilogram<>>(10.0_lb).as_native(), approx(4.5359237));
            REQUIRE_THAT(convert<pound<>>(1.0_kg).as_native(), approx(2.20462262185));
        }

        SECTION("Reporting Units From Kernel Output")
        {
            REQUIRE_THAT(from_si<grain<>>((230.0_gr).as_si()).as_native(), approx(230.0));
            REQUIRE_THAT(from_si<pound<>>(0.45359237).as_native(), approx(1.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<gram<int>>(2_kg).as_native() == 2000);
            REQUIRE(fp_convert<grain<int>>(1_lb).as_native() == 7000);
            REQUIRE(fp_convert<milligram<int>>(1_gr).as_native() == 65);
            REQUIRE(fp_convert<milligram<long>>(2_kg).as_native() == 2000000);
        }
    }
}
