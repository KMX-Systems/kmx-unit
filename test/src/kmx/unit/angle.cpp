/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/angle.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/angle.hpp>

namespace kmx::unit::angle
{
    TEST_CASE("Angle Unit Tests", "[angle]")
    {
        SECTION("Float Conversions (convert)")
        {
            REQUIRE_THAT(convert<degree<>>(_rad(std::numbers::pi)).as_native(), approx(180.0));
            REQUIRE_THAT(convert<radian<>>(_deg(90.0)).as_native(), approx(std::numbers::pi / 2.0));
        }

        SECTION("Arcminute and Arcsecond Conversions")
        {
            REQUIRE(_deg(1.0) == _arcmin(60.0));
            REQUIRE(_arcmin(1.0) == _arcsec(60.0));
            REQUIRE(_deg(1.0) == _arcsec(3600.0));

            REQUIRE_THAT(convert<degree<>>(_arcsec(1800.0)).as_native(), approx(0.5));
            REQUIRE_THAT(convert<radian<>>(_arcsec(1.0)).as_native(), approx(4.8481368e-6));
        }

        SECTION("Arithmetic with Arc Units")
        {
            REQUIRE((_arcmin(59.0) + _arcsec(60.0)) == _deg(1.0));
            REQUIRE((_deg(1.5) - _arcmin(30.0)) == _deg(1.0));

            const auto result = _rad(std::numbers::pi) - _deg(179.0);
            REQUIRE_THAT(result.as_si(), approx(_arcmin(60.0).as_si()));
        }

        SECTION("Integer Conversions (fp_convert)")
        {
            REQUIRE(fp_convert<degree<int>>(_rad(1)).as_native() == 57);
            REQUIRE(fp_convert<radian<int>>(_deg(180)).as_native() == 3);
            REQUIRE(fp_convert<arcsecond<long>>(_deg(1)).as_native() == 3600);
            REQUIRE(fp_convert<degree<int>>(_arcmin(120)).as_native() == 2);
        }
    }
}
