/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/example.cpp
/// @brief Walks through the way a computation is meant to use this library: inputs built in the unit they
/// are quoted in, normalization to base SI scalars for the computation itself, and the return trip into
/// the units a report is written in.
#include <kmx/unit/all.hpp>
#include <format>
#include <iostream>

using namespace kmx::literals;

/// @brief A worked example of the round trip this library is built for: input, normalization, reporting.
namespace kmx::unit::example
{
    /// @brief Prints the inputs of a shot, each one built in the unit it is quoted in.
    static void report_inputs() noexcept(false)
    {
        // Every value states its unit at the point of construction, so no call site has to remember one.
        const distance::yard<double> range {300.0};
        const mass::grain<double> projectile_mass {168.0};
        const velocity::feet_per_second<double> muzzle_velocity {2700.0};
        const pressure::inch_of_mercury<double> station_pressure {29.92};
        const temperature::fahrenheit<double> ambient_temperature {59.0};

        std::cout << "Inputs\n"
                  << std::format("  range              {}\n", range) << std::format("  projectile mass    {}\n", projectile_mass)
                  << std::format("  muzzle velocity    {}\n", muzzle_velocity) << std::format("  station pressure   {}\n", station_pressure)
                  << std::format("  ambient temp       {}\n", ambient_temperature);
    }

    /// @brief Prints the same inputs normalized to the base SI scalars a computation kernel works with.
    static void report_normalization() noexcept(false)
    {
        const distance::yard<double> range {300.0};
        const mass::grain<double> projectile_mass {168.0};
        const velocity::feet_per_second<double> muzzle_velocity {2700.0};
        const temperature::fahrenheit<double> ambient_temperature {59.0};

        // as_si is the single normalization step; it is constexpr and never allocates.
        std::cout << "\nNormalized to base SI units\n"
                  << std::format("  range              {:.4f} m\n", range.as_si())
                  << std::format("  projectile mass    {:.6f} kg\n", projectile_mass.as_si())
                  << std::format("  muzzle velocity    {:.4f} m/s\n", muzzle_velocity.as_si())
                  << std::format("  ambient temp       {:.2f} K\n", ambient_temperature.as_si())
                  << std::format("  standard gravity   {:.5f} m/s2\n", acceleration::standard_gravity.as_si());
    }

    /// @brief Turns computation results back into the units a report is written in.
    static void report_results() noexcept(false)
    {
        const mass::grain<double> projectile_mass {168.0};
        const velocity::feet_per_second<double> muzzle_velocity {2700.0};

        // Cross-dimension arithmetic yields a value in base SI units, named by its dimension alone.
        const auto muzzle_energy = 0.5 * (projectile_mass * muzzle_velocity * muzzle_velocity);
        const auto time_of_flight = distance::yard<double> {100.0} / muzzle_velocity;

        // convert names the unit a result is read back in; from_si does the same for a bare SI scalar.
        std::cout << "\nResults\n"
                  << std::format("  muzzle energy      {:.1f}\n", convert<energy::joule<>>(muzzle_energy))
                  << std::format("  muzzle energy      {:.1f}\n", convert<energy::foot_pound<>>(muzzle_energy))
                  << std::format("  time of flight     {:.2f}\n", convert<duration::millisecond<>>(time_of_flight))
                  << std::format("  drop of 0.5 m      {:.4f}\n", from_si<distance::inch<>>(0.5));
    }

    /// @brief Prints one sight correction in each of the three angular units, which never alias one another.
    static void report_corrections() noexcept(false)
    {
        // A drop of half a meter at three hundred meters, as an angle.
        const auto correction = angle::atan2(distance::meter<double> {0.5}.as_si(), distance::meter<double> {300.0}.as_si());

        std::cout << "\nSight correction\n"
                  << std::format("  {:.4f}\n", convert<angle::moa<>>(correction))
                  << std::format("  {:.4f}\n", convert<angle::milliradian<>>(correction))
                  << std::format("  {:.4f}\n", convert<angle::mil<>>(correction));
    }

    /// @brief Prints the difference of two temperature readings, which is a value of its own kind.
    static void report_temperature_difference() noexcept(false)
    {
        const temperature::fahrenheit<double> morning {41.0};
        const temperature::fahrenheit<double> afternoon {77.0};

        // Subtracting two readings yields a difference, so it can never be mistaken for a reading.
        const auto warming = afternoon - morning;

        std::cout << "\nTemperature\n"
                  << std::format("  morning            {}\n", morning) << std::format("  afternoon          {}\n", afternoon)
                  << std::format("  warming            {}\n", warming)
                  << std::format("  warming            {}\n", convert<temperature::delta_celsius<>>(warming));
    }
}

/// @brief Runs every part of the example in turn.
/// @return Zero, the example cannot fail.
int main() noexcept(false)
{
    kmx::unit::example::report_inputs();
    kmx::unit::example::report_normalization();
    kmx::unit::example::report_results();
    kmx::unit::example::report_corrections();
    kmx::unit::example::report_temperature_difference();
    return 0;
}
