/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/power.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/power.hpp>
#include <kmx/unit/text.hpp>
#include <sstream>

namespace kmx::unit::power
{
    TEST_CASE("Power Unit Tests", "[power]")
    {
        SECTION("Float Conversions (convert)")
        {
            auto val = _kW(3.5);
            REQUIRE_THAT(convert<watt<>>(val).as_native(), approx(3500.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            auto val = kilowatt<int>(4);
            REQUIRE(fp_convert<watt<int>>(val).as_native() == 4000);
        }

        SECTION("Large Scale Conversions")
        {
            REQUIRE(_MW(1.0) == _kW(1000.0));
            REQUIRE(_GW(1.0) == _MW(1000.0));
            REQUIRE(_TW(1.0) == _GW(1000.0));
            REQUIRE(_PW(1.0) == _TW(1000.0));

            REQUIRE(_GW(2.5) == _W(2.5e9));
            REQUIRE(_PW(0.5) == _MW(500000000.0));

            auto val = _TW(1.2);
            REQUIRE_THAT(convert<megawatt<>>(val).as_native(), approx(1200000.0));
        }

        SECTION("Streaming")
        {
            std::stringstream ss;
            ss << _GW(1.21);
            REQUIRE(ss.str() == "1.21 GW");
        }
    }
}
