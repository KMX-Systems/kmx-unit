/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/pressure.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/pressure.hpp>

using namespace kmx::literals;

namespace kmx::unit::pressure
{
    TEST_CASE("Pressure Unit Tests", "[pressure]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<kilopascal<>>(1.5e6_Pa).as_native(), approx(1500.0));
            REQUIRE_THAT(convert<megapascal<>>(1.5e6_Pa).as_native(), approx(1.5));
            REQUIRE_THAT(convert<pascal<>>(250.0_kPa).as_native(), approx(250000.0));
            REQUIRE_THAT(convert<megapascal<>>(250.0_kPa).as_native(), approx(0.25));
            REQUIRE_THAT(convert<pascal<>>(0.75_MPa).as_native(), approx(750000.0));
        }

        SECTION("Hectopascal And Bar Conversions")
        {
            REQUIRE(1.0_bar == 1000.0_hPa);
            REQUIRE(1.0_hPa == 100.0_Pa);
            REQUIRE_THAT((1013.25_hPa).as_si(), approx(101325.0));
            REQUIRE_THAT(convert<hectopascal<>>(101325.0_Pa).as_native(), approx(1013.25));
            REQUIRE_THAT(convert<bar<>>(250.0_kPa).as_native(), approx(2.5));
        }

        SECTION("Field Pressure Units")
        {
            // Every factor follows from exact definitions, so the values below are not approximations
            // of a measurement but of the printed decimal expansion.
            REQUIRE_THAT((1.0_psi).as_si(), approx_rel(6894.757293168361));
            REQUIRE_THAT((1.0_inHg).as_si(), approx_rel(3386.388640341));
            REQUIRE_THAT((1.0_mmHg).as_si(), approx_rel(133.322387415));

            REQUIRE(1.0_inHg == 25.4_mmHg);
            REQUIRE_THAT((14.6959487755_psi).as_si(), approx_rel(101325.0, 1e-9));
            REQUIRE_THAT(convert<psi<>>(101325.0_Pa).as_native(), approx(14.6959487755));
            REQUIRE_THAT(convert<inch_of_mercury<>>(101325.0_Pa).as_native(), approx(29.9212555797));
            REQUIRE_THAT(convert<millimeter_of_mercury<>>(101325.0_Pa).as_native(), approx(759.9998917));
            REQUIRE_THAT(convert<hectopascal<>>(29.92_inHg).as_native(), approx(1013.20748119));
            REQUIRE_THAT(convert<inch_of_mercury<>>(1013.25_hPa).as_native(), approx(29.9212555797));
            REQUIRE_THAT(convert<psi<>>(1.0_bar).as_native(), approx(14.5037737730));
        }

        SECTION("Reporting Units From Kernel Output")
        {
            REQUIRE_THAT(from_si<inch_of_mercury<>>((29.92_inHg).as_si()).as_native(), approx(29.92));
            REQUIRE_THAT(from_si<psi<>>(pascal_per_psi_si).as_native(), approx(1.0));
        }
    }
}
