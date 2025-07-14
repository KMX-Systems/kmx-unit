/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/testing.hpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace kmx::unit
{
    // Helper for comparing floating point values in tests
    inline auto approx(const auto val)
    {
        return Catch::Matchers::WithinAbs(val, 1e-4);
    }
}
