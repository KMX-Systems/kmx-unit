/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/properties.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <format>

using namespace kmx::literals;

/// @brief The tests asserting the properties the library guarantees: layout, constexpr use and safety.
namespace kmx::unit::properties
{
    /// @brief Satisfied when a meter may hold a value of this type.
    /// @tparam T Candidate value type.
    template <typename T>
    concept holds_a_meter = requires { typename distance::meter<T>; };

    /// @brief Satisfied when two values may be compared within a tolerance expressed in @p Tolerance.
    /// @tparam U The unit of both operands.
    /// @tparam Tolerance Candidate tolerance unit.
    template <typename U, typename Tolerance>
    concept can_measure_within = requires(const U value, const Tolerance tolerance) { near(value, value, tolerance); };

    /// @brief Satisfied when a value may be converted into a target unit type.
    /// @tparam TargetUnit The candidate target unit type.
    /// @tparam SourceUnit The candidate source unit type.
    template <typename TargetUnit, typename SourceUnit>
    concept can_convert_to = same_quantity<TargetUnit, SourceUnit> &&
                             (is_absolute_scale_v<TargetUnit> == is_absolute_scale_v<SourceUnit>);

    namespace custom_quantity_kinds
    {
        struct survey_distance_kind_tag
        {
        };

        struct layout_distance_kind_tag
        {
        };

        KMX_UNIT_DEFINE_EX(survey_meter, dimension::length_t, scale::one, std::ratio<0>, "sv-m",
                           using quantity_kind_t = survey_distance_kind_tag;)
        KMX_UNIT_DEFINE_EX(survey_kilometer, dimension::length_t, scale::kilo, std::ratio<0>, "sv-km",
                           using quantity_kind_t = survey_distance_kind_tag;)
        KMX_UNIT_DEFINE_EX(layout_meter, dimension::length_t, scale::one, std::ratio<0>, "ly-m",
                           using quantity_kind_t = layout_distance_kind_tag;)
    }

    using distance::meter;
    using distance::yard;
    using mass::grain;
    using temperature::celsius;

    // A unit value holds its native value and nothing else, so construction and conversion can neither
    // allocate nor reach any storage beyond the value itself.
    static_assert(sizeof(meter<double>) == sizeof(double));
    static_assert(sizeof(grain<float>) == sizeof(float));
    static_assert(sizeof(meter<std::int32_t>) == sizeof(std::int32_t));
    static_assert(std::is_standard_layout_v<meter<double>>);
    static_assert(std::is_trivially_copyable_v<meter<double>>);
    static_assert(std::is_trivially_destructible_v<meter<double>>);
    static_assert(std::is_nothrow_default_constructible_v<meter<double>>);
    static_assert(meter<double> {}.as_native() == 0.0); // default construction zero-initializes

    // Construction, normalization and conversion are all usable at compile time.
    static_assert((100.0_yd).as_native() == 100.0);
    static_assert((1.0_ft).as_si() == 0.3048);
    static_assert(convert<meter<>>(100.0_yd).as_native() == 91.44);
    static_assert(from_si<yard<>>(91.44).as_native() == 100.0);
    static_assert((10.0_m + 5.0_m).as_native() == 15.0);
    static_assert((10.0_m * 3.0).as_native() == 30.0);
    static_assert(100.0_yd == 300.0_ft);
    static_assert(0.0_degC == 273.15_K);
    static_assert(angle::radians(180.0_deg) == std::numbers::pi);

    // Every operation on the value itself is declared not to throw.
    static_assert(noexcept(1.0_m));
    static_assert(noexcept((1.0_m).as_native()));
    static_assert(noexcept((1.0_m).as_si()));
    static_assert(noexcept(convert<yard<>>(1.0_m)));
    static_assert(noexcept(from_si<yard<>>(1.0)));
    static_assert(noexcept(1.0_m + 1.0_m));
    static_assert(noexcept(1.0_m == 1.0_ft));

    // The unit concepts describe the library types and reject everything else.
    static_assert(unit_type<meter<double>>);
    static_assert(unit_type<si_unit<dimension::length_t>>);
    static_assert(!unit_type<double>);
    static_assert(same_dimension<meter<double>, yard<float>>);
    static_assert(!same_dimension<meter<double>, grain<double>>);

    TEST_CASE("Quality Property Tests", "[properties]")
    {
        SECTION("A suffix builds a double, and the type is named to build anything else")
        {
            // A literal operator receives only a long double or an unsigned long long, so a suffix cannot
            // carry the value type. Both spellings therefore yield the same double-valued unit.
            static_assert(std::is_same_v<decltype(1.0_m), meter<double>>);
            static_assert(std::is_same_v<decltype(1_m), meter<double>>);

            // Naming the type is what selects another value type.
            static_assert(std::is_same_v<decltype(meter<float> {1.0f}), meter<float>>);
            static_assert(std::is_same_v<decltype(meter<std::int16_t> {1}), meter<std::int16_t>>);
            static_assert(std::is_same_v<decltype(meter<std::uint64_t> {1u}), meter<std::uint64_t>>);
        }

        SECTION("A unit value holds an arithmetic value, and never a truth value")
        {
            // The constraint is on the class template, so it holds however the type is named rather than
            // only through one entry point, and it is detectable rather than a hard error.
            static_assert(holds_a_meter<double>);
            static_assert(holds_a_meter<float>);
            static_assert(holds_a_meter<std::int32_t>);
            static_assert(!holds_a_meter<bool>);
            static_assert(!holds_a_meter<const char*>);
            static_assert(!holds_a_meter<std::string_view>);
        }

        SECTION("A value is scaled by a number, and never by a truth value")
        {
            // A bool is arithmetic, so every scalar operation used to accept one. Its arithmetic is a
            // conversion to zero or one rather than a scale: `10.0_m * true` read as a factor and was a
            // copy, and `10.0_m * false` silently discarded the value.
            static_assert(!can_scale_by<meter<double>, bool>);
            static_assert(!can_descale_by<meter<double>, bool>);
            static_assert(!can_prescale_by<bool, meter<double>>);
            static_assert(!can_scale_assign_by<meter<double>, bool>);
            static_assert(!can_descale_assign_by<meter<double>, bool>);
            static_assert(!can_invert_by<bool, meter<double>>);

            // Every other arithmetic type still scales, on either side and in place.
            static_assert(can_scale_by<meter<double>, double>);
            static_assert(can_scale_by<meter<double>, std::int32_t>);
            static_assert(can_descale_by<meter<double>, float>);
            static_assert(can_prescale_by<double, meter<double>>);
            static_assert(can_scale_assign_by<meter<double>, double>);
            static_assert(can_descale_assign_by<meter<double>, double>);
            static_assert(can_invert_by<double, duration::second<double>>);

            // A char is arithmetic and keeps scaling; only the truth value is out.
            static_assert(can_scale_by<meter<double>, char>);
        }

        SECTION("The version is readable by the preprocessor and by the code, and the two agree")
        {
            static_assert(version == KMX_UNIT_VERSION);
            static_assert(version == KMX_UNIT_VERSION_NUMBER(version_major, version_minor, version_patch));
            static_assert(version_major == KMX_UNIT_VERSION_MAJOR);
            static_assert(version_minor == KMX_UNIT_VERSION_MINOR);
            static_assert(version_patch == KMX_UNIT_VERSION_PATCH);

            // A release is ordered against another, which is the whole point of the combined number.
            static_assert(KMX_UNIT_VERSION >= KMX_UNIT_VERSION_NUMBER(1, 0, 0));
            static_assert(KMX_UNIT_VERSION_NUMBER(1, 2, 0) > KMX_UNIT_VERSION_NUMBER(1, 1, 99));
            static_assert(KMX_UNIT_VERSION_NUMBER(2, 0, 0) > KMX_UNIT_VERSION_NUMBER(1, 99, 99));

            REQUIRE(version_text == std::format("{}.{}.{}", version_major, version_minor, version_patch));
        }

        SECTION("An operation keeps the value type of its operands")
        {
            // Two floating point operands keep their own common type. A float value used to widen to
            // double, which changed the type of the result and stopped it from being stored back.
            static_assert(std::is_same_v<decltype(meter<float> {} + meter<float> {}), meter<float>>);
            static_assert(std::is_same_v<decltype(meter<float> {} + yard<float> {}), meter<float>>);
            static_assert(std::is_same_v<decltype(meter<float> {} - meter<float> {}), meter<float>>);
            static_assert(std::is_same_v<decltype(meter<double> {} + meter<float> {}), meter<double>>);
            static_assert(std::is_same_v<decltype(meter<double> {} + meter<double> {}), meter<double>>);

            meter<float> accumulator {};
            accumulator = accumulator + meter<float> {1.5f};
            REQUIRE_THAT(accumulator.as_native(), approx(1.5));

            // Two values of the SAME unit need no scale factor, so an integral value type is exact and is
            // kept. It is promoted only when a factor really has to be applied, that factor being a double.
            static_assert(std::is_same_v<decltype(meter<std::int32_t> {} + meter<std::int32_t> {}), meter<std::int32_t>>);
            static_assert(std::is_same_v<decltype(meter<std::int32_t> {} + yard<std::int32_t> {}), meter<double>>);

            // Keeping the type is what keeps the value: a double cannot hold every std::int64_t.
            constexpr std::int64_t past_the_mantissa = 9007199254740993LL; // two to the fifty-third, plus one
            const auto exact = meter<std::int64_t> {past_the_mantissa} + meter<std::int64_t> {};
            REQUIRE(exact.as_native() == past_the_mantissa);
        }

        SECTION("A conversion never divides by a truncated scale factor")
        {
            // An integral argument used to truncate the scale factor of the target to zero, so this was a
            // division by zero rather than a conversion.
            REQUIRE_THAT(from_si<yard<>>(5).as_native(), approx(5.468066));
            REQUIRE_THAT(from_si<yard<>>(5.0).as_native(), approx(5.468066));
            REQUIRE_THAT(meter<std::int32_t> {3}.as_si(), approx(3.0));
        }

        SECTION("Values of different value types interoperate")
        {
            REQUIRE(yard<float> {100.0f} == 91.44_m);
            REQUIRE(fp_convert<meter<int>>(100_yd).as_native() == 91);
            REQUIRE_THAT(convert<meter<>>(yard<float> {100.0f}).as_native(), approx(91.44));

            const auto rounded = checked_fp_convert<meter<int>>(100_yd);
            REQUIRE(rounded.has_value());
            REQUIRE(rounded->as_native() == 91);
            REQUIRE(checked_fp_convert<meter<int>>(1.0e100_m) == std::nullopt);
            REQUIRE(checked_fp_convert<meter<unsigned int>>(-1.0_m) == std::nullopt);
            REQUIRE(checked_fp_convert<meter<int>>(std::numeric_limits<double>::infinity() * 1.0_m) == std::nullopt);
            REQUIRE(checked_fp_convert<meter<int>>(std::numeric_limits<double>::quiet_NaN() * 1.0_m) == std::nullopt);
            REQUIRE(checked_from_si<meter<std::uint64_t>>(18446744073709551616.0) == std::nullopt);

            const auto from_si = checked_from_si<temperature::celsius<int>>(273.15);
            REQUIRE(from_si.has_value());
            REQUIRE(from_si->as_native() == 0);
            REQUIRE(checked_from_si<meter<int>>(1.0e100) == std::nullopt);
            REQUIRE(checked_from_si<meter<int>>(std::numeric_limits<double>::quiet_NaN()) == std::nullopt);
        }

        SECTION("Scaling a value keeps its unit")
        {
            const auto doubled = 10.0_m * 2.0;
            static_assert(std::is_same_v<std::remove_const_t<decltype(doubled)>, meter<double>>);
            REQUIRE_THAT(doubled.as_native(), approx(20.0));

            const auto commuted = 2.0 * 10.0_m;
            static_assert(std::is_same_v<std::remove_const_t<decltype(commuted)>, meter<double>>);
            REQUIRE_THAT(commuted.as_native(), approx(20.0));

            auto scaled = 10.0_m;
            scaled *= 3.0;
            REQUIRE_THAT(scaled.as_native(), approx(30.0));
            scaled /= 2.0;
            REQUIRE_THAT(scaled.as_native(), approx(15.0));
        }

        SECTION("Units that divide into one another compare exactly")
        {
            // A pound is exactly seven thousand grains, and the two sizes are kept exact, so the pair
            // reduces to the whole number 7000 and both values reach grains without a rounded factor.
            REQUIRE(1.0_lb == 7000.0_gr);
            static_assert(1.0_lb == 7000.0_gr);
            static_assert(7000.0_gr == 1.0_lb); // the reversed candidate is the same computation
            static_assert(1.0_ft == 12.0_in);
            static_assert(1.0_mi == 1760.0_yd);
            static_assert(1.0_km == 1000.0_m);
            static_assert(1.0_h == 3600.0_s);
            static_assert(1.0_deg == 60.0_arcmin);
            static_assert(1.0_B == 8.0_b);
            static_assert(1.0_KiB == 1024.0_B);

            // The conversion between such a pair is exact as well, where dividing one rounded size by
            // another used to land at 6999.9999999999991.
            static_assert(convert<grain<>>(1.0_lb).as_native() == 7000.0);
            static_assert(convert<mass::pound<>>(7000.0_gr).as_native() == 1.0);
            static_assert(convert<distance::inch<>>(1.0_ft).as_native() == 12.0);
        }

        SECTION("A unit may declare a custom quantity kind")
        {
            using custom_quantity_kinds::layout_meter;
            using custom_quantity_kinds::survey_kilometer;
            using custom_quantity_kinds::survey_meter;

            // A user-defined kind is an ordinary type tag: equal tags interoperate, different tags do not.
            static_assert(same_kind<survey_meter<double>, survey_kilometer<double>>);
            static_assert(!same_kind<survey_meter<double>, layout_meter<double>>);
            static_assert(!same_kind<survey_meter<double>, meter<double>>);

            static_assert(can_convert_to<survey_kilometer<>, survey_meter<>>);
            static_assert(!can_convert_to<meter<>, survey_meter<>>);

            // A generic SI value remains a wildcard kind and can be read back as any length convention.
            static_assert(can_convert_to<survey_meter<>, si_unit<dimension::length_t>>);
            static_assert(can_convert_to<si_unit<dimension::length_t>, survey_meter<>>);

            const auto converted = convert<survey_kilometer<>>(survey_meter<> {1250.0});
            REQUIRE_THAT(converted.as_native(), approx(1.25));
        }

        SECTION("A tolerance answers what exact equality cannot")
        {
            // Two units that do NOT divide into one another meet in base SI units instead, each through
            // its own rounded factor, so their equality is an ordinary floating point comparison.
            REQUIRE(near(1.0_lb, 0.45359237_kg, 1.0_ug));

            // A zero tolerance is exact equality, so the two never disagree about a pair they both accept.
            REQUIRE(near(1.0_lb, 7000.0_gr, 0.0_gr));
            REQUIRE(near(1.0_m, yard<> {1.0 / 0.9144}, 0.001_mm));
            static_assert(near(1.0_lb, 7000.0_gr, 0.001_gr));

            // A negative tolerance admits nothing, not even a value compared with itself.
            REQUIRE_FALSE(near(1.0_m, 1.0_m, -1.0_mm));

            // A reading on a scale with an origin is compared to within a difference, never a reading.
            REQUIRE(near(celsius<double> {100.0}, temperature::fahrenheit<double> {212.0}, temperature::delta_kelvin<double> {0.001}));
            static_assert(!can_measure_within<celsius<double>, celsius<double>>);
            static_assert(can_measure_within<celsius<double>, temperature::delta_celsius<double>>);
            static_assert(!can_measure_within<meter<double>, duration::second<double>>);
        }

        SECTION("A tolerance measures the difference exact equality cannot")
        {
            // Every other `near` in this suite is answered by one of the two early outs, an exact equality
            // or a negative tolerance, so these are the ones that actually measure a difference.
            REQUIRE(near(1.0_m, meter<> {1.5}, 0.5_m));        // exactly on the boundary
            REQUIRE(near(meter<> {1.5}, 1.0_m, 0.5_m));        // and with the difference the other way round
            REQUIRE_FALSE(near(1.0_m, meter<> {1.5}, 0.25_m)); // outside it
            REQUIRE_FALSE(near(meter<> {1.5}, 1.0_m, 0.25_m));

            // Across two units, where the difference is not a whole number in either of them.
            REQUIRE(near(1.0_m, yard<> {1.1}, 0.1_m));
            REQUIRE_FALSE(near(1.0_m, yard<> {1.1}, 0.001_m));

            // Across two scales with an origin, which cancels in the difference.
            REQUIRE(near(celsius<> {20.0}, temperature::fahrenheit<> {68.5}, temperature::delta_kelvin<> {0.5}));
            REQUIRE_FALSE(near(celsius<> {20.0}, temperature::fahrenheit<> {70.0}, temperature::delta_kelvin<> {0.5}));

            static_assert(near(1.0_m, meter<> {1.5}, 0.5_m));
            static_assert(!near(1.0_m, meter<> {1.5}, 0.25_m));
        }

        SECTION("A compound assignment keeps the value type of its left operand")
        {
            // The product is formed in the common type of the value and the scalar, so a factor below one
            // is applied before anything is stored; an integral value type then truncates on the way back.
            meter<int> narrowed {10};
            narrowed *= 0.25;
            REQUIRE(narrowed.as_native() == 2);
            static_assert(std::is_same_v<decltype(narrowed.as_native()), int>);

            // The non-assigning form widens the value type instead of narrowing the result.
            const auto widened = meter<int> {10} * 0.25;
            static_assert(std::is_same_v<std::remove_const_t<decltype(widened)>, meter<double>>);
            REQUIRE_THAT(widened.as_native(), approx(2.5));

            meter<int> divided {10};
            divided /= 4.0;
            REQUIRE(divided.as_native() == 2);
        }

        SECTION("Values of unrelated dimensions never compare or add")
        {
            static_assert(!can_compare<meter<double>, mass::kilogram<double>>);
            static_assert(!can_add<meter<double>, duration::second<double>>);
            static_assert(!can_order<meter<double>, energy::joule<double>>);

            static_assert(can_compare<meter<double>, yard<double>>);
            static_assert(can_add<meter<double>, yard<double>>);
            static_assert(can_order<meter<double>, yard<double>>);
        }
    }
}
