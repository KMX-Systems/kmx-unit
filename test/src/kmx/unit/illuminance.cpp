/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/illuminance.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/illuminance.hpp>

namespace kmx::unit::illuminance
{
    TEST_CASE("Illuminance Unit Tests", "[illuminance]")
    {
        const auto val = _lx(500.0);
        REQUIRE(val.value == 500.0);
    }
}
