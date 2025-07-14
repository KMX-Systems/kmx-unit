/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/duration.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/duration.hpp>

namespace kmx::unit::duration
{
    TEST_CASE("Duration Unit Tests", "[duration]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<minute<>>(_s(3600.0)).as_native(), approx(60.0));
            REQUIRE_THAT(convert<hour<>>(_s(3600.0)).as_native(), approx(1.0));
            REQUIRE_THAT(convert<day<>>(_h(24.0)).as_native(), approx(1.0));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<hour<long>>(_d(1)).as_native() == 24);
            REQUIRE(fp_convert<minute<long>>(_d(1)).as_native() == 1440);
            REQUIRE(fp_convert<second<long>>(_d(1)).as_native() == 86400);
            REQUIRE(fp_convert<millisecond<long>>(_s(1)).as_native() == 1000);
            REQUIRE(fp_convert<microsecond<long>>(_s(1)).as_native() == 1000000);
            REQUIRE(fp_convert<nanosecond<std::int64_t>>(_s(1)).as_native() == 1000000000LL);
        }
    }
}
