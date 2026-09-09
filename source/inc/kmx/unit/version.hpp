/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/version.hpp
/// @brief The version of the library, in the two forms a consumer needs it.
/// @details This header is the single source of the version. The CMake build reads the three numbers back
/// out of it rather than repeating them, so the package version installed alongside the headers and the
/// version the headers report can never disagree.
#pragma once
#ifndef PCH
    #include <cstdint>
    #include <string_view>
#endif

/// @brief The major version. It changes when a release breaks source compatibility.
#define KMX_UNIT_VERSION_MAJOR 1

/// @brief The minor version. It changes when a release adds to the interface compatibly.
#define KMX_UNIT_VERSION_MINOR 0

/// @brief The patch version. It changes when a release only fixes behaviour.
#define KMX_UNIT_VERSION_PATCH 0

/// @brief The whole version as one integer, the form an ordered preprocessor comparison needs.
/// @details Each component occupies two decimal digits, so a consumer guards a feature with
/// `#if KMX_UNIT_VERSION >= KMX_UNIT_VERSION_NUMBER(1, 2, 0)`.
#define KMX_UNIT_VERSION_NUMBER(major, minor, patch) (((major) * 10000) + ((minor) * 100) + (patch))

/// @brief The version of this library as one integer; see KMX_UNIT_VERSION_NUMBER.
#define KMX_UNIT_VERSION KMX_UNIT_VERSION_NUMBER(KMX_UNIT_VERSION_MAJOR, KMX_UNIT_VERSION_MINOR, KMX_UNIT_VERSION_PATCH)

/// @brief Turns the expansion of a macro into a string literal; not part of the interface.
#define KMX_UNIT_DETAIL_STRINGIFY_(value) #value

/// @brief Turns @p value into a string literal after expanding it; not part of the interface.
#define KMX_UNIT_DETAIL_STRINGIFY(value) KMX_UNIT_DETAIL_STRINGIFY_(value)

/// @brief The version as a string literal, such as "1.0.0", for a banner or a log line.
#define KMX_UNIT_VERSION_TEXT                         \
    KMX_UNIT_DETAIL_STRINGIFY(KMX_UNIT_VERSION_MAJOR) \
    "." KMX_UNIT_DETAIL_STRINGIFY(KMX_UNIT_VERSION_MINOR) "." KMX_UNIT_DETAIL_STRINGIFY(KMX_UNIT_VERSION_PATCH)

namespace kmx::unit
{
    /// @brief The major version; see KMX_UNIT_VERSION_MAJOR.
    inline constexpr std::uint32_t version_major = KMX_UNIT_VERSION_MAJOR;

    /// @brief The minor version; see KMX_UNIT_VERSION_MINOR.
    inline constexpr std::uint32_t version_minor = KMX_UNIT_VERSION_MINOR;

    /// @brief The patch version; see KMX_UNIT_VERSION_PATCH.
    inline constexpr std::uint32_t version_patch = KMX_UNIT_VERSION_PATCH;

    /// @brief The whole version as one integer, the form an ordered comparison needs.
    /// @details This is the constant to compare in code; KMX_UNIT_VERSION is the same value for the
    /// preprocessor, where a conditional inclusion has no other choice.
    inline constexpr std::uint32_t version = KMX_UNIT_VERSION;

    /// @brief The version as text, such as "1.0.0".
    inline constexpr std::string_view version_text = KMX_UNIT_VERSION_TEXT;
}
