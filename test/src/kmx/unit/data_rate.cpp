/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/data_rate.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/data_rate.hpp>
#include <kmx/unit/duration.hpp>

using namespace kmx::literals;

namespace kmx::unit::data_rate
{
    using namespace kmx::unit::information;
    using namespace kmx::unit::duration;

    TEST_CASE("Data Rate Unit Tests", "[data_rate]")
    {
        SECTION("Derived Rate Creation")
        {
            const auto rate1 = 100.0_Mb / 1.0_s; // 100 Mbps
            REQUIRE(std::is_same_v<decltype(rate1)::dimension_t, dimension_t>);
            REQUIRE_THAT(rate1.as_native(), approx(100e6)); // native is bps

            // Check against convenience types
            REQUIRE(rate1 == 100.0_Mbps);
            REQUIRE(rate1 == 12.5_MBps); // 100 / 8
        }

        SECTION("Conversions")
        {
            const auto my_connection = 100.0_Mbps;
            REQUIRE_THAT(convert<megabytes_per_second<>>(my_connection).as_native(), approx(12.5));

            const auto file_download_speed = 20.0_MiBps; // 20 MiB/s
            // 20 * 8 * 1024 * 1024 bits per second
            // (20 * 8 * 1024 * 1024) / 1,000,000 megabits per second
            REQUIRE_THAT(convert<megabits_per_second<>>(file_download_speed).as_native(), approx(167.77216));
        }

        SECTION("Arithmetic")
        {
            // How long to download a 1 GiB file over a 100 Mbps connection?
            const auto file_size = 1.0_GiB;
            const auto net_speed = 100.0_Mbps;

            // An amount of information divided by a rate is a time, reported as a typed value.
            const auto transfer_time = file_size / net_speed;
            static_assert(std::is_same_v<std::remove_const_t<decltype(transfer_time)>, second<double>>);
            REQUIRE_THAT(transfer_time.as_native(), approx(85.89934)); // ~86 seconds

            // The inverse holds as well: a rate multiplied by a time is the amount carried, in either order.
            const auto carried = net_speed * transfer_time;
            static_assert(std::is_same_v<std::remove_const_t<decltype(carried)>, bit<double>>);
            REQUIRE_THAT(convert<gibibyte<>>(carried).as_native(), approx(1.0));
            REQUIRE_THAT(convert<gibibyte<>>(transfer_time * net_speed).as_native(), approx(1.0));
        }

        SECTION("Information Takes Part In The Ordinary Dimension Algebra")
        {
            // The two families carry dimensions of the library rather than tags of their own, so a rate is
            // an ordinary quotient and any other combination has a dimension too, instead of failing to
            // compile deep inside the dimension arithmetic.
            static_assert(std::is_same_v<information::dimension_t, ::kmx::unit::dimension::information_t>);
            static_assert(std::is_same_v<dimension_t, ::kmx::unit::dimension::data_rate_t>);
            static_assert(
                std::is_same_v<::kmx::unit::dimension::divide_t<information::dimension_t, ::kmx::unit::dimension::time_t>, dimension_t>);
            static_assert(can_multiply<byte<double>, second<double>>);
            static_assert(can_divide<byte<double>, second<double>>);
            static_assert(can_divide<byte<double>, bits_per_second<double>>);

            // Information stays independent of every other dimension, as two SI dimensions are of each other.
            static_assert(!can_add<byte<double>, second<double>>);
            static_assert(!can_compare<byte<double>, bits_per_second<double>>);
        }
    }

    TEST_CASE("Large Data Rate Tests (Giga)", "[data_rate][large]")
    {
        SECTION("Derived Rate Creation")
        {
            const auto rate1 = 10.0_Gb / 1.0_s; // 10 Gbps
            REQUIRE(rate1 == 10.0_Gbps);
            REQUIRE(rate1 == 1250.0_MBps); // 10,000 / 8
        }

        SECTION("Arithmetic")
        {
            // How long to download a 50 TiB data store over a 10 Gbps connection?
            const auto data_store_size = 50.0_TiB;
            const auto network_speed = 10.0_Gbps;

            const auto time_in_days = convert<day<>>(data_store_size / network_speed);

            // (50 * tebi * 8) / (10 * giga) seconds
            // = (50 * 1024^4 * 8) / (10 * 10^9) seconds
            // = 43980.465111 seconds
            // = 0.509033 days
            REQUIRE_THAT(time_in_days.as_native(), approx(0.509033));
        }
    }
}
