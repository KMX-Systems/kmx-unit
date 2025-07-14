/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/luminous_intensity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/luminous_intensity.hpp>

namespace kmx::unit::luminous_intensity
{
    TEST_CASE("Luminous Intensity Unit Tests", "[luminous_intensity]")
    {
        auto val = _cd(100.0);
        REQUIRE(val.value == 100.0);
    }
}
