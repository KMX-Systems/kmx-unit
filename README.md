# kmx-unit

A header-only C++23 library of dimensional quantities: a value carries its unit in its type, conversions are
resolved at compile time, and a value costs exactly what its underlying arithmetic type costs.

## Design

A unit is a class template over the arithmetic type holding the value. It stores the value in its **native**
representation and knows two compile-time constants: the size of one native unit in base SI units
(`magnitude_t`) and the SI value matching a native zero (`offset_t`). Both are **exact**: a magnitude is a
`std::ratio` times a power of pi, an offset is a `std::ratio`, and each is rounded to a `double` once, at
the point a value is actually scaled. See [Exact sizes](#exact-sizes).

```cpp
#include <kmx/unit/all.hpp>

using namespace kmx::unit;                 // convert, from_si, ...
using namespace kmx::literals;             // the unit suffixes, `_m`, `_K`, `_MB`, ...

const auto muzzle_velocity = 2700.0_fps;             // a literal carries its unit in its suffix
const distance::yard<double> range {300.0};          // anything else names its type
const distance::meter<float> reported {sample};      // as does a value type other than double

const double range_si = range.as_si();                       // 274.32, the form a kernel expects
const auto reported = convert<distance::meter<>>(range);      // 274.32 m, a typed value
const auto from_kernel = from_si<distance::yard<>>(274.32);   // back into the reporting unit
```

- Every unit is written as a literal suffix: `2700.0_fps`, `10_kV`, `1.5_MB`. The suffixes live in
  `kmx::literals`, so a header alone does not bring all 134 of them into scope. Two consequences are worth
  knowing. A literal operator receives only `long double` or `unsigned long long`, so a suffix always yields
  a `double`-valued unit, and a value built from anything but a literal — a variable, an expression, another
  value type — names its type instead: `distance::meter<float> {measured}`. And a ud-suffix lexes greedily,
  so a member call on a literal needs parentheses: `(1.0_K).as_si()`, not `1.0_K.as_si()`.
- `as_si()` normalizes to base SI units; `from_si<Unit>()` is its explicit inverse.
- `convert<Unit>()` converts between units of one dimension; `fp_convert<Unit>()` rounds, for integral targets.
  `checked_fp_convert<Unit>()` and `checked_from_si<Unit>()` provide the same rounding while returning
  `std::nullopt` for NaN, infinity or a result outside the target integral type's range.
- Unchecked conversions (`convert`, `from_si`, `to_chrono`, `from_chrono`, `fp_convert`) preserve the same
  narrowing and truncation behavior as ordinary C++ casts. Checked conversions
  (`checked_fp_convert`, `checked_from_si`, `checked_to_chrono`, `checked_from_chrono`) preserve the same
  scale and rounding policy while returning `std::nullopt` for non-finite and out-of-range results.
- Arithmetic between same-dimension values keeps the unit of the left operand; multiplication and division
  across dimensions yield an `si_unit` of the resulting dimension.
- Comparison works across units of one dimension. Two values meet at a size both can reach, and which size
  that is follows from the pair of units alone and never from which side of the operator either sits on,
  which is what keeps `a == b` and `b == a` the same computation. Where one unit is a whole multiple of the
  other, that size is the finer of the two units and both values reach it exactly, so `1.0_lb == 7000.0_gr`
  is true. Two readings on scales with origins meet in one of their own scales, so `59.0_degF == 15.0_degC`
  is true as well. Every other pair meets in base SI units, each operand through its own rounded factor, and
  is an ordinary floating point comparison.
- `near(lhs, rhs, tolerance)` is the comparison to reach for whenever the two units do not divide into one
  another. The tolerance is a typed value, so it cannot be read in the wrong unit, and it may not be an
  absolute reading — a temperature is compared to within a `delta_celsius`, never a `celsius`. It accepts
  every pair `==` accepts, so a tolerance of zero is never stricter than exact equality.

  ```cpp
  near(1.0_lb, 0.45359237_kg, 1.0_ug)               // true, where a pound and a kilogram do not divide
  near(100.0_degC, 212.0_degF, 0.001_delta_K)       // true, across two scales with an origin
  ```

## Unit families

| Family | Header | Units |
| --- | --- | --- |
| length | `distance.hpp` | millimeter, meter, kilometer, inch, foot, yard, mile, nautical mile |
| plane angle | `angle.hpp` | radian, milliradian, mil, degree, arcminute, arcsecond, MOA |
| speed | `velocity.hpp` | meter per second, foot per second, kilometer per hour, mile per hour, knot |
| acceleration | `acceleration.hpp` | meter per second squared, foot per second squared, standard gravity |
| mass | `mass.hpp` | kilogram, gram, milligram, microgram, grain, pound |
| force | `force.hpp` | newton, kilonewton, kilogram-force |
| pressure | `pressure.hpp` | pascal, hectopascal, kilopascal, megapascal, bar, psi, inHg, mmHg |
| temperature | `temperature.hpp` | kelvin, degree Celsius, degree Fahrenheit, and the difference of each |
| time | `duration.hpp` | second, millisecond, microsecond, nanosecond, minute, hour, day |
| energy | `energy.hpp` | joule, kilojoule, foot-pound, kilowatt-hour, megawatt-hour, gigawatt-hour |
| frequency | `frequency.hpp` | hertz, kilohertz, megahertz, gigahertz, rpm |
| torque | `torque.hpp` | newton metre, kilonewton metre, pound-force foot, pound-force inch |
| amount of substance | `amount.hpp` | mole, millimole, micromole, kilomole |
| solid angle | `solid_angle.hpp` | steradian, millisteradian, square degree |
| others | | area, volume, power, angular velocity, illuminance, luminous intensity, electromagnetism (including weber, tesla and gauss), information, data rate |

A unit definition is one line. `KMX_UNIT_DEFINE(name, dimension, magnitude, symbol)` generates the type, its
constructor, its rebinding alias and its symbol, so a new family is a handful of lines and a unit can never
be defined with a mistyped name:

```cpp
KMX_UNIT_DEFINE(kilometer, dimension::length_t, scale::kilo, "km")
```

When two domains share a dimension but must not inter-convert, a unit can declare its own quantity-kind
tag with `using quantity_kind_t = my_kind_tag;` inside `KMX_UNIT_DEFINE_EX(...)`. Units sharing the same
custom tag interoperate; units with different tags do not.

## Exact sizes

A scale factor written as a `double` is already rounded when the unit is defined, so a conversion between
two such units divides one rounded number by another and lands beside the exact answer. Every size in this
library is therefore a `std::ratio` times a power of pi, and every origin a `std::ratio`. They are combined
exactly and rounded once, at the point a value is actually scaled.

```cpp
using meter_per_inch = scale::ratio<127, 5000>;                            // exactly 0.0254
using meter_per_foot = scale::multiply_t<meter_per_inch, scale::ratio<12>>;
using radian_per_degree = scale::pi_multiple<1, 180>;                      // exactly pi/180
```

`scale` also names the decimal prefixes (`nano` … `peta`) and the binary ones (`kibi` … `pebi`), which is
how most units of the library spell their size. What this buys:

| | with `double` factors | with exact sizes |
| --- | --- | --- |
| `convert<grain<>>(1.0_lb)` | 6999.9999999999991 | exactly 7000 |
| `convert<arcminute<>>(1.0_deg)` | 59.999999999999993 | exactly 60 |
| `convert<celsius<>>(32.0_degF)` | 3.55e-15 | exactly 0 |
| `1.0_lb == 7000.0_gr` | false | true |
| `59.0_degF == 15.0_degC` | true by luck | true |

The exactness is in the constants, not in the arithmetic: a conversion is still one multiplication by one
compile-time `double`, and the [performance](#performance) table is unchanged. What changed is which
`double` that is — the correctly rounded value of the whole reduced ratio, rather than an accumulation of
one rounding per factor the size was built from.

A size that is not rational is still exact, because the exponent of pi is carried beside the ratio: a degree
is `pi/180` and a square degree `pi²/32400`, so a degree and an arcminute divide into the whole number 60
even though neither is a rational number of radians.

Two units still have to actually divide into one another for their comparison to be exact. A pound and a
grain do, and compare exactly; a pound and a kilogram do not, meet in kilograms, and are compared as
ordinary floating point — that is what `near` is for.

Two exact sizes also do not always combine into a third. A foot-pound is `3389544870828501/2500000000000000`
joules, and dividing that by a kilowatt hour asks for a denominator of nine thousand million million
million, which no `std::intmax_t` holds. Such a pair falls back to the rounded size rather than refusing to
compile, and only that pair loses the exactness — its comparison steps back to base SI units. The
`test/src/kmx/unit/scale.cpp` test converts, compares and orders **every** pair of units in every family, so
a size that stops fitting is caught by the suite rather than by a consumer.

## Distinctions the type system enforces

### Mass is not weight

Every unit of `kmx::unit::mass` measures mass, the pound and the grain included. A projectile "weight"
quoted in grains is a mass and belongs there. A weight is a force and belongs to `kmx::unit::force`, where
`kilogram_force` is the force one kilogram of mass exerts under standard gravity. The `foot_pound` energy
unit is named after the pound-**force**, not after the mass unit of the same name.

### Pressure is absolute, never gauge

Every unit of `kmx::unit::pressure` carries an absolute pressure. A gauge reading is the difference between
an absolute pressure and the ambient one, so a caller holding a gauge reading has to add the ambient
pressure before handing the value over. No name or helper of the library implies a gauge reading.

### A temperature reading is not a temperature difference

Readings (`kelvin`, `celsius`, `fahrenheit`) are positions on a scale with an origin. Differences
(`delta_kelvin`, `delta_celsius`, `delta_fahrenheit`) are steps along one. The library keeps them apart:

```cpp
const auto step  = 25.0_degC - 10.0_degC;   // delta_celsius, never a reading
const auto later = 15.0_degC + step;          // celsius, a reading again
20.0_degC + 10.0_degC;                      // rejected: two readings do not add
20.0_degC * 2.0;                              // rejected: a reading does not scale
20.0_degC == 20.0_delta_degC;               // rejected: the two kinds never compare
convert<celsius<>>(1.0_delta_degC);           // rejected: the two kinds never convert
```

Normalization stays correct for both kinds: a reading normalizes through its origin offset, a difference
normalizes as a step, so `(15.0_degC).as_si()` is `288.15` while `(15.0_delta_degC).as_si()` is `15.0`.

### MOA, milliradian and mil are three units, not three names

| Unit | Type | Size | Text |
| --- | --- | --- | --- |
| minute of angle | `angle::moa` | 1/60 degree | `MOA` |
| milliradian | `angle::milliradian` | 0.001 radian | `mrad` |
| NATO mil | `angle::mil` | 1/6400 turn | `mil` |

A milliradian is about 1.9 percent larger than a NATO mil; they are separate types and neither is an alias
of the other. `moa` has the size of an `arcminute` but is a separate type as well, so a sight correction
never mixes with a plain angular measurement and always prints as `MOA`. `moa` is the true minute of angle,
not the "shooter's MOA" of one inch per hundred yards, which is an approximation rather than a unit.

Because an angle is dimensionless, the dimension system alone cannot tell an angle from a plain ratio. Every
angle unit therefore carries the `angular` tag and the `angle::angle_unit` concept selects them:

```cpp
const double x = angle::radians(1.5_moa);  // normalized kernel input
const double s = angle::sin(30.0_deg);     // sin, cos, tan take any angle unit
const auto a = angle::atan2(0.5, 300.0);     // the inverses return a radian, not a bare number
```

## Mathematics

`math.hpp` provides the functions that carry a unit through instead of dropping it. The dimension exponents
are `std::ratio` values, so a root is exact rather than approximate: the square root of a volume is a length
to the power 3/2.

```cpp
abs(-3.5_m);                      // 3.5 m
min(2.0_m, 1.0_yd);             // 0.9144 m, in the unit of the left operand
clamp(5.0_m, 1.0_m, 3.0_yd);  // 2.7432 m
hypot(3.0_m, 4.0_m);            // 5 m
sqrt(3.0_m * 12.0_m);           // 6 m, a length again
pow<3>(2.0_m);                    // 8 m^3
cbrt(pow<3>(2.0_m));              // 2 m
```

Values also accumulate in place. The right operand of `+=` and `-=` may never be an absolute reading, so a
temperature can take a step but never another temperature:

```cpp
auto total = 10.0_m;
total += 100.0_yd;                // 101.44 m, still a meter<double>
auto reading = 20.0_degC;
reading += 5.0_delta_degC;        // 25 degC
reading += 5.0_degC;              // rejected
```

## Interoperation with std::chrono

```cpp
const auto elapsed = from_chrono<duration::millisecond<>>(1500ms);  // 1500 ms
const auto timeout = to_chrono<std::chrono::milliseconds>(1.5_s); // 1500ms
const auto checked = checked_to_chrono<std::chrono::milliseconds>(1.5_s); // optional, checked range
const auto checked_input = checked_from_chrono<duration::millisecond<int>>(1500ms);
```

The tick period of a `std::chrono::duration` is a `std::ratio` of seconds, so both directions resolve their
factor at compile time. Only a value of the time dimension converts. The checked chrono functions preserve
the existing narrowing behavior while returning `std::nullopt` for non-finite or out-of-range results.

### Checked conversion policy

Use unchecked conversions when representability is guaranteed by construction or when narrow/truncate
behavior is the intended policy. Use checked conversions at boundaries where conversion failure should be
explicitly handled.

| Conversion family | Unchecked API | Checked API | Narrowing policy |
| --- | --- | --- | --- |
| unit -> unit (integral target) | `fp_convert<Target>(value)` | `checked_fp_convert<Target>(value)` | rounded to nearest target integral value |
| SI scalar -> unit (integral target) | `from_si<Target>(si)` | `checked_from_si<Target>(si)` | rounded to nearest target integral value |
| chrono -> unit (integral target) | `from_chrono<Target>(duration)` | `checked_from_chrono<Target>(duration)` | truncates toward zero, matching chrono cast style |
| unit -> chrono (integral rep) | `to_chrono<Target>(value)` | `checked_to_chrono<Target>(value)` | truncates toward zero, matching chrono cast style |

## Formatting

Every named unit streams and formats as its native value followed by its symbol. The format specification
is the one of the underlying value type.

```cpp
std::cout << 29.92_inHg;                 // 29.92 inHg
std::format("{:.2f}", 2718.96_ftlbf);    // "2718.96 ft-lbf"
std::cout << (10.0_kg * 9.8_mps2);     // 98 kg·m·s^-2, for a value with no named unit
std::format("{:.2f}", 3.0_m * 4.0_m);  // "12.00 m^2", the same for a computed value
```

The symbol of a dimension is assembled once at compile time, so a value with no named unit costs no more to
print than a string, and formats as well as it streams.

A width applies to the value as a whole; every other part of the specification is handed to the formatter of
the number, so it behaves exactly as it would on the number alone. That includes a width or a precision
passed as an argument, and it includes being rejected: a specification the value type would not accept is
not accepted here either, at compile time.

```cpp
std::format("{:>12}", 9.8_mps2);           // "    9.8 m/s2", the whole value padded
std::format("{:{}.{}f}", 10.0_m, 12, 2);   // "     10.00 m", width and precision from arguments
std::format("{:08.2f}", 14.7_psi);         // "00014.70 psi", a zero padding stays with the number
std::format("{:Z}", 10.0_m);               // rejected, exactly as `{:Z}` on a double is
```

## Guarantees

- No allocation. A unit value holds its native value and nothing else: `sizeof(meter<double>) == sizeof(double)`,
  and the type is trivially copyable and standard layout. A size and an origin are types, so neither costs a
  byte of storage.
- Construction, normalization, conversion, arithmetic and comparison are `constexpr` and `noexcept`.
- Values of unrelated dimensions never compare, add or convert; the failure is a compile error.
- A value cannot be written through without naming a unit: the stored value is not a public member.
- The library is `std::`-qualified throughout and needs no dependency beyond the standard library.

The `test/src/kmx/unit/properties.cpp` test asserts these properties at compile time.

## Performance

A unit value costs what its underlying arithmetic type costs. Every operation compiles to the same
instruction a caller would write by hand:

| Operation | Emitted code |
| --- | --- |
| `meter + meter` | `addsd` |
| `meter + yard` | `mulsd`, `addsd` |
| `yard::as_si()` | `mulsd` |
| `convert<yard>(meter)` | `mulsd` |
| `from_si<yard>(v)` | `mulsd` |
| `convert<fahrenheit>(celsius)` | `mulsd`, `addsd` |
| `meter * meter` | `mulsd` |
| `meter<float> + meter<float>` | `addss` |
| `pound == grain` | `mulsd`, `ucomisd` |

The scale factor and the origin offset of a conversion are reduced to compile-time constants, so a
conversion is a multiplication and never a division, and a size of one or an origin of zero costs nothing at
all. Whether it does is read off the exact size rather than off the number that size rounds to. A loop over
a conversion vectorizes at the full width of the value type.

`test/src/kmx/unit/benchmark.cpp` measures each of these against the raw scalar loop it should match. The
cases carry the `[!benchmark]` tag, so a plain run of the suite skips them. The benchmark set also covers
width-based formatting (`{:>12.2f}` and `{:>{}.2f}`) to track the unit formatter path:

```bash
profile="$(bash tools/qbs/select_profile.sh)"
qbs run -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release --products unit-test -- "[benchmark]"
```

A guard script checks that unit-formatting overhead against the raw scalar baseline stays within a bounded
ratio in the same run. By default it emits a warning above 2.00x and fails above 2.50x; both thresholds can
be overridden through environment variables. Pass either the executable itself or a directory to search for
it:

```bash
CXX=g++ bash tools/bench/guard_formatting.sh "$(find output/qbs -type f -name unit-test | head -n 1)"
```

The measured ratios are also written to `output/benchmark/formatting-guard.csv`, which
`KMX_UNIT_FORMAT_BENCH_RESULT_PATH` overrides.

## Compile-time include probes

`all.hpp` is the convenience umbrella include; selective includes keep translation units lighter when they
need only a subset of the library. The repository includes probe translation units and a measurement script
under `tools/compile_time`:

```bash
CXX=g++-14 bash tools/compile_time/measure.sh 5
```

The CSV summary is written to `output/compile_time/report.csv`; a second argument names a different file:

```bash
CXX=g++-14 bash tools/compile_time/measure.sh 5 output/compile_time/report.csv
```

The script reports average/min/max front-end wall times for:

- `tools/compile_time/probe_all.cpp` (`#include <kmx/unit/all.hpp>`)
- `tools/compile_time/probe_selective.cpp` (feature-equivalent selective includes)

When run with Clang, it also emits `-ftime-trace` JSON files into `output/compile_time/traces/`.

## Build

The library is header-only; adding `source/inc` to the include path is enough.

### Output layout

Nothing a build produces is written beside the sources. Every build directory, binary, report and artifact
goes under `output/`, which is the one directory a checkout ignores and the one directory to delete for a
clean slate:

```text
output/
  qbs/                     QBS build directories, one per configuration
  cmake/<preset>/          CMake build directories, one per preset
  install/                 the local install prefix
  coverage/                lcov data and the HTML report
  compile_time/            compile-time probe report, CSV and Clang traces
  benchmark/               formatter benchmark guard results
  staging/, consumer/      the install prefix and scratch project of the CI packaging check
```

Neither build system picks that location on its own, so both are told where to write: QBS through
`--build-directory` (its default is the current directory, which is what would leave a stray `release/`
behind), CMake through the presets below, which configure into `output/cmake/<preset>`. Being told is not
enough on its own, so both projects also refuse a build directory that lands anywhere inside the source tree
but under `output/` - an in-source CMake configuration, a `build/` beside the sources, the QBS default -
and name the option to pass instead. A build directory outside the tree writes nothing here and is
accepted. The `KMX_UNIT_OUTPUT_DIR` CMake cache variable and the environment variable of the same name,
which the scripts under `tools/` read, move the root elsewhere.

### Primary workflow: QBS

Development, warning-clean builds, tests, benchmarks and CI run through QBS:

```bash
profile="$(bash tools/qbs/select_profile.sh)"
qbs resolve -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release
qbs build -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release
qbs run -f kmx-unit.qbs --build-directory output/qbs profile:"${profile}" config:release --products unit-test
```

The QBS products enable all compiler warnings and treat warnings as errors. The profile selector prefers
known GCC/Clang profiles and falls back to the first available profile. After building into `output/qbs`,
run the formatter guard with:

```bash
CXX=g++ bash tools/bench/guard_formatting.sh output/qbs
```

### Compatibility and packaging: CMake

The CMake build remains for packaging, installation, `find_package` consumption and compatibility checks.
The presets - `release`, `debug`, `sanitizers` and `coverage` - each configure into their own directory
under `output/cmake`:

```bash
cmake --preset release
cmake --build --preset release
cmake --build output/cmake/release --target check
```

`ctest --preset release` runs the same suite directly, without the runtime path that `check` resolves.

Without the presets, the build directory has to be named explicitly, and it belongs under `output`:

```bash
cmake -S . -B output/cmake/release -DCMAKE_BUILD_TYPE=Release
cmake --build output/cmake/release
cmake --build output/cmake/release --target check
```

`check` runs `ctest` through `tools/cmake/run_with_compiler_runtime.sh`, which resolves and prepends the
selected compiler's C++ runtime directory to avoid loader mismatches (for example `GLIBCXX_*` errors).

```cmake
find_package(kmx-unit REQUIRED)
target_link_libraries(my_target PRIVATE kmx::unit)
```

Catch2 is used when it is already installed and fetched when it is not, so a fresh checkout builds its
tests without preparation. CMake's `-DKMX_UNIT_STRICT_WARNINGS=ON` and `-DKMX_UNIT_SANITIZERS=ON` options
remain available for package and consumer compatibility checks, not as the primary development path.

## Coverage via CMake

Coverage instrumentation is currently provided through the CMake compatibility build. `-DKMX_UNIT_COVERAGE=ON`
instruments the suite for gcov and adds a `coverage` target that runs it, collects the counters with lcov,
keeps only the library headers, writes an HTML report and fails when the line coverage falls below
`KMX_UNIT_COVERAGE_MINIMUM` (97 by default). It needs lcov 2.0 or newer.

```bash
cmake --preset coverage
cmake --build --preset coverage
```

```text
-- kmx-unit line coverage: 100.0% (minimum 97%)
-- kmx-unit coverage report: /path/to/kmx-unit/output/coverage/html/index.html
```

The report is written to `output/coverage`, not into the build directory, so it survives reconfiguring and
is found in the same place whichever preset produced it.

The library is header-only, so what is measured is the code the suite instantiates and runs; a template
never instantiated emits nothing for gcov to count and cannot appear in the report. Inlining is turned off
under coverage so that a function defined in a header is counted where it is written.

## Version

`kmx/unit/version.hpp` names the version for the preprocessor and for the code alike. The CMake packaging
build reads the three numbers back out of that header rather than repeating them, so the installed package
version and the version the headers report cannot disagree.

```cpp
#include <kmx/unit/version.hpp>

#if KMX_UNIT_VERSION >= KMX_UNIT_VERSION_NUMBER(1, 2, 0)   // guarding on a release
    ...
#endif

std::cout << kmx::unit::version_text;   // "1.0.0"
```

| Name | Meaning |
| --- | --- |
| `KMX_UNIT_VERSION_MAJOR`, `_MINOR`, `_PATCH` | the three components, as preprocessor integers |
| `KMX_UNIT_VERSION_NUMBER(major, minor, patch)` | those three combined into one ordered integer |
| `KMX_UNIT_VERSION` | this release as that integer |
| `KMX_UNIT_VERSION_TEXT` | this release as a string literal |
| `kmx::unit::version`, `version_major`, `version_minor`, `version_patch`, `version_text` | the same values as constants |

- `kmx-unit.qbs` - the top-level QBS project used for development and CI.
- `source/library.qbs` - the `unit` product, exporting `source/inc`.
- `test/unit-test.qbs` - the Catch2 test suite.
- `example/example.qbs` - a worked example of the input, normalization and reporting round trip.
- `CMakeLists.txt` - the compatibility and packaging target, its install rules and its package config.
- `CMakePresets.json` - the CMake configurations, each building into `output/cmake/<preset>`.

For release preparation on a dirty worktree, see `source/repository-cleanup-plan.md`.

## License

Copyright © 2025 - present KMX Systems. All rights reserved.
