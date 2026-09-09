/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/luminous_intensity.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/luminous_intensity.hpp>

using namespace kmx::literals;

namespace kmx::unit::luminous_intensity
{
    TEST_CASE("Luminous Intensity Unit Tests", "[luminous_intensity]")
    {
        const auto val = 100.0_cd;
        REQUIRE(val.as_native() == 100.0);
    }
}
