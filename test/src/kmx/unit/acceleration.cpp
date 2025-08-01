/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/acceleration.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/acceleration.hpp>

namespace kmx::unit::acceleration
{
    TEST_CASE("Acceleration Unit Tests", "[acceleration]")
    {
        const auto a1 = _mps2(9.8);
        REQUIRE_THAT(a1.as_native(), approx(9.8));
        REQUIRE_THAT(a1.as_si(), approx(9.8));
    }
}
