/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/derived.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/acceleration.hpp>
#include <kmx/unit/area.hpp>
#include <kmx/unit/distance.hpp>
#include <kmx/unit/duration.hpp>
#include <kmx/unit/electromagnetism.hpp>
#include <kmx/unit/energy.hpp>
#include <kmx/unit/force.hpp>
#include <kmx/unit/mass.hpp>
#include <kmx/unit/power.hpp>
#include <kmx/unit/pressure.hpp>
#include <kmx/unit/velocity.hpp>

using namespace kmx::literals;

namespace kmx::unit
{
    TEST_CASE("Derived Unit Tests2", "[derived2]")
    {
        using namespace kmx::unit::distance;
        using namespace kmx::unit::duration;
        using namespace kmx::unit::mass;
        using namespace kmx::unit::velocity;
        using namespace kmx::unit::acceleration;
        using namespace kmx::unit::area;
        using namespace kmx::unit::force;
        using namespace kmx::unit::energy;
        using namespace kmx::unit::power;
        using namespace kmx::unit::pressure;
        using namespace kmx::unit::electromagnetism;

        SECTION("Velocity = Distance / Time")
        {
            const auto v1 = 100.0_m / 10.0_s;
            REQUIRE_THAT(v1.as_native(), approx(10.0));
            REQUIRE(std::is_same_v<decltype(v1)::dimension_t, dimension::velocity_t>);
            REQUIRE(v1 == 10.0_mps);

            const auto v2 = 36.0_km / 1.0_h;
            REQUIRE_THAT(convert<meters_per_second<>>(v2).as_native(), approx(10.0));
        }

        SECTION("Acceleration = Velocity / Time")
        {
            const auto a1 = 98.0_mps / 10.0_s;
            REQUIRE_THAT(a1.as_native(), approx(9.8));
            REQUIRE(std::is_same_v<decltype(a1)::dimension_t, dimension::acceleration_t>);
            REQUIRE(a1 == 9.8_mps2);
        }

        SECTION("Area = Distance * Distance")
        {
            const auto a1 = 10.0_m * 5.0_m;
            REQUIRE_THAT(a1.as_native(), approx(50.0));
            REQUIRE(std::is_same_v<decltype(a1)::dimension_t, dimension::area_t>);
            REQUIRE(a1 == 50.0_m2);

            const auto a2 = 3.0_ft * 3.0_ft;
            const auto r2 = 1.0_yd * 1.0_yd;
            REQUIRE_THAT(a2.as_native(), approx(r2.as_native()));
            REQUIRE_THAT(convert<square_meter<>>(a2).as_native(), approx(0.836127));
        }

        SECTION("Force = Mass * Acceleration")
        {
            const auto f1 = 10.0_kg * 9.8_mps2;
            REQUIRE_THAT(f1.as_native(), approx(98.0));
            REQUIRE(std::is_same_v<decltype(f1)::dimension_t, dimension::force_t>);
            REQUIRE(f1 == 98.0_N);

            const auto f2 = 10.0_kg * (98.0_m / 10.0_s / 1.0_s);
            REQUIRE(f2 == 98.0_N);
        }

        SECTION("Pressure = Force / Area")
        {
            const auto p1 = 100.0_N / 10.0_m2;
            REQUIRE_THAT(p1.as_native(), approx(10.0));
            REQUIRE(std::is_same_v<decltype(p1)::dimension_t, dimension::pressure_t>);
            REQUIRE(p1 == 10.0_Pa);
            REQUIRE(p1 == 0.01_kPa);
        }

        SECTION("Energy = Force * Distance")
        {
            const auto e1 = 50.0_N * 10.0_m;
            REQUIRE_THAT(e1.as_native(), approx(500.0));
            REQUIRE(std::is_same_v<decltype(e1)::dimension_t, dimension::energy_t>);
            REQUIRE(e1 == 500.0_J);
        }

        SECTION("Power = Energy / Time")
        {
            const auto p1 = 1000.0_J / 2.0_s;
            REQUIRE_THAT(p1.as_native(), approx(500.0));
            REQUIRE(std::is_same_v<decltype(p1)::dimension_t, dimension::power_t>);
            REQUIRE(p1 == 500.0_W);
            REQUIRE(p1 == 0.5_kW);
        }

        SECTION("Ohm's Law: Resistance = Voltage / Current")
        {
            using namespace voltage;
            using namespace current;
            using namespace resistance;

            const auto r1 = 12.0_V / 1.5_A;
            REQUIRE_THAT(r1.as_native(), approx(8.0));
            REQUIRE(std::is_same_v<decltype(r1)::dimension_t, dimension::resistance_t>);
            REQUIRE(r1 == 8.0_Ohm);

            const auto r2 = 0.22_kV / 100.0_A; // 220V / 100A = 2.2 Ohm
            REQUIRE_THAT(convert<ohm<>>(r2).as_native(), approx(2.2));
        }

        SECTION("Dimensionless quantity")
        {
            const auto d1 = 10.0_m / 2.0_m;
            REQUIRE_THAT(d1.as_native(), approx(5.0));
            REQUIRE(std::is_same_v<decltype(d1)::dimension_t, dimension::dimensionless_t>);

            const auto d2 = 10.0_kg / 2000.0_g; // 10kg / 2kg = 5
            REQUIRE_THAT(d2.as_native(), approx(5.0));
        }
    }
}
