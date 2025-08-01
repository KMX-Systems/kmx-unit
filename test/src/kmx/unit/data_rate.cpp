/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/data_rate.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/data_rate.hpp>
#include <kmx/unit/duration.hpp>

namespace kmx::unit::data_rate
{
    using namespace kmx::unit::information;
    using namespace kmx::unit::duration;

    TEST_CASE("Data Rate Unit Tests", "[data_rate]")
    {
        SECTION("Derived Rate Creation")
        {
            const auto rate1 = _Mb(100.0) / _s(1.0); // 100 Mbps
            REQUIRE(std::is_same_v<decltype(rate1)::dimension, dimension>);
            REQUIRE_THAT(rate1.as_native(), approx(100e6)); // native is bps

            // Check against convenience types
            REQUIRE(rate1 == _Mbps(100.0));
            REQUIRE(rate1 == _MBps(12.5)); // 100 / 8
        }

        SECTION("Conversions")
        {
            const auto my_connection = _Mbps(100.0);
            REQUIRE_THAT(convert<megabytes_per_second<>>(my_connection).as_native(), approx(12.5));

            const auto file_download_speed = _MiBps(20.0); // 20 MiB/s
            // 20 * 8 * 1024 * 1024 bits per second
            // (20 * 8 * 1024 * 1024) / 1,000,000 megabits per second
            REQUIRE_THAT(convert<megabits_per_second<>>(file_download_speed).as_native(), approx(167.77216));
        }

        SECTION("Arithmetic")
        {
            // How long to download a 1 GiB file over a 100 Mbps connection?
            const auto fileSize = _GiB(1.0);
            const auto netSpeed = _Mbps(100.0);

            // The library can't divide information by data_rate yet, so we do it manually
            // This would require another operator overload for (info / data_rate) -> time
            const auto timeInSeconds = fileSize.as_si() / netSpeed.as_si();

            REQUIRE_THAT(timeInSeconds, approx(85.89934)); // ~86 seconds
        }
    }

    TEST_CASE("Large Data Rate Tests (Giga)", "[data_rate][large]")
    {
        SECTION("Derived Rate Creation")
        {
            const auto rate1 = _Gb(10.0) / _s(1.0); // 10 Gbps
            REQUIRE(rate1 == _Gbps(10.0));
            REQUIRE(rate1 == _MBps(1250.0)); // 10,000 / 8
        }

        SECTION("Arithmetic")
        {
            // How long to download a 50 TiB data store over a 10 Gbps connection?
            const auto data_store_size = _TiB(50.0);
            const auto network_speed = _Gbps(10.0);

            const auto time_in_seconds = data_store_size.as_si() / network_speed.as_si();
            const auto time_in_days = convert<day<>>(second<>(time_in_seconds));

            // (50 * tebi * 8) / (10 * giga) seconds
            // = (50 * 1024^4 * 8) / (10 * 10^9) seconds
            // = 43980.465111 seconds
            // = 0.509033 days
            REQUIRE_THAT(time_in_days.as_native(), approx(0.509033));
        }
    }
}
