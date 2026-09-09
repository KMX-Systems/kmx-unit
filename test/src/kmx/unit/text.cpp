/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/text.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <format>
#include <sstream>

using namespace kmx::literals;

namespace kmx::unit::streaming
{
    TEST_CASE("Streaming Operator Tests", "[streaming]")
    {
        static constexpr std::string empty;
        int negative {-3};
        std::string_view text {"x"};

        SECTION("Named Units Stream Correctly")
        {
            std::stringstream ss;

            // --- Physical Units ---
            ss << 9.8_mps2;
            REQUIRE(ss.str() == "9.8 m/s2");
            ss.str(empty);
            ss << 1.57_rad;
            REQUIRE(ss.str() == "1.57 rad");
            ss.str(empty);
            ss << 30_arcsec;
            REQUIRE(ss.str() == "30 arcsec");
            ss.str(empty);
            ss << 15_arcmin;
            REQUIRE(ss.str() == "15 arcmin");
            ss.str(empty);
            ss << 2.0_rad_s;
            REQUIRE(ss.str() == "2 rad/s");
            ss.str(empty);
            ss << 150.5_m2;
            REQUIRE(ss.str() == "150.5 m2");
            ss.str(empty);
            ss << 10_m;
            REQUIRE(ss.str() == "10 m");
            ss.str(empty);
            ss << 3.2_ft;
            REQUIRE(ss.str() == "3.2 ft");
            ss.str(empty);
            ss << 15_s;
            REQUIRE(ss.str() == "15 s");
            ss.str(empty);
            ss << 2.5_h;
            REQUIRE(ss.str() == "2.5 h");
            ss.str(empty);
            ss << 1.5_A;
            REQUIRE(ss.str() == "1.5 A");
            ss.str(empty);
            ss << -1e-6_C;
            REQUIRE(ss.str() == "-1e-06 C");
            ss.str(empty);
            ss << 120_V;
            REQUIRE(ss.str() == "120 V");
            ss.str(empty);
            ss << 2.2e3_Ohm;
            REQUIRE(ss.str() == "2200 Ohm");
            ss.str(empty);
            ss << 100_F;
            REQUIRE(ss.str() == "100 F");
            ss.str(empty);
            ss << 0.5_H;
            REQUIRE(ss.str() == "0.5 H");
            ss.str(empty);
            ss << 500_J;
            REQUIRE(ss.str() == "500 J");
            ss.str(empty);
            ss << 75.5_kWh;
            REQUIRE(ss.str() == "75.5 kWh");
            ss.str(empty);
            ss << 980_N;
            REQUIRE(ss.str() == "980 N");
            ss.str(empty);
            ss << 1000_lx;
            REQUIRE(ss.str() == "1000 lx");
            ss.str(empty);
            ss << 12.5_cd;
            REQUIRE(ss.str() == "12.5 cd");
            ss.str(empty);
            ss << 5.5_kg;
            REQUIRE(ss.str() == "5.5 kg");
            ss.str(empty);
            ss << 500_g;
            REQUIRE(ss.str() == "500 g");
            ss.str(empty);
            ss << 1210_W;
            REQUIRE(ss.str() == "1210 W");
            ss.str(empty);
            ss << 1.21_GW;
            REQUIRE(ss.str() == "1.21 GW");
            ss.str(empty);
            ss << 101325_Pa;
            REQUIRE(ss.str() == "101325 Pa");
            ss.str(empty);
            ss << 273.15_K;
            REQUIRE(ss.str() == "273.15 K");
            ss.str(empty);
            ss << 20_degC;
            REQUIRE(ss.str() == "20 deg C");
            ss.str(empty);
            ss << 68_degF;
            REQUIRE(ss.str() == "68 deg F");
            ss.str(empty);
            ss << 25_mps;
            REQUIRE(ss.str() == "25 m/s");
            ss.str(empty);
            ss << 100_kmh;
            REQUIRE(ss.str() == "100 km/h");
            ss.str(empty);
            ss << 1.5_L;
            REQUIRE(ss.str() == "1.5 L");
            ss.str(empty);

            // --- Information Units ---
            ss << 1_b;
            REQUIRE(ss.str() == "1 b");
            ss.str(empty);
            ss << 1_B;
            REQUIRE(ss.str() == "1 B");
            ss.str(empty);
            ss << 256_MB;
            REQUIRE(ss.str() == "256 MB");
            ss.str(empty);
            ss << 4_GiB;
            REQUIRE(ss.str() == "4 GiB");
            ss.str(empty);
            ss << 2_TB;
            REQUIRE(ss.str() == "2 TB");
            ss.str(empty);
            ss << 1_PiB;
            REQUIRE(ss.str() == "1 PiB");
            ss.str(empty);

            // --- Data Rate Units ---
            ss << 56000_bps;
            REQUIRE(ss.str() == "56000 bps");
            ss.str(empty);
            ss << 100_Mbps;
            REQUIRE(ss.str() == "100 Mbps");
            ss.str(empty);
            ss << 10_GBps;
            REQUIRE(ss.str() == "10 GBps");
            ss.str(empty);
            ss << 50_MiBps;
            REQUIRE(ss.str() == "50 MiBps");
            ss.str(empty);
        }

        SECTION("Field Units Stream With Their Own Symbol")
        {
            std::stringstream ss;

            // --- Distance ---
            ss << 25.4_mm;
            REQUIRE(ss.str() == "25.4 mm");
            ss.str(empty);
            ss << 1.0_in;
            REQUIRE(ss.str() == "1 in");
            ss.str(empty);

            // --- Angle: the three small-angle units never print the same symbol ---
            ss << 1.5_moa;
            REQUIRE(ss.str() == "1.5 MOA");
            ss.str(empty);
            ss << 0.4_mrad;
            REQUIRE(ss.str() == "0.4 mrad");
            ss.str(empty);
            ss << 0.4_mil;
            REQUIRE(ss.str() == "0.4 mil");
            ss.str(empty);

            // --- Mass ---
            ss << 168_gr;
            REQUIRE(ss.str() == "168 gr");
            ss.str(empty);
            ss << 2.5_lb;
            REQUIRE(ss.str() == "2.5 lb");
            ss.str(empty);

            // --- Velocity and acceleration ---
            ss << 2700_fps;
            REQUIRE(ss.str() == "2700 ft/s");
            ss.str(empty);
            ss << 32.174_fps2;
            REQUIRE(ss.str() == "32.174 ft/s2");
            ss.str(empty);

            // --- Pressure ---
            ss << 1013.25_hPa;
            REQUIRE(ss.str() == "1013.25 hPa");
            ss.str(empty);
            ss << 1.0_bar;
            REQUIRE(ss.str() == "1 bar");
            ss.str(empty);
            ss << 14.7_psi;
            REQUIRE(ss.str() == "14.7 psi");
            ss.str(empty);
            ss << 29.92_inHg;
            REQUIRE(ss.str() == "29.92 inHg");
            ss.str(empty);
            ss << 760_mmHg;
            REQUIRE(ss.str() == "760 mmHg");
            ss.str(empty);

            // --- Energy ---
            ss << 2500_ftlbf;
            REQUIRE(ss.str() == "2500 ft-lbf");
            ss.str(empty);

            // --- Temperature: a difference never prints like a reading ---
            ss << 10_delta_K;
            REQUIRE(ss.str() == "10 delta K");
            ss.str(empty);
            ss << 10_delta_degC;
            REQUIRE(ss.str() == "10 delta deg C");
            ss.str(empty);
            ss << 18_delta_degF;
            REQUIRE(ss.str() == "18 delta deg F");
            ss.str(empty);
        }

        SECTION("Named Units Format Through std::format")
        {
            REQUIRE(std::format("{}", 10_m) == "10 m");
            REQUIRE(std::format("{}", 168.0_gr) == "168 gr");
            REQUIRE(std::format("{:.2f}", 29.921255_inHg) == "29.92 inHg");
            REQUIRE(std::format("{:.1f}", 1.25_moa) == "1.2 MOA");
            // A width covers the whole value, the number and the symbol together, so a column of them
            // lines up. It counts characters rather than bytes, so a middle dot takes one column.
            REQUIRE(std::format("{:>10}", 14.7_psi) == "  14.7 psi");
            REQUIRE(std::format("{:<10}", 14.7_psi) == "14.7 psi  ");
            REQUIRE(std::format("{:^10}", 14.7_psi) == " 14.7 psi ");
            REQUIRE(std::format("{:.>10}", 14.7_psi) == "..14.7 psi");
            REQUIRE(std::format("{:>10.1f}", 14.7_psi) == "  14.7 psi");
            REQUIRE(std::format("{:>8}", 6.0_Nm) == "   6 N·m");

            // Zero padding stays with the number, being written between the sign and the digits.
            REQUIRE(std::format("{:08.2f}", 14.7_psi) == "00014.70 psi");
            REQUIRE(std::format("{} at {}", 2700.0_fps, 59.0_degF) == "2700 ft/s at 59 deg F");
        }

        SECTION("The sign and the alternate form belong to the number")
        {
            // A width is taken by the value as a whole; every other flag of the specification is handed to
            // the formatter of the number untouched, and is written between the sign and the digits.
            REQUIRE(std::format("{:+.1f}", 10.0_m) == "+10.0 m");
            REQUIRE(std::format("{:+.1f}", -10.0_m) == "-10.0 m");
            REQUIRE(std::format("{: .1f}", 10.0_m) == " 10.0 m");
            REQUIRE(std::format("{:-.1f}", 10.0_m) == "10.0 m");
            REQUIRE(std::format("{:#.0f}", 10.0_m) == "10. m");
            REQUIRE(std::format("{:+#.0f}", 10.0_m) == "+10. m");
            REQUIRE(std::format("{:+.1f}", 20.0_degC) == "+20.0 deg C");

            // The same holds for a value that has no named unit.
            REQUIRE(std::format("{:+.2f}", 3.0_m * 4.0_m) == "+12.00 m^2");
        }

        SECTION("A width or a precision may be a formatting argument")
        {
            // A nested number is claimed while the specification is parsed, so that it is numbered among
            // the arguments of the whole format string, and read while the value is formatted. Building a
            // specification for the number that still held the `{}` left it referring to an argument the
            // number was never given, and every one of these threw at run time.
            REQUIRE(std::format("{:{}}", 10.0_m, 12) == "        10 m");
            REQUIRE(std::format("{:<{}}", 10.0_m, 12) == "10 m        ");
            REQUIRE(std::format("{:^{}}", 10.0_m, 12) == "    10 m    ");
            REQUIRE(std::format("{:.{}f}", 10.0_m, 3) == "10.000 m");
            REQUIRE(std::format("{:{}.{}f}", 10.0_m, 12, 2) == "     10.00 m");

            // A zero padding keeps the width with the number, whether it is written or passed.
            REQUIRE(std::format("{:0{}.2f}", 14.7_psi, 8) == "00014.70 psi");

            // Explicit indices work as well, and a value with no named unit is no different.
            REQUIRE(std::format("{0:{1}}", 10.0_m, 12) == "        10 m");
            REQUIRE(std::format("{:{}}", 3.0_m * 4.0_m, 14) == "        12 m^2");

            // An argument that cannot be a width says so rather than being read as one.
            auto metre = distance::meter<> {10.0};
            REQUIRE_THROWS_AS(std::vformat("{:{}}", std::make_format_args(metre, negative)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:{}}", std::make_format_args(metre, text)), std::format_error);
        }

        SECTION("A specification the value type would reject is rejected here too")
        {
            // The rules for a presentation type belong to the value type, and the formatter of the number
            // is handed the specification it will be given, so `{:Z}` on a metre fails exactly where
            // `{:Z}` on a double fails. Through std::format that is compile time, which no test can catch;
            // std::vformat takes the same path at run time and does.
            auto metre = distance::meter<> {10.0};
            REQUIRE_THROWS_AS(std::vformat("{:Z}", std::make_format_args(metre)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:ff}", std::make_format_args(metre)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:.2d}", std::make_format_args(metre)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:s}", std::make_format_args(metre)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:.}", std::make_format_args(metre)), std::format_error);
            REQUIRE_THROWS_AS(std::vformat("{:{ }", std::make_format_args(metre)), std::format_error);

            // The locale flag belongs to the number as every other flag does.
            REQUIRE(std::format("{:L}", 10.0_m) == "10 m");
            REQUIRE(std::format("{:.2Lf}", 10.0_m) == "10.00 m");
            REQUIRE(std::format("{:L}", 3.0_m * 4.0_m) == "12 m^2");

            // The ones it accepts still work.
            REQUIRE(std::vformat("{:.3e}", std::make_format_args(metre)) == "1.000e+01 m");
        }

        SECTION("Generic SI Units Stream with Detailed Dimensions")
        {
            std::stringstream ss;

            // Force (M·L·T⁻²)
            const auto force = 10.0_kg * 9.8_mps2;
            ss << force;
            REQUIRE(ss.str() == "98 kg·m·s^-2");
            ss.str(empty);

            // Pressure (M·L⁻¹·T⁻²)
            const auto pressure = 100.0_N / 10.0_m2;
            ss << pressure;
            REQUIRE(ss.str() == "10 kg·m^-1·s^-2");
            ss.str(empty);

            // Dimensionless ratio (should have no units)
            const auto ratio = 1.0_mi / 5280.0_ft;
            ss << ratio;
            REQUIRE(ss.str() == "1");
            ss.str(empty);

            // Velocity (L·T⁻¹)
            const auto velocity = 100_m / 10_s;
            ss << velocity;
            REQUIRE(ss.str() == "10 m·s^-1");
            ss.str(empty);
        }
    }

    // A symbol is written straight to the output rather than through a fixed buffer, so a unit defined by
    // a caller formats whatever its symbol length, and a UTF-8 symbol spending several bytes per character
    // is no different. A bounded buffer overflowed here for any symbol past its capacity.
    namespace long_symbol
    {
        KMX_UNIT_DEFINE(readings_per_square_kilometer, dimension::area_t, scale::micro, "readings per square kilometer")
        KMX_UNIT_DEFINE(wide_symbol, dimension::area_t, scale::one,
                        "\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5\u00b7\u00b5"
                        "\u00b7\u00b5\u00b7\u00b5")
    }

    TEST_CASE("A symbol of any length formats and streams", "[streaming]")
    {
        SECTION("A symbol longer than any of the library's own")
        {
            const long_symbol::readings_per_square_kilometer<double> value {2.5};
            REQUIRE(std::format("{}", value) == "2.5 readings per square kilometer");
            REQUIRE(std::format("{:.2f}", value) == "2.50 readings per square kilometer");

            std::stringstream ss;
            ss << value;
            REQUIRE(ss.str() == "2.5 readings per square kilometer");
        }

        SECTION("Padding a long symbol counts characters, not bytes")
        {
            // Twelve micro signs and eleven middle dots: 23 characters written in 46 bytes. The number and
            // its separating space bring the value to 25 characters, so a width of 30 pads it by five.
            const long_symbol::wide_symbol<double> value {1.0};
            const auto text = std::format("{:30}", value);
            REQUIRE(kmx::unit::detail::text_width(text) == 30u);
            REQUIRE(text.size() == (30u + 23u)); // every character of the symbol spends two bytes
            REQUIRE(text.starts_with("     1 "));
            REQUIRE(text.ends_with(long_symbol::wide_symbol<double>::text));
        }
    }
}
