/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/frequency.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

namespace kmx::unit::frequency
{
    TEST_CASE("Frequency Unit Tests", "[frequency]")
    {
        SECTION("Conversions")
        {
            REQUIRE_THAT(convert<hertz<>>(2.5_kHz).as_native(), approx(2500.0));
            REQUIRE_THAT(convert<megahertz<>>(1.0_GHz).as_native(), approx(1000.0));
            REQUIRE_THAT(convert<hertz<>>(3000.0_rpm).as_native(), approx(50.0));
            REQUIRE_THAT(convert<revolutions_per_minute<>>(50.0_Hz).as_native(), approx(3000.0));
        }

        SECTION("A frequency is the reciprocal of a time")
        {
            // Dividing a plain count by a time gives the dimension of a frequency.
            const auto period = 0.02_s;
            REQUIRE_THAT(convert<hertz<>>(from_si<hertz<>>(1.0 / period.as_si())).as_native(), approx(50.0));
            static_assert(std::is_same_v<hertz<>::dimension_t, dimension::frequency_t>);

            // It shares that dimension with an angular velocity, which is why the two are separate types
            // carrying separate symbols rather than one type.
            static_assert(std::is_same_v<dimension::frequency_t, dimension::angular_velocity_t>);
        }
    }
}
