/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/energy.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/energy.hpp>

using namespace kmx::literals;

namespace kmx::unit::energy
{
    TEST_CASE("Energy Unit Tests", "[energy]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilojoule<>>(3.6e6_J).as_native(), approx(3600.0));
            REQUIRE_THAT(convert<kilowatt_hour<>>(3.6e6_J).as_native(), approx(1.0));
            REQUIRE_THAT(convert<megawatt_hour<>>(2000.0_kWh).as_native(), approx(2.0));
        }

        SECTION("Foot-Pound Conversions")
        {
            REQUIRE_THAT((1.0_ftlbf).as_si(), approx_rel(1.3558179483314004));
            REQUIRE_THAT(convert<joule<>>(1000.0_ftlbf).as_native(), approx(1355.8179483314));
            REQUIRE_THAT(convert<foot_pound<>>(1000.0_J).as_native(), approx(737.562149277));
            REQUIRE_THAT(convert<foot_pound<>>(1.0_kJ).as_native(), approx(737.562149277));
            REQUIRE(1.0_ftlbf == joule<> {joule_per_foot_pound_si});
        }

        SECTION("Reporting Units From Kernel Output")
        {
            REQUIRE_THAT(from_si<foot_pound<>>((2500.0_ftlbf).as_si()).as_native(), approx(2500.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<kilowatt_hour<int>>(1_MWh).as_native() == 1000);
            REQUIRE(fp_convert<foot_pound<int>>(1000_J).as_native() == 738);
            REQUIRE(fp_convert<gigawatt_hour<int>>(2000_MWh).as_native() == 2);
            REQUIRE(fp_convert<kilojoule<long>>(1_MWh).as_native() == 3600000);
            REQUIRE(fp_convert<kilowatt_hour<int>>(7200_kJ).as_native() == 2);
        }
    }
}
