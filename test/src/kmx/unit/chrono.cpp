/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/chrono.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <chrono>

using namespace kmx::literals;

/// @brief The tests of the conversion between this library and std::chrono.
namespace kmx::unit::chrono_interop
{
    using namespace std::chrono_literals;

    /// @brief Satisfied when a value may be converted into a std::chrono::duration.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept convertible_to_chrono = requires(const Unit unit) { to_chrono(unit); };

    /// @brief Satisfied when a std::chrono::duration may be converted into a unit type.
    /// @tparam Unit Candidate unit type.
    template <typename Unit>
    concept convertible_from_chrono = requires(const std::chrono::seconds value) { from_chrono<Unit>(value); };

    static_assert(from_chrono<duration::second<>>(1s).as_native() == 1.0);
    static_assert(from_chrono<duration::millisecond<>>(1s).as_native() == 1000.0);
    static_assert(to_chrono(duration::second<double> {1.5}).count() == 1.5);
    static_assert(checked_from_chrono<duration::millisecond<int>>(1500ms)->as_native() == 1500);
    static_assert(checked_from_chrono<duration::second<int>>(1500ms)->as_native() == 1);
    static_assert(!checked_from_chrono<duration::second<int>>(std::chrono::hours {1000000}).has_value());
    static_assert(checked_to_chrono<std::chrono::seconds>(1500.0_ms)->count() == 1);
    static_assert(!checked_to_chrono<std::chrono::milliseconds>(duration::second<double> {1.0e100}).has_value());

    TEST_CASE("Chrono Interoperation Tests", "[chrono]")
    {
        SECTION("From std::chrono into a unit of the library")
        {
            REQUIRE_THAT(from_chrono<duration::second<>>(1500ms).as_native(), approx(1.5));
            REQUIRE_THAT(from_chrono<duration::millisecond<>>(2s).as_native(), approx(2000.0));
            REQUIRE_THAT(from_chrono<duration::minute<>>(90s).as_native(), approx(1.5));
            REQUIRE_THAT(from_chrono<duration::hour<>>(std::chrono::minutes {90}).as_native(), approx(1.5));

            REQUIRE(checked_from_chrono<duration::millisecond<int>>(1500ms)->as_native() == 1500);
            REQUIRE(checked_from_chrono<duration::second<int>>(1500ms)->as_native() == 1);
            REQUIRE(checked_from_chrono<duration::second<int>>(std::chrono::hours {1000000}) == std::nullopt);
            REQUIRE(checked_from_chrono<duration::second<int>>(
                        std::chrono::duration<double> {std::numeric_limits<double>::quiet_NaN()}) == std::nullopt);

            // The default target is the second, which represents every duration without loss.
            REQUIRE_THAT(from_chrono(2500us).as_native(), approx(0.0025));
        }

        SECTION("From a unit of the library into std::chrono")
        {
            REQUIRE_THAT(to_chrono(1.5_min).count(), approx(90.0));
            REQUIRE(to_chrono<std::chrono::milliseconds>(1.5_s).count() == 1500);
            REQUIRE(to_chrono<std::chrono::seconds>(2.0_h).count() == 7200);

            // An integral duration truncates, exactly as a narrowing duration_cast does.
            REQUIRE(to_chrono<std::chrono::seconds>(1500.0_ms).count() == 1);

            REQUIRE(checked_to_chrono<std::chrono::seconds>(1500.0_ms)->count() == 1);
            REQUIRE(checked_to_chrono<std::chrono::milliseconds>(duration::second<double> {1.0e100}) == std::nullopt);
            REQUIRE(checked_to_chrono<std::chrono::seconds>(duration::second<double> {
                        std::numeric_limits<double>::quiet_NaN()}) == std::nullopt);
        }

        SECTION("A round trip preserves the value")
        {
            const auto original = 1234567us;
            const auto as_unit = from_chrono<duration::second<>>(original);
            REQUIRE(to_chrono<std::chrono::microseconds>(as_unit).count() == original.count());
        }

        SECTION("Only a time converts")
        {
            static_assert(convertible_to_chrono<duration::second<double>>);
            static_assert(!convertible_to_chrono<distance::meter<double>>);
            static_assert(convertible_from_chrono<duration::second<double>>);
            static_assert(!convertible_from_chrono<distance::meter<double>>);
        }
    }
}
