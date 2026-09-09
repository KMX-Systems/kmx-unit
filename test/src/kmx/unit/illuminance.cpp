/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/illuminance.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/illuminance.hpp>

using namespace kmx::literals;

namespace kmx::unit::illuminance
{
    TEST_CASE("Illuminance Unit Tests", "[illuminance]")
    {
        const auto val = 500.0_lx;
        REQUIRE(val.as_native() == 500.0);
    }
}
