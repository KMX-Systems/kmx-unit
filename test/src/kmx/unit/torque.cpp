/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/torque.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>

using namespace kmx::literals;

namespace kmx::unit::torque
{
    TEST_CASE("Torque Unit Tests", "[torque]")
    {
        SECTION("Conversions")
        {
            REQUIRE_THAT(convert<newton_meter<>>(1.5_kNm).as_native(), approx(1500.0));
            REQUIRE_THAT(convert<newton_meter<>>(1.0_lbfft).as_native(), approx(1.3558179));
            REQUIRE_THAT(convert<pound_force_inch<>>(1.0_lbfft).as_native(), approx(12.0));
        }

        SECTION("A torque shares the dimension of an energy, and is told apart by its type")
        {
            static_assert(std::is_same_v<dimension::torque_t, dimension::energy_t>);
            static_assert(torque_unit<newton_meter<double>>);
            static_assert(!torque_unit<energy::joule<double>>);

            // The dimension being shared is not enough to make the two one quantity: the kind keeps them
            // apart, so a torque and an energy neither compare nor add, and neither converts into the other.
            static_assert(!can_compare<newton_meter<double>, energy::joule<double>>);
            static_assert(quantity_kind_of<newton_meter<double>> == quantity_kind::torque);
            static_assert(quantity_kind_of<energy::joule<double>> == quantity_kind::plain);

            // A product of a force and a distance is generic, so it still reads back as either.
            const auto moment = force::newton<double> {2.0} * distance::meter<double> {3.0};
            REQUIRE_THAT(convert<newton_meter<double>>(moment).as_native(), approx(6.0));
            REQUIRE_THAT(convert<energy::joule<double>>(moment).as_native(), approx(6.0));
        }

        SECTION("A torque prints as a torque")
        {
            REQUIRE(std::format("{:.1f}", 12.0_Nm) == "12.0 N·m");
            REQUIRE(std::format("{:.0f}", 9.0_lbfft) == "9 lbf·ft");
        }
    }
}
