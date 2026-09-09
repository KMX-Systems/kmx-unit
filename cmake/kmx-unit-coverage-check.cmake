# Copyright (c) 2025 - present KMX Systems. All rights reserved.
#
# Fails the build when the line coverage recorded in INFO is below MINIMUM percent. It is a script rather
# than a shell pipeline so that the `coverage` target behaves the same on every platform the library builds
# on, and so that the threshold is enforced by the build rather than by a reader of the report.
#
# Expects LCOV, INFO and MINIMUM to be defined on the command line with -D. REPORT is the HTML entry point
# the messages point a reader at; the build passes the path it actually wrote, since the report does not
# live at a fixed place relative to the caller.

foreach(required LCOV INFO MINIMUM)
    if(NOT DEFINED ${required})
        message(FATAL_ERROR "kmx-unit-coverage-check.cmake needs -D${required}=...")
    endif()
endforeach()

if(NOT DEFINED REPORT)
    get_filename_component(REPORT "${INFO}" DIRECTORY)
    set(REPORT "${REPORT}/html/index.html")
endif()

execute_process(
    COMMAND "${LCOV}" --summary "${INFO}"
    OUTPUT_VARIABLE coverage_output
    ERROR_VARIABLE coverage_error
    RESULT_VARIABLE coverage_result)

if(NOT coverage_result EQUAL 0)
    message(FATAL_ERROR "lcov could not summarize ${INFO}:\n${coverage_error}")
endif()

# lcov writes its summary to stderr, but not every version does, so both streams are searched.
set(coverage_report "${coverage_output}${coverage_error}")
if(NOT coverage_report MATCHES "lines[.]*: *([0-9]+([.][0-9]+)?)%")
    message(FATAL_ERROR "lcov reported no line coverage for ${INFO}:\n${coverage_report}")
endif()

set(measured "${CMAKE_MATCH_1}")
message(STATUS "kmx-unit line coverage: ${measured}% (minimum ${MINIMUM}%)")
message(STATUS "kmx-unit coverage report: ${REPORT}")

# CMake compares only integers, so both sides are scaled by a hundred to keep the fractional digits.
math(EXPR measured_scaled "0")
string(REGEX MATCH "^([0-9]+)[.]?([0-9]?)([0-9]?)" _ "${measured}")
set(whole "${CMAKE_MATCH_1}")
set(tenth "${CMAKE_MATCH_2}")
set(hundredth "${CMAKE_MATCH_3}")
if(tenth STREQUAL "")
    set(tenth "0")
endif()
if(hundredth STREQUAL "")
    set(hundredth "0")
endif()
math(EXPR measured_scaled "(${whole} * 100) + (${tenth} * 10) + ${hundredth}")
math(EXPR minimum_scaled "${MINIMUM} * 100")

if(measured_scaled LESS minimum_scaled)
    message(FATAL_ERROR
        "Line coverage ${measured}% is below the required ${MINIMUM}%. "
        "Open ${REPORT} to see which lines are not reached.")
endif()
