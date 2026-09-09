/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/ballistics.cpp
/// @brief Exercises the unit families a ballistic solver needs, end to end: explicit construction of the
/// inputs, normalization to base SI scalars for the kernel, and the return trip into reporting units.
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <sstream>

using namespace kmx::literals;

/// @brief A worked ballistics computation, exercising the library the way a caller would.
namespace kmx::unit::ballistics
{
    using acceleration::standard_gravity;
    using angle::mil;
    using angle::milliradian;
    using angle::moa;
    using distance::inch;
    using distance::meter;
    using distance::millimeter;
    using distance::yard;
    using duration::millisecond;
    using energy::foot_pound;
    using energy::joule;
    using pressure::hectopascal;
    using pressure::inch_of_mercury;
    using temperature::celsius;
    using velocity::feet_per_second;
    using velocity::meters_per_second;

    TEST_CASE("Ballistic Unit Workflow", "[ballistics]")
    {
        // The inputs a shot is described with, each one built in the unit it is quoted in.
        const auto projectile_mass = 168.0_gr;
        const auto muzzle_velocity = 2700.0_fps;
        const auto zero_range = 100.0_yd;
        const auto sight_height = 2.0_in;
        const auto bore_diameter = 0.308_in;
        const auto station_pressure = 29.92_inHg;
        const auto ambient_temperature = 59.0_degF;

        SECTION("Inputs normalize to base SI kernel scalars")
        {
            // This is what a solver kernel receives: plain scalars, already in base SI units.
            REQUIRE_THAT(projectile_mass.as_si(), approx_rel(0.01088621688));
            REQUIRE_THAT(muzzle_velocity.as_si(), approx_rel(822.96));
            REQUIRE_THAT(zero_range.as_si(), approx_rel(91.44));
            REQUIRE_THAT(sight_height.as_si(), approx_rel(0.0508));
            REQUIRE_THAT(bore_diameter.as_si(), approx_rel(0.0078232));
            REQUIRE_THAT(station_pressure.as_si(), approx_rel(101320.74811900274));
            REQUIRE_THAT(ambient_temperature.as_si(), approx_rel(288.15));
            REQUIRE_THAT(standard_gravity.as_si(), approx_rel(9.80665));
        }

        SECTION("Kernel outputs convert back into reporting units")
        {
            // A kernel returns bare SI scalars; from_si names the unit they are read back in.
            REQUIRE_THAT(from_si<inch_of_mercury<>>(101325.0).as_native(), approx(29.9212555797));
            REQUIRE_THAT(from_si<hectopascal<>>(101325.0).as_native(), approx(1013.25));
            REQUIRE_THAT(from_si<celsius<>>(288.15).as_native(), approx(15.0));
            REQUIRE_THAT(from_si<feet_per_second<>>(822.96).as_native(), approx(2700.0));
            REQUIRE_THAT(from_si<yard<>>(91.44).as_native(), approx(100.0));
            REQUIRE_THAT(from_si<millimeter<>>(0.0078232).as_native(), approx(7.8232));
        }

        SECTION("Muzzle energy crosses dimensions and reports in both units")
        {
            const auto muzzle_energy = 0.5 * (projectile_mass * muzzle_velocity * muzzle_velocity);
            static_assert(std::is_same_v<decltype(muzzle_energy)::dimension_t, dimension::energy_t>);

            REQUIRE_THAT(muzzle_energy.as_si(), approx_rel(3686.416831006045));
            REQUIRE_THAT(convert<joule<>>(muzzle_energy).as_native(), approx_rel(3686.416831006045));
            REQUIRE_THAT(convert<foot_pound<>>(muzzle_energy).as_native(), approx_rel(2718.961521008704));
            REQUIRE_THAT(muzzle_energy.as_si(), approx_rel((3686.416831006045_J).as_si()));
        }

        SECTION("Time of flight over the zero range")
        {
            const auto time_of_flight = zero_range / muzzle_velocity;
            static_assert(std::is_same_v<decltype(time_of_flight)::dimension_t, dimension::time_t>);

            REQUIRE_THAT(time_of_flight.as_si(), approx(0.1111111111));
            REQUIRE_THAT(convert<millisecond<>>(time_of_flight).as_native(), approx(111.1111111));
        }

        SECTION("Angular corrections stay unambiguous across MOA, mrad and mil")
        {
            // A drop of half a meter at three hundred meters, expressed as a sight correction.
            const auto correction = angle::atan2((0.5_m).as_si(), (300.0_m).as_si());

            REQUIRE_THAT(convert<milliradian<>>(correction).as_native(), approx(1.6666651235));
            REQUIRE_THAT(convert<moa<>>(correction).as_native(), approx(5.7295726462));
            REQUIRE_THAT(convert<mil<>>(correction).as_native(), approx(1.6976511544));

            // The same correction in three units is one and the same angle.
            REQUIRE_THAT(convert<moa<>>(correction).as_si(), approx_rel(convert<milliradian<>>(correction).as_si()));
            REQUIRE_THAT(convert<mil<>>(correction).as_si(), approx_rel(convert<moa<>>(correction).as_si()));
        }

        SECTION("Subtension invariants hold")
        {
            // One minute of angle subtends 1.047 inches at a hundred yards, not one inch.
            const auto moa_subtension = meter<> {angle::tan(1.0_moa) * zero_range.as_si()};
            REQUIRE_THAT(convert<inch<>>(moa_subtension).as_native(), approx(1.0471975807));
            REQUIRE(convert<inch<>>(moa_subtension) > 1.0_in);

            // One milliradian subtends a tenth of a meter at a hundred meters.
            const auto mrad_subtension = meter<> {angle::tan(1.0_mrad) * (100.0_m).as_si()};
            REQUIRE_THAT(mrad_subtension.as_native(), approx(0.1));
            REQUIRE_THAT(convert<millimeter<>>(mrad_subtension).as_native(), approx(100.0));

            // A NATO mil subtends slightly less than a meter at a kilometer, which is what tells it apart.
            const auto mil_subtension = meter<> {angle::tan(1.0_mil) * (1.0_km).as_si()};
            REQUIRE_THAT(mil_subtension.as_native(), approx(0.9817480197));
            REQUIRE(mil_subtension < 1.0_m);
        }

        SECTION("Invariant checks compare values quoted in different units")
        {
            REQUIRE(zero_range == 91.44_m);
            REQUIRE(muzzle_velocity == 822.96_mps);
            REQUIRE(projectile_mass < 1.0_lb);
            REQUIRE_THAT(bore_diameter.as_si(), approx_rel((7.8232_mm).as_si()));
            REQUIRE(station_pressure < 101325.0_Pa);
            REQUIRE(ambient_temperature == 15.0_degC);
            REQUIRE_THAT((7000.0_gr).as_si(), approx_rel((1.0_lb).as_si()));
        }

        SECTION("Diagnostics print the unit alongside the value")
        {
            std::stringstream text;
            text << projectile_mass << ", " << muzzle_velocity << ", " << station_pressure;
            REQUIRE(text.str() == "168 gr, 2700 ft/s, 29.92 inHg");

            REQUIRE(std::format("{:.1f}", 1.5_moa) == "1.5 MOA");
            REQUIRE(std::format("{:.2f}", convert<foot_pound<>>(3686.416831006045_J)) == "2718.96 ft-lbf");
            REQUIRE(std::format("{}", 59.0_degF) == "59 deg F");
        }
    }
}
