/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/information.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/information.hpp>

using namespace kmx::literals;

namespace kmx::unit::information
{
    TEST_CASE("Information Unit Tests", "[information]")
    {
        SECTION("Base Conversions")
        {
            REQUIRE(8_b == 1_B);
            REQUIRE(2_B == 16_b);
            REQUIRE_THAT(convert<bit<>>(1.5_B).as_native(), approx(12.0));
        }

        SECTION("Decimal (SI) Conversions")
        {
            REQUIRE(1.0_kb == 1000.0_b);
            REQUIRE(1.0_kB == 8000.0_b);
            REQUIRE(1.0_kB == 1000.0_B);
            REQUIRE(1.0_MB == 1000.0_kB);
            REQUIRE(1.0_MB == 8e6_b);
        }

        SECTION("Binary (IEC) Conversions")
        {
            REQUIRE(1.0_Kib == 1024.0_b);
            REQUIRE(1.0_KiB == 8192.0_b);
            REQUIRE(1.0_KiB == 1024.0_B);
            REQUIRE(1.0_MiB == 1024.0_KiB);
            REQUIRE(1.0_MiB == bit<> {8.0 * 1024 * 1024});
        }

        SECTION("Every prefix divides into the bit exactly")
        {
            // The prefixes are whole numbers and are kept as such, so no conversion in the family rounds.
            static_assert(convert<bit<>>(1.0_B).as_native() == 8.0);
            static_assert(convert<byte<>>(1.0_KiB).as_native() == 1024.0);
            static_assert(convert<byte<>>(1.0_MiB).as_native() == 1048576.0);
            static_assert(convert<byte<>>(1.0_GiB).as_native() == 1073741824.0);
            static_assert(convert<byte<>>(1.0_kB).as_native() == 1000.0);
            static_assert(convert<kilobyte<>>(1.0_MB).as_native() == 1000.0);
            static_assert(1.0_B == 8.0_b);
            static_assert(1.0_KiB == 1024.0_B);
            static_assert(1.0_MB == 1000.0_kB);
        }

        SECTION("Arithmetic and Floating Point Correctness")
        {
            // 1 kilobyte (8000 bits) + 24 bits = 8024 bits.
            const auto result1 = 1_kB + 24_b;
            const auto result1_in_bits = convert<bit<>>(result1);
            REQUIRE_THAT(result1_in_bits.as_native(), approx(8024.0));

            // 1 kibibyte (8192 bits) + 24 bits = 8216 bits.
            const auto result2 = 1_KiB + 24_b;
            const auto result2_in_bits = convert<bit<>>(result2);
            REQUIRE_THAT(result2_in_bits.as_native(), approx(8216.0));

            // 1 kibibyte + 1 kilobyte = 1024 bytes + 1000 bytes = 2024 bytes
            const auto result3 = 1_KiB + 1_kB;
            const auto result3_in_bytes = convert<byte<>>(result3);
            REQUIRE_THAT(result3_in_bytes.as_native(), approx(2024.0));
        }
    }

    TEST_CASE("Large Information Unit Tests (Tera/Peta)", "[information][large]")
    {
        SECTION("Tera-scale Conversions")
        {
            REQUIRE(1.0_Tb == 1000.0_Gb);
            REQUIRE(1.0_TB == 1000.0_GB);
            REQUIRE(1.0_Tib == 1024.0_Gib);
            REQUIRE(1.0_TiB == 1024.0_GiB);

            // Compare a "1 terabyte" hard drive to its actual tebibyte capacity
            const auto hdd_advertised = 1.0_TB;
            const auto hdd_actual_in_TiB = convert<tebibyte<>>(hdd_advertised);
            // 1,000,000,000,000 / (1024^4) = 0.909...
            REQUIRE_THAT(hdd_actual_in_TiB.as_native(), approx(0.909495));
        }

        SECTION("Peta-scale Conversions")
        {
            REQUIRE(1.0_Pb == 1000.0_Tb);
            REQUIRE(1.0_PB == 1000.0_TB);
            REQUIRE(1.0_Pib == 1024.0_Tib);
            REQUIRE(1.0_PiB == 1024.0_TiB);
        }
    }
}
