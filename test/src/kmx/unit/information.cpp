/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/information.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/information.hpp>

namespace kmx::unit::information
{
    TEST_CASE("Information Unit Tests", "[information]")
    {
        SECTION("Base Conversions")
        {
            REQUIRE(_b(8) == _B(1));
            REQUIRE(_B(2) == _b(16));
            REQUIRE_THAT(convert<bit<>>(_B(1.5)).as_native(), approx(12.0));
        }

        SECTION("Decimal (SI) Conversions")
        {
            REQUIRE(_kb(1.0) == _b(1000.0));
            REQUIRE(_kB(1.0) == _b(8000.0));
            REQUIRE(_kB(1.0) == _B(1000.0));
            REQUIRE(_MB(1.0) == _kB(1000.0));
            REQUIRE(_MB(1.0) == _b(8e6));
        }

        SECTION("Binary (IEC) Conversions")
        {
            REQUIRE(_Kib(1.0) == _b(1024.0));
            REQUIRE(_KiB(1.0) == _b(8192.0));
            REQUIRE(_KiB(1.0) == _B(1024.0));
            REQUIRE(_MiB(1.0) == _KiB(1024.0));
            REQUIRE(_MiB(1.0) == _b(8.0 * 1024 * 1024));
        }

        SECTION("Arithmetic and Floating Point Correctness")
        {
            // 1 kilobyte (8000 bits) + 24 bits = 8024 bits.
            const auto result1 = _kB(1) + _b(24);
            const auto result1_in_bits = convert<bit<>>(result1);
            REQUIRE_THAT(result1_in_bits.as_native(), approx(8024.0));

            // 1 kibibyte (8192 bits) + 24 bits = 8216 bits.
            const auto result2 = _KiB(1) + _b(24);
            const auto result2_in_bits = convert<bit<>>(result2);
            REQUIRE_THAT(result2_in_bits.as_native(), approx(8216.0));

            // 1 kibibyte + 1 kilobyte = 1024 bytes + 1000 bytes = 2024 bytes
            const auto result3 = _KiB(1) + _kB(1);
            const auto result3_in_bytes = convert<byte<>>(result3);
            REQUIRE_THAT(result3_in_bytes.as_native(), approx(2024.0));
        }
    }

    TEST_CASE("Large Information Unit Tests (Tera/Peta)", "[information][large]")
    {
        SECTION("Tera-scale Conversions")
        {
            REQUIRE(_Tb(1.0) == _Gb(1000.0));
            REQUIRE(_TB(1.0) == _GB(1000.0));
            REQUIRE(_Tib(1.0) == _Gib(1024.0));
            REQUIRE(_TiB(1.0) == _GiB(1024.0));

            // Compare a "1 terabyte" hard drive to its actual tebibyte capacity
            const auto hdd_advertised = _TB(1.0);
            const auto hdd_actual_in_TiB = convert<tebibyte<>>(hdd_advertised);
            // 1,000,000,000,000 / (1024^4) = 0.909...
            REQUIRE_THAT(hdd_actual_in_TiB.as_native(), approx(0.909495));
        }

        SECTION("Peta-scale Conversions")
        {
            REQUIRE(_Pb(1.0) == _Tb(1000.0));
            REQUIRE(_PB(1.0) == _TB(1000.0));
            REQUIRE(_Pib(1.0) == _Tib(1024.0));
            REQUIRE(_PiB(1.0) == _TiB(1024.0));
        }
    }
}
