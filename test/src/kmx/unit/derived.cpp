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
            const auto v1 = _m(100.0) / _s(10.0);
            REQUIRE_THAT(v1.as_native(), approx(10.0));
            REQUIRE(std::is_same_v<decltype(v1)::dimension, dimension::velocity>);
            REQUIRE(v1 == _mps(10.0));

            const auto v2 = _km(36.0) / _h(1.0);
            REQUIRE_THAT(convert<meters_per_second<>>(v2).as_native(), approx(10.0));
        }

        SECTION("Acceleration = Velocity / Time")
        {
            const auto a1 = _mps(98.0) / _s(10.0);
            REQUIRE_THAT(a1.as_native(), approx(9.8));
            REQUIRE(std::is_same_v<decltype(a1)::dimension, dimension::acceleration>);
            REQUIRE(a1 == _mps2(9.8));
        }

        SECTION("Area = Distance * Distance")
        {
            const auto a1 = _m(10.0) * _m(5.0);
            REQUIRE_THAT(a1.as_native(), approx(50.0));
            REQUIRE(std::is_same_v<decltype(a1)::dimension, dimension::area>);
            REQUIRE(a1 == _m2(50.0));

            const auto a2 = _ft(3.0) * _ft(3.0);
            const auto r2 = _yd(1.0) * _yd(1.0);
            REQUIRE_THAT(a2.value, approx(r2.value));
            REQUIRE_THAT(convert<square_meter<>>(a2).as_native(), approx(0.836127));
        }

        SECTION("Force = Mass * Acceleration")
        {
            const auto f1 = _kg(10.0) * _mps2(9.8);
            REQUIRE_THAT(f1.as_native(), approx(98.0));
            REQUIRE(std::is_same_v<decltype(f1)::dimension, dimension::force>);
            REQUIRE(f1 == _N(98.0));

            const auto f2 = _kg(10.0) * (_m(98.0) / _s(10.0) / _s(1.0));
            REQUIRE(f2 == _N(98.0));
        }

        SECTION("Pressure = Force / Area")
        {
            const auto p1 = _N(100.0) / _m2(10.0);
            REQUIRE_THAT(p1.as_native(), approx(10.0));
            REQUIRE(std::is_same_v<decltype(p1)::dimension, dimension::pressure>);
            REQUIRE(p1 == _Pa(10.0));
            REQUIRE(p1 == _kPa(0.01));
        }

        SECTION("Energy = Force * Distance")
        {
            const auto e1 = _N(50.0) * _m(10.0);
            REQUIRE_THAT(e1.as_native(), approx(500.0));
            REQUIRE(std::is_same_v<decltype(e1)::dimension, dimension::energy>);
            REQUIRE(e1 == _J(500.0));
        }

        SECTION("Power = Energy / Time")
        {
            const auto p1 = _J(1000.0) / _s(2.0);
            REQUIRE_THAT(p1.as_native(), approx(500.0));
            REQUIRE(std::is_same_v<decltype(p1)::dimension, dimension::power>);
            REQUIRE(p1 == _W(500.0));
            REQUIRE(p1 == _kW(0.5));
        }

        SECTION("Ohm's Law: Resistance = Voltage / Current")
        {
            using namespace voltage;
            using namespace current;
            using namespace resistance;

            const auto r1 = _V(12.0) / _A(1.5);
            REQUIRE_THAT(r1.as_native(), approx(8.0));
            REQUIRE(std::is_same_v<decltype(r1)::dimension, dimension::resistance>);
            REQUIRE(r1 == _Ohm(8.0));

            const auto r2 = _kV(0.22) / _A(100.0); // 220V / 100A = 2.2 Ohm
            REQUIRE_THAT(convert<ohm<>>(r2).as_native(), approx(2.2));
        }

        SECTION("Dimensionless quantity")
        {
            const auto d1 = _m(10.0) / _m(2.0);
            REQUIRE_THAT(d1.as_native(), approx(5.0));
            REQUIRE(std::is_same_v<decltype(d1)::dimension, dimension::dimensionless>);

            const auto d2 = _kg(10.0) / _g(2000.0); // 10kg / 2kg = 5
            REQUIRE_THAT(d2.as_native(), approx(5.0));
        }
    }
}
