/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/duration.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/duration.hpp>

using namespace kmx::literals;

namespace kmx::unit::duration
{
    TEST_CASE("Duration Unit Tests", "[duration]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<minute<>>(3600.0_s).as_native(), approx(60.0));
            REQUIRE_THAT(convert<hour<>>(3600.0_s).as_native(), approx(1.0));
            REQUIRE_THAT(convert<day<>>(24.0_h).as_native(), approx(1.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<hour<long>>(1_d).as_native() == 24);
            REQUIRE(fp_convert<minute<long>>(1_d).as_native() == 1440);
            REQUIRE(fp_convert<second<long>>(1_d).as_native() == 86400);
            REQUIRE(fp_convert<millisecond<long>>(1_s).as_native() == 1000);
            REQUIRE(fp_convert<microsecond<long>>(1_s).as_native() == 1000000);
            REQUIRE(fp_convert<nanosecond<std::int64_t>>(1_s).as_native() == 1000000000LL);
        }
    }
}
