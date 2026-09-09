/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/amount.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/amount.hpp>
#include <kmx/unit/distance.hpp>

using namespace kmx::literals;

namespace kmx::unit::amount
{
    TEST_CASE("Amount Of Substance Unit Tests", "[amount]")
    {
        SECTION("Conversions")
        {
            REQUIRE_THAT(convert<millimole<>>(2.0_mol).as_native(), approx(2000.0));
            REQUIRE_THAT(convert<mole<>>(500.0_mmol).as_native(), approx(0.5));
            REQUIRE_THAT(convert<micromole<>>(1.0_mmol).as_native(), approx(1000.0));
            REQUIRE_THAT(convert<mole<>>(0.25_kmol).as_native(), approx(250.0));
        }

        SECTION("The dimension is the sixth base quantity of the SI")
        {
            static_assert(std::is_same_v<mole<>::dimension_t, dimension::amount_of_substance_t>);
            static_assert(!can_add<mole<>, distance::meter<>>);
            REQUIRE_THAT((3.0_mol).as_si(), approx(3.0));
        }
    }
}
