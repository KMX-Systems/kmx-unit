# Copyright (c) 2025 - present KMX Systems. All rights reserved.
cmake_minimum_required(VERSION 3.23)

# A build writes nothing into the source tree except under `output/`. Configuring in place would scatter the
# cache, the generated makefiles and the objects among the headers, and a `build/` beside them splits the
# generated files over two directories, so that a clean slate is no longer one deletion. A binary directory
# outside the tree writes nothing here and is left alone. The check precedes project(), which is the call
# that would write the cache this refuses to produce; being ahead of it also means PROJECT_IS_TOP_LEVEL is
# not set yet, so top level is read off the directories.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    cmake_path(SET kmx_unit_guard_source NORMALIZE "${CMAKE_SOURCE_DIR}")
    cmake_path(SET kmx_unit_guard_output NORMALIZE "${CMAKE_SOURCE_DIR}/output")
    # IS_PREFIX holds for a path equal to the prefix as well, so the in-source case is covered by the first
    # test and rejected by the second.
    cmake_path(IS_PREFIX kmx_unit_guard_source "${CMAKE_BINARY_DIR}" NORMALIZE kmx_unit_guard_in_tree)
    cmake_path(IS_PREFIX kmx_unit_guard_output "${CMAKE_BINARY_DIR}" NORMALIZE kmx_unit_guard_in_output)
    if(kmx_unit_guard_in_tree AND NOT kmx_unit_guard_in_output)
        message(FATAL_ERROR
            "kmx-unit writes every build artifact under ${kmx_unit_guard_output}, but the binary directory "
            "is ${CMAKE_BINARY_DIR}.\n"
            "  cmake --preset release      (or: cmake -S . -B output/cmake/release)")
    endif()
    unset(kmx_unit_guard_source)
    unset(kmx_unit_guard_output)
    unset(kmx_unit_guard_in_tree)
    unset(kmx_unit_guard_in_output)
endif()

# The version lives in the header, so that a consumer reading it and the installed package config always
# report the same thing. Repeating it here is what lets the two drift.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/source/inc/kmx/unit/version.hpp" KMX_UNIT_VERSION_HEADER)
foreach(component MAJOR MINOR PATCH)
    if(NOT KMX_UNIT_VERSION_HEADER MATCHES "define[ \t]+KMX_UNIT_VERSION_${component}[ \t]+([0-9]+)")
        message(FATAL_ERROR "KMX_UNIT_VERSION_${component} is not defined in kmx/unit/version.hpp")
    endif()
    set(KMX_UNIT_${component} "${CMAKE_MATCH_1}")
endforeach()
unset(KMX_UNIT_VERSION_HEADER)

project(kmx-unit
    VERSION "${KMX_UNIT_MAJOR}.${KMX_UNIT_MINOR}.${KMX_UNIT_PATCH}"
    DESCRIPTION "A header-only C++23 library of dimensional quantities"
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakeDependentOption)

# Everything this project produces belongs under one directory, so that a checkout has a single place to
# ignore and a single place to delete. The build directory is where the caller points `-B`, and the presets
# point it inside `output/`; this variable is what places the products that outlive a build directory - the
# coverage report - there as well, whichever build directory was configured.
if(PROJECT_IS_TOP_LEVEL)
    set(kmx_unit_output_default "${CMAKE_CURRENT_SOURCE_DIR}/output")
else()
    # A consumer's source tree is not this project's to write into.
    set(kmx_unit_output_default "${CMAKE_CURRENT_BINARY_DIR}/output")
endif()
set(KMX_UNIT_OUTPUT_DIR "${kmx_unit_output_default}" CACHE PATH
    "The directory every kmx-unit output that is not build-system bookkeeping is written under")
unset(kmx_unit_output_default)

# The library is header-only, so the tests and the example are built only when this project is the top
# level one, or when a consumer asks for them explicitly.
cmake_dependent_option(KMX_UNIT_BUILD_TESTS "Build the kmx-unit test suite" ON "PROJECT_IS_TOP_LEVEL" OFF)
cmake_dependent_option(KMX_UNIT_BUILD_EXAMPLE "Build the kmx-unit example" ON "PROJECT_IS_TOP_LEVEL" OFF)
option(KMX_UNIT_STRICT_WARNINGS "Compile the tests and the example with warnings as errors" OFF)
option(KMX_UNIT_SANITIZERS "Compile the tests and the example with the address and undefined behaviour sanitizers" OFF)
option(KMX_UNIT_COVERAGE "Instrument the tests for gcov line coverage and add the `coverage` target" OFF)
set(KMX_UNIT_COVERAGE_MINIMUM "97" CACHE STRING "The line coverage percentage the `coverage` target requires")

# The strict flags belong to this project's own targets. Putting them in CMAKE_CXX_FLAGS would apply them
# to a fetched dependency as well, which is not this project's code to police.
function(kmx_unit_apply_warnings target)
    if(NOT KMX_UNIT_STRICT_WARNINGS)
        return()
    endif()
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Werror)
    endif()
endfunction()

# The sanitizers instrument this project's own targets for the same reason. A dependency compiled without
# them still links and still runs: the runtime supports a partly instrumented program, and it is this
# library's code a sanitizer run is meant to check.
function(kmx_unit_apply_sanitizers target)
    if(NOT KMX_UNIT_SANITIZERS)
        return()
    endif()
    if(MSVC)
        target_compile_options(${target} PRIVATE /fsanitize=address)
    else()
        # Undefined behaviour is reported and then aborts. Left recoverable it prints and carries on, which
        # a test run reports as a pass.
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

# Coverage is measured on the library, which is entirely made of headers, so the counters live in the test
# binary and gcov attributes them back to the headers it included. Inlining is turned off so that a function
# defined in a header is counted where it is written rather than where it was folded into.
function(kmx_unit_apply_coverage target)
    if(NOT KMX_UNIT_COVERAGE)
        return()
    endif()
    if(MSVC)
        message(FATAL_ERROR "KMX_UNIT_COVERAGE needs a GCC or Clang toolchain.")
    endif()
    target_compile_options(${target} PRIVATE --coverage -O0 -g -fno-inline)
    target_link_options(${target} PRIVATE --coverage)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # lcov matches a header of a header-only library against the source tree only when gcov recorded an
        # absolute path for it; a relative one is resolved against the wrong directory and the file is lost.
        target_compile_options(${target} PRIVATE -fprofile-abs-path)
    endif()
endfunction()

add_library(kmx-unit INTERFACE)
add_library(kmx::unit ALIAS kmx-unit)

# An ALIAS is local to this build, so the installed package would otherwise export the target under its own
# name and a consumer would have to link `kmx::kmx-unit`. The export name is what makes `kmx::unit` mean the
# same thing through find_package as it does through add_subdirectory.
set_target_properties(kmx-unit PROPERTIES EXPORT_NAME unit)

target_compile_features(kmx-unit INTERFACE cxx_std_23)

# Two MSVC settings are requirements of the headers rather than preferences, so they travel with the target
# to every consumer. `/utf-8` reads the sources as UTF-8, which they are: a unit symbol such as "N·m" is
# mojibake under the system codepage. `/Zc:preprocessor` selects the conforming preprocessor, which the
# variadic KMX_UNIT_DEFINE_EX needs to expand an empty __VA_ARGS__ the way the standard describes.
target_compile_options(kmx-unit INTERFACE "$<$<CXX_COMPILER_ID:MSVC>:/utf-8;/Zc:preprocessor>")

target_include_directories(kmx-unit INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/inc>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

file(GLOB KMX_UNIT_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/inc/kmx/unit/*.hpp")
target_sources(kmx-unit INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/source/inc"
    FILES ${KMX_UNIT_HEADERS})

if(KMX_UNIT_BUILD_EXAMPLE)
    add_executable(kmx-unit-example example/src/kmx/unit/example.cpp)
    target_link_libraries(kmx-unit-example PRIVATE kmx::unit)
    kmx_unit_apply_warnings(kmx-unit-example)
    kmx_unit_apply_sanitizers(kmx-unit-example)
endif()

if(KMX_UNIT_BUILD_TESTS)
    # Catch2 is used when it is already installed, and fetched when it is not, so that a fresh checkout
    # builds its tests without any preparation.
    find_package(Catch2 3 QUIET)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(Catch2)
        list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    endif()
    enable_testing()

    file(GLOB KMX_UNIT_TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/src/kmx/unit/*.cpp")
    add_executable(kmx-unit-test ${KMX_UNIT_TEST_SOURCES})
    target_include_directories(kmx-unit-test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test/inc")
    target_link_libraries(kmx-unit-test PRIVATE kmx::unit Catch2::Catch2WithMain)
    kmx_unit_apply_warnings(kmx-unit-test)
    kmx_unit_apply_sanitizers(kmx-unit-test)
    kmx_unit_apply_coverage(kmx-unit-test)

    if(KMX_UNIT_COVERAGE)
        find_program(KMX_UNIT_LCOV NAMES lcov)
        find_program(KMX_UNIT_GENHTML NAMES genhtml)
        if(NOT KMX_UNIT_LCOV OR NOT KMX_UNIT_GENHTML)
            message(FATAL_ERROR "KMX_UNIT_COVERAGE needs lcov and genhtml (lcov 2.0 or newer) on the PATH.")
        endif()

        # gcov has to be the one belonging to the compiler that produced the counters, or it rejects their
        # format. A version-suffixed compiler names a version-suffixed gcov beside it.
        set(KMX_UNIT_GCOV "gcov")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            string(REGEX MATCH "-[0-9]+$" KMX_UNIT_COMPILER_SUFFIX "${CMAKE_CXX_COMPILER}")
            find_program(KMX_UNIT_GCOV_TOOL NAMES "gcov${KMX_UNIT_COMPILER_SUFFIX}" gcov)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            find_program(KMX_UNIT_GCOV_TOOL NAMES llvm-cov gcov)
        endif()
        if(KMX_UNIT_GCOV_TOOL)
            set(KMX_UNIT_GCOV "${KMX_UNIT_GCOV_TOOL}")
        endif()

        # A header-only library instantiates the same header in every translation unit, and the standard
        # headers it includes carry counters of their own. `unused` and `empty` cover the files the extract
        # then drops; `mismatch` and `negative` cover what gcov reports for an inlined standard header.
        set(KMX_UNIT_LCOV_TOLERANCE --ignore-errors mismatch,negative,unused,empty,gcov)
        set(KMX_UNIT_GENHTML_TOLERANCE --ignore-errors source,unmapped,inconsistent,category,empty,unused)

        # The report is a result to read rather than an intermediate of this build directory, so it is
        # written under the output directory by an absolute path. The counters themselves stay where the
        # compiler left them, beside the objects, which is where lcov is pointed to collect them.
        set(KMX_UNIT_COVERAGE_DIR "${KMX_UNIT_OUTPUT_DIR}/coverage")

        add_custom_target(coverage
            COMMAND "${CMAKE_COMMAND}" -E rm -rf "${KMX_UNIT_COVERAGE_DIR}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${KMX_UNIT_COVERAGE_DIR}"
            COMMAND "${KMX_UNIT_LCOV}" --quiet --zerocounters --directory .
            COMMAND "${CMAKE_COMMAND}" -E env "CXX=${CMAKE_CXX_COMPILER}"
                bash "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/run_with_compiler_runtime.sh"
                "$<TARGET_FILE:kmx-unit-test>"
            COMMAND "${KMX_UNIT_LCOV}" --quiet --capture --directory . --gcov-tool "${KMX_UNIT_GCOV}"
                    --output-file "${KMX_UNIT_COVERAGE_DIR}/all.info" ${KMX_UNIT_LCOV_TOLERANCE}
            COMMAND "${KMX_UNIT_LCOV}" --quiet --extract "${KMX_UNIT_COVERAGE_DIR}/all.info"
                    "${CMAKE_CURRENT_SOURCE_DIR}/source/inc/kmx/unit/*"
                    --output-file "${KMX_UNIT_COVERAGE_DIR}/library.info" ${KMX_UNIT_LCOV_TOLERANCE}
            COMMAND "${KMX_UNIT_GENHTML}" --quiet "${KMX_UNIT_COVERAGE_DIR}/library.info"
                    --output-directory "${KMX_UNIT_COVERAGE_DIR}/html"
                    --title "kmx-unit" --legend ${KMX_UNIT_GENHTML_TOLERANCE}
            COMMAND "${CMAKE_COMMAND}" -D "LCOV=${KMX_UNIT_LCOV}"
                    -D "INFO=${KMX_UNIT_COVERAGE_DIR}/library.info"
                    -D "REPORT=${KMX_UNIT_COVERAGE_DIR}/html/index.html"
                    -D "MINIMUM=${KMX_UNIT_COVERAGE_MINIMUM}"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/kmx-unit-coverage-check.cmake"
            DEPENDS kmx-unit-test
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Measuring the line coverage of the library headers"
            VERBATIM)
    endif()

    include(Catch)
    # The benchmarks carry the `[!benchmark]` tag and are skipped by a plain run; `ctest` runs the rest.
    # Discovery happens when the tests run rather than when they are built, so that building never has to
    # execute the freshly linked binary.
    catch_discover_tests(kmx-unit-test DISCOVERY_MODE PRE_TEST)

    add_custom_target(check
        COMMAND "${CMAKE_COMMAND}" -E env "CXX=${CMAKE_CXX_COMPILER}"
            bash "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/run_with_compiler_runtime.sh"
            "${CMAKE_CTEST_COMMAND}" --test-dir "${CMAKE_CURRENT_BINARY_DIR}" --output-on-failure
        DEPENDS kmx-unit-test
        USES_TERMINAL
        COMMENT "Running kmx-unit tests with the compiler runtime library path")
endif()

install(TARGETS kmx-unit EXPORT kmx-unit-targets FILE_SET HEADERS)
install(EXPORT kmx-unit-targets
    FILE kmx-unit-targets.cmake
    NAMESPACE kmx::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/kmx-unit")

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/kmx-unit-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/kmx-unit-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/kmx-unit")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/kmx-unit-config-version.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kmx-unit-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/kmx-unit-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/kmx-unit")
