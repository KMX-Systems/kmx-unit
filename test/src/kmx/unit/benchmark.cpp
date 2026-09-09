/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/benchmark.cpp
/// @brief Measures the cost of the library against the hand written arithmetic it is meant to compile down
/// to. Every case is paired with the raw scalar loop a caller would otherwise write, so the number that
/// matters is the ratio of the two rather than either timing on its own.
/// @details Each case transforms a whole input array into an output array rather than accumulating into a
/// single value. An accumulator makes every iteration depend on the previous one, and the latency of that
/// chain then hides the cost of the arithmetic under measurement; writing independent results measures the
/// throughput of the operation itself.
/// @note These cases carry the `[!benchmark]` tag, so a plain run of the suite skips them. Run them with
/// `unit-test "[benchmark]"`.
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <array>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <format>
#include <vector>

using namespace kmx::literals;

/// @brief The benchmarks comparing the library against the equivalent raw scalar arithmetic.
namespace kmx::unit::benchmarks
{
    /// @brief The number of values every benchmark walks over; the working set stays inside the L1 cache.
    /// @details A case reads one or two input arrays and writes a third, so the working set is up to three
    /// times this count of doubles. At 1024 that is 24 KiB, which fits the 32 KiB L1 data cache of a
    /// typical core; 4096 would be 96 KiB and would measure the L2 bandwidth instead of the arithmetic.
    constexpr std::size_t sample_count = 1024u;

    /// @brief The exact size of one yard in meters, the factor a caller would write by hand.
    constexpr double meter_per_yard = 0.9144;

    /// @brief The exact size of one foot in meters, the factor a caller would write by hand.
    constexpr double meter_per_foot = 0.3048;

    /// @brief Builds the input of a benchmark, filled at run time so that nothing folds at compile time.
    /// @param first The value of the first sample.
    /// @param step The distance between two consecutive samples.
    /// @return The samples.
    static std::vector<double> make_samples(const double first, const double step) noexcept(false)
    {
        std::vector<double> result(sample_count);
        double value = first;
        for (auto& item: result)
        {
            item = value;
            value += step;
        }

        return result;
    }

    TEST_CASE("Addition of two values of the same unit", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.5);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a + b")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] + meter_per_yard;

            return results.back();
        };

        BENCHMARK("unit: meter + meter")
        {
            const distance::meter<double> addend {meter_per_yard};
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::meter<double> {samples[i]} + addend).as_native();

            return results.back();
        };
    }

    TEST_CASE("Addition of two values of different units", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.5);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a + (b * factor)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] + (2.0 * meter_per_yard);

            return results.back();
        };

        BENCHMARK("unit: meter + yard")
        {
            const distance::yard<double> addend {2.0};
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::meter<double> {samples[i]} + addend).as_native();

            return results.back();
        };
    }

    TEST_CASE("Normalization to base SI units", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.5);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a * factor")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] * meter_per_yard;

            return results.back();
        };

        BENCHMARK("unit: yard::as_si()")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = distance::yard<double> {samples[i]}.as_si();

            return results.back();
        };
    }

    TEST_CASE("Conversion into the larger unit", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(100.0, 0.25);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a * factor")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] * meter_per_yard;

            return results.back();
        };

        BENCHMARK("unit: convert<meter>(yard)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = convert<distance::meter<>>(distance::yard<double> {samples[i]}).as_native();

            return results.back();
        };
    }

    TEST_CASE("Conversion into the smaller unit", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(100.0, 0.25);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a * reciprocal")
        {
            constexpr double yard_per_meter = 1.0 / meter_per_yard;
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] * yard_per_meter;

            return results.back();
        };

        BENCHMARK("unit: convert<yard>(meter)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = convert<distance::yard<>>(distance::meter<double> {samples[i]}).as_native();

            return results.back();
        };

        BENCHMARK("unit: from_si<yard>(double)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = from_si<distance::yard<>>(samples[i]).as_native();

            return results.back();
        };
    }

    TEST_CASE("Arithmetic on float values", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.5);
        const std::vector<float> float_samples(samples.begin(), samples.end());
        std::vector<float> results(sample_count);

        BENCHMARK("raw: float a + b")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = float_samples[i] + 2.0f;

            return results.back();
        };

        BENCHMARK("unit: meter<float> + meter<float>")
        {
            const distance::meter<float> addend {2.0f};
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::meter<float> {float_samples[i]} + addend).as_native();

            return results.back();
        };
    }

    TEST_CASE("Cross dimension arithmetic", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.5);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: a * a")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = samples[i] * samples[i];

            return results.back();
        };
        BENCHMARK("unit: meter * meter")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                const distance::meter<double> side {samples[i]};
                results[i] = (side * side).as_native();
            }

            return results.back();
        };
    }

    TEST_CASE("Compile-time integer powers", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(1.0, 0.01);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: repeated multiplication")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                const auto value = samples[i];
                results[i] = value * value * value * value * value * value * value * value * value * value;
            }

            return results.back();
        };

        BENCHMARK("unit: pow<10>(meter)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = pow<10>(distance::meter<double> {samples[i]}).as_native();

            return results.back();
        };
    }

    TEST_CASE("Formatting with width and nested width", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(100.0, 0.125);
        std::vector<std::size_t> lengths(sample_count);
        constexpr int field_width = 12;

        BENCHMARK("raw: format_to {:>12.2f}")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                std::array<char, 64u> buffer {};
                const auto it = std::format_to(buffer.begin(), "{:>12.2f}", samples[i]);
                lengths[i] = static_cast<std::size_t>(it - buffer.begin());
            }

            return lengths.back();
        };

        BENCHMARK("unit: format_to {:>12.2f} meter")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                std::array<char, 64u> buffer {};
                const auto it = std::format_to(buffer.begin(), "{:>12.2f}", distance::meter<double> {samples[i]});
                lengths[i] = static_cast<std::size_t>(it - buffer.begin());
            }

            return lengths.back();
        };

        BENCHMARK("raw: format_to {:>{}.2f}")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                std::array<char, 64u> buffer {};
                const auto it = std::format_to(buffer.begin(), "{:>{}.2f}", samples[i], field_width);
                lengths[i] = static_cast<std::size_t>(it - buffer.begin());
            }

            return lengths.back();
        };

        BENCHMARK("unit: format_to {:>{}.2f} meter")
        {
            for (std::size_t i {}; i != sample_count; ++i)
            {
                std::array<char, 64u> buffer {};
                const auto it = std::format_to(buffer.begin(), "{:>{}.2f}", distance::meter<double> {samples[i]}, field_width);
                lengths[i] = static_cast<std::size_t>(it - buffer.begin());
            }

            return lengths.back();
        };
    }

    TEST_CASE("Cross dimension arithmetic on scaled units", "[benchmark][!benchmark]")
    {
        // The case above pairs two units whose scale factor is one, so it never exercises the scaling a
        // product or a quotient has to do. Both operands here carry a factor, which is the shape a mixed
        // customary computation actually has.
        const auto samples = make_samples(1.0, 0.5);
        const auto divisors = make_samples(3.0, 0.25);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: (a * fa) * (b * fb)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (samples[i] * meter_per_yard) * (divisors[i] * meter_per_foot);

            return results.back();
        };

        BENCHMARK("unit: yard * foot")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::yard<double> {samples[i]} * distance::foot<double> {divisors[i]}).as_native();

            return results.back();
        };

        BENCHMARK("raw: (a * fa) / (b * fb)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (samples[i] * meter_per_yard) / (divisors[i] * meter_per_foot);

            return results.back();
        };

        BENCHMARK("unit: yard / foot")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::yard<double> {samples[i]} / distance::foot<double> {divisors[i]}).as_native();

            return results.back();
        };

        // Two values of the SAME unit: the two factors cancel to exactly one, so the quotient costs the
        // division alone and the ratio is a plain number.
        BENCHMARK("raw: (a * f) / (b * f)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (samples[i] * meter_per_yard) / (divisors[i] * meter_per_yard);

            return results.back();
        };

        BENCHMARK("unit: yard / yard")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (distance::yard<double> {samples[i]} / distance::yard<double> {divisors[i]}).as_native();

            return results.back();
        };
    }

    TEST_CASE("Conversion of a reading on a scale with an origin", "[benchmark][!benchmark]")
    {
        const auto samples = make_samples(-40.0, 0.05);
        std::vector<double> results(sample_count);

        BENCHMARK("raw: (a * 1.8) + 32")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = (samples[i] * 1.8) + 32.0;

            return results.back();
        };

        BENCHMARK("unit: convert<fahrenheit>(celsius)")
        {
            for (std::size_t i {}; i != sample_count; ++i)
                results[i] = convert<temperature::fahrenheit<>>(temperature::celsius<double> {samples[i]}).as_native();

            return results.back();
        };
    }
}
