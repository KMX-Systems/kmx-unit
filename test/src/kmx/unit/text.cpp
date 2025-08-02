/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file src/kmx/unit/text.cpp
#include "kmx/unit/testing.hpp"
#include <kmx/unit/all.hpp>
#include <sstream>

namespace kmx::unit::streaming
{
    TEST_CASE("Streaming Operator Tests", "[streaming]")
    {
        static constexpr std::string empty;

        SECTION("Named Units Stream Correctly")
        {
            std::stringstream ss;
            const std::string empty; // Helper for clearing the stringstream

            // --- Physical Units ---
            ss << _mps2(9.8);
            auto a = ss.str();
            REQUIRE(ss.str() == "9.8 mps2");
            ss.str(empty);
            ss << _rad(1.57);
            REQUIRE(ss.str() == "1.57 rad");
            ss.str(empty);
            ss << _arcsec(30);
            REQUIRE(ss.str() == "30 arcsec");
            ss.str(empty);
            ss << _arcmin(15);
            REQUIRE(ss.str() == "15 arcmin");
            ss.str(empty);
            ss << _rad_s(2.0);
            REQUIRE(ss.str() == "2 rad/s");
            ss.str(empty);
            ss << _m2(150.5);
            REQUIRE(ss.str() == "150.5 m2");
            ss.str(empty);
            ss << _m(10);
            REQUIRE(ss.str() == "10 m");
            ss.str(empty);
            ss << _ft(3.2);
            REQUIRE(ss.str() == "3.2 ft");
            ss.str(empty);
            ss << _s(15);
            REQUIRE(ss.str() == "15 s");
            ss.str(empty);
            ss << _h(2.5);
            REQUIRE(ss.str() == "2.5 h");
            ss.str(empty);
            ss << _A(1.5);
            REQUIRE(ss.str() == "1.5 A");
            ss.str(empty);
            ss << _C(-1e-6);
            REQUIRE(ss.str() == "-1e-06 C");
            ss.str(empty);
            ss << _V(120);
            REQUIRE(ss.str() == "120 V");
            ss.str(empty);
            ss << _Ohm(2.2e3);
            REQUIRE(ss.str() == "2200 Ohm");
            ss.str(empty);
            ss << _F(100);
            REQUIRE(ss.str() == "100 F");
            ss.str(empty);
            ss << _H(0.5);
            REQUIRE(ss.str() == "0.5 H");
            ss.str(empty);
            ss << _J(500);
            REQUIRE(ss.str() == "500 J");
            ss.str(empty);
            ss << _kWh(75.5);
            REQUIRE(ss.str() == "75.5 kWh");
            ss.str(empty);
            ss << _N(980);
            REQUIRE(ss.str() == "980 N");
            ss.str(empty);
            ss << _lx(1000);
            REQUIRE(ss.str() == "1000 lx");
            ss.str(empty);
            ss << _cd(12.5);
            REQUIRE(ss.str() == "12.5 cd");
            ss.str(empty);
            ss << _kg(5.5);
            REQUIRE(ss.str() == "5.5 kg");
            ss.str(empty);
            ss << _g(500);
            REQUIRE(ss.str() == "500 g");
            ss.str(empty);
            ss << _W(1210);
            REQUIRE(ss.str() == "1210 W");
            ss.str(empty);
            ss << _GW(1.21);
            REQUIRE(ss.str() == "1.21 GW");
            ss.str(empty);
            ss << _Pa(101325);
            REQUIRE(ss.str() == "101325 Pa");
            ss.str(empty);
            ss << _K(273.15);
            REQUIRE(ss.str() == "273.15 K");
            ss.str(empty);
            ss << _degC(20);
            REQUIRE(ss.str() == "20 deg C");
            ss.str(empty);
            ss << _degF(68);
            REQUIRE(ss.str() == "68 deg F");
            ss.str(empty);
            ss << _mps(25);
            REQUIRE(ss.str() == "25 m/s");
            ss.str(empty);
            ss << _kmh(100);
            REQUIRE(ss.str() == "100 km/h");
            ss.str(empty);
            ss << _L(1.5);
            REQUIRE(ss.str() == "1.5 L");
            ss.str(empty);

            // --- Information Units ---
            ss << _b(1);
            REQUIRE(ss.str() == "1 b");
            ss.str(empty);
            ss << _B(1);
            REQUIRE(ss.str() == "1 B");
            ss.str(empty);
            ss << _MB(256);
            REQUIRE(ss.str() == "256 MB");
            ss.str(empty);
            ss << _GiB(4);
            REQUIRE(ss.str() == "4 GiB");
            ss.str(empty);
            ss << _TB(2);
            REQUIRE(ss.str() == "2 TB");
            ss.str(empty);
            ss << _PiB(1);
            REQUIRE(ss.str() == "1 PiB");
            ss.str(empty);

            // --- Data Rate Units ---
            ss << _bps(56000);
            REQUIRE(ss.str() == "56000 bps");
            ss.str(empty);
            ss << _Mbps(100);
            REQUIRE(ss.str() == "100 Mbps");
            ss.str(empty);
            ss << _GBps(10);
            REQUIRE(ss.str() == "10 GBps");
            ss.str(empty);
            ss << _MiBps(50);
            REQUIRE(ss.str() == "50 MiBps");
            ss.str(empty);
        }

        SECTION("Generic SI Units Stream with Detailed Dimensions")
        {
            std::stringstream ss;
            const std::string empty;

            // Force (M·L·T⁻²)
            auto force = _kg(10.0) * _mps2(9.8);
            ss << force;
            REQUIRE(ss.str() == "98 kg·m·s^-2");
            ss.str(empty);

            // Pressure (M·L⁻¹·T⁻²)
            auto pressure = _N(100.0) / _m2(10.0);
            ss << pressure;
            REQUIRE(ss.str() == "10 kg·m^-1·s^-2");
            ss.str(empty);

            // Dimensionless ratio (should have no units)
            auto ratio = _mi(1.0) / _ft(5280.0);
            ss << ratio;
            REQUIRE(ss.str() == "1");
            ss.str(empty);

            // Velocity (L·T⁻¹)
            auto velocity = _m(100) / _s(10);
            ss << velocity;
            REQUIRE(ss.str() == "10 m·s^-1");
            ss.str(empty);
        }
    }
}
