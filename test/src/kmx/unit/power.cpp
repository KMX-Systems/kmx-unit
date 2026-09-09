/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/power.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/power.hpp>
#include <kmx/unit/text.hpp>
#include <sstream>

using namespace kmx::literals;

namespace kmx::unit::power
{
    TEST_CASE("Power Unit Tests", "[power]")
    {
        SECTION("Float Conversions (convert)")
        {
            auto val = 3.5_kW;
            REQUIRE_THAT(convert<watt<>>(val).as_native(), approx(3500.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            auto val = kilowatt<int>(4);
            REQUIRE(fp_convert<watt<int>>(val).as_native() == 4000);
        }

        SECTION("Large Scale Conversions")
        {
            REQUIRE(1.0_MW == 1000.0_kW);
            REQUIRE(1.0_GW == 1000.0_MW);
            REQUIRE(1.0_TW == 1000.0_GW);
            REQUIRE(1.0_PW == 1000.0_TW);

            REQUIRE(2.5_GW == 2.5e9_W);
            REQUIRE(0.5_PW == 500000000.0_MW);

            auto val = 1.2_TW;
            REQUIRE_THAT(convert<megawatt<>>(val).as_native(), approx(1200000.0));
        }

        SECTION("Streaming")
        {
            std::stringstream ss;
            ss << 1.21_GW;
            REQUIRE(ss.str() == "1.21 GW");
        }
    }
}
