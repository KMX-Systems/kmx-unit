/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/angle.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/angle.hpp>

using namespace kmx::literals;

namespace kmx::unit::angle
{
    TEST_CASE("Angle Unit Tests", "[angle]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<degree<>>(radian<> {std::numbers::pi}).as_native(), approx(180.0));
            REQUIRE_THAT(convert<radian<>>(90.0_deg).as_native(), approx(std::numbers::pi / 2.0));
        }

        SECTION("Arcminute and Arcsecond Conversions")
        {
            REQUIRE(1.0_deg == 60.0_arcmin);
            REQUIRE(1.0_arcmin == 60.0_arcsec);
            REQUIRE(1.0_deg == 3600.0_arcsec);

            REQUIRE_THAT(convert<degree<>>(1800.0_arcsec).as_native(), approx(0.5));
            REQUIRE_THAT(convert<radian<>>(1.0_arcsec).as_native(), approx(4.8481368e-6));
        }

        SECTION("Arithmetic with Arc Units")
        {
            REQUIRE((59.0_arcmin + 60.0_arcsec) == 1.0_deg);
            REQUIRE((1.5_deg - 30.0_arcmin) == 1.0_deg);

            const auto result = radian<> {std::numbers::pi} - 179.0_deg;
            REQUIRE_THAT(result.as_si(), approx((60.0_arcmin).as_si()));
        }

        SECTION("MOA, Milliradian And Mil Are Distinct Units")
        {
            // The three units are separate types, none of them an alias of another.
            static_assert(!std::is_same_v<moa<>, arcminute<>>);
            static_assert(!std::is_same_v<milliradian<>, mil<>>);
            static_assert(!std::is_same_v<milliradian<>, radian<>>);

            // A milliradian and a NATO mil are close but not equal.
            REQUIRE(1.0_mrad != 1.0_mil);
            REQUIRE(1.0_mrad > 1.0_mil);
            REQUIRE_THAT((1.0_mrad).as_si(), approx(1.0e-3));
            REQUIRE_THAT((1.0_mil).as_si(), approx_rel(2.0 * std::numbers::pi / 6400.0));
            REQUIRE_THAT(convert<milliradian<>>(1.0_mil).as_native(), approx(0.98174770));

            // A NATO mil divides the full turn into 6400 parts.
            REQUIRE_THAT(convert<mil<>>(radian<> {2.0 * std::numbers::pi}).as_native(), approx_rel(6400.0));

            // A minute of angle is a sixtieth of a degree, the same size as an arcminute.
            REQUIRE(60.0_moa == 1.0_deg);
            REQUIRE(1.0_moa == 1.0_arcmin);
            REQUIRE_THAT((1.0_moa).as_si(), approx_rel(std::numbers::pi / 10800.0));
            REQUIRE_THAT(convert<milliradian<>>(1.0_moa).as_native(), approx(0.29088821));
            REQUIRE_THAT(convert<moa<>>(1.0_mrad).as_native(), approx(3.43774677));
            REQUIRE_THAT(convert<milliradian<>>(1.0_deg).as_native(), approx(17.4532925));
        }

        SECTION("Angle units divide into one another exactly")
        {
            // Every size of the family is a rational multiple of pi, and the pi cancels between any two of
            // them, so the pair reduces to the whole number its definition implies.
            static_assert(convert<arcminute<>>(1.0_deg).as_native() == 60.0);
            static_assert(convert<arcsecond<>>(1.0_deg).as_native() == 3600.0);
            static_assert(convert<arcsecond<>>(1.0_arcmin).as_native() == 60.0);
            static_assert(convert<degree<>>(60.0_arcmin).as_native() == 1.0);
            static_assert(1.0_deg == 60.0_arcmin);
            static_assert(1.0_deg == 3600.0_arcsec);
            static_assert(1.0_arcmin == 60.0_arcsec);

            // A MOA has the size of an arcminute, so the two are the same number of degrees apart.
            static_assert(convert<moa<>>(1.0_deg).as_native() == 60.0);

            // A radian and a degree do not divide into one another: the ratio is pi over 180.
            REQUIRE_THAT(convert<degree<>>(1.0_rad).as_native(), approx(57.29577951308232));
        }

        SECTION("Trigonometric Interoperability")
        {
            // Any angle unit normalizes to radians, the form a computation kernel expects.
            REQUIRE_THAT(radians(180.0_deg), approx(std::numbers::pi));
            REQUIRE_THAT(radians(1.5_rad), approx(1.5));
            REQUIRE_THAT(radians(1.0_moa), approx_rel(std::numbers::pi / 10800.0));

            REQUIRE_THAT(sin(30.0_deg), approx(0.5));
            REQUIRE_THAT(cos(60.0_deg), approx(0.5));
            REQUIRE_THAT(tan(45.0_deg), approx(1.0));
            REQUIRE_THAT(sin(radian<> {std::numbers::pi / 6.0}), approx(0.5));
            REQUIRE_THAT(tan(10.0_mrad), approx(0.01000033));

            // The inverse functions return an angle, never a bare number.
            static_assert(std::is_same_v<decltype(asin(0.5)), radian<double>>);
            REQUIRE_THAT(convert<degree<>>(asin(0.5)).as_native(), approx(30.0));
            REQUIRE_THAT(convert<degree<>>(acos(0.5)).as_native(), approx(60.0));
            REQUIRE_THAT(convert<degree<>>(atan(1.0)).as_native(), approx(45.0));
            REQUIRE_THAT(convert<degree<>>(atan2(1.0, 1.0)).as_native(), approx(45.0));

            // A plain dimensionless ratio is not an angle.
            static_assert(angle_unit<degree<>>);
            static_assert(angle_unit<moa<>>);
            static_assert(!angle_unit<si_unit<dimension::dimensionless_t>>);
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<degree<int>>(1_rad).as_native() == 57);
            REQUIRE(fp_convert<moa<int>>(2_deg).as_native() == 120);
            REQUIRE(fp_convert<milliradian<int>>(10_deg).as_native() == 175);
            REQUIRE(fp_convert<radian<int>>(180_deg).as_native() == 3);
            REQUIRE(fp_convert<arcsecond<long>>(1_deg).as_native() == 3600);
            REQUIRE(fp_convert<degree<int>>(120_arcmin).as_native() == 2);
        }
    }
}
