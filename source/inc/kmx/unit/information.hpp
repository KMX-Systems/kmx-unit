/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/information.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

/// @brief Units of information. The base unit of the family is the bit.
/// @details The family carries both prefix conventions and keeps them apart by type: the decimal
/// prefixes of the SI, where a kilobyte is a thousand bytes, and the binary prefixes of the IEC, where
/// a kibibyte is 1024 bytes. The scale factors are built from integer constants, so no prefix loses
/// precision to a rounded literal.
namespace kmx::unit::information
{
    /// @brief The dimension of the family, whose base unit is the bit.
    /// @details It is an alias of kmx::unit::dimension::information_t rather than a tag of its own, so a
    /// product or a quotient involving an information value goes through the ordinary dimension algebra:
    /// dividing by a time yields a data rate, and multiplying a data rate by a time yields information back.
    using dimension_t = ::kmx::unit::dimension::information_t;

    /// @brief The number of bits in one byte.
    constexpr std::uint64_t bits_in_byte = 8ull;

    /// @brief The size of one byte in bits, the factor every byte-denominated unit is built from.
    using bit_per_byte = scale::ratio<8>;

    /// @brief The size of a byte-denominated unit, given the prefix its count of bytes carries.
    /// @tparam Prefix The prefix, a kmx::unit::scale magnitude.
    template <typename Prefix>
    using bytes_of = scale::multiply_t<bit_per_byte, Prefix>;

    /// @brief The base unit of information, a single binary digit.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(bit, dimension_t, scale::one, "b")

    /// @brief Eight bits.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(byte, dimension_t, bit_per_byte, "B")

    /// @brief One thousand bits, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilobit, dimension_t, scale::kilo, "kb")

    /// @brief One thousand bytes, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kilobyte, dimension_t, bytes_of<scale::kilo>, "kB")

    /// @brief One million bits, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megabit, dimension_t, scale::mega, "Mb")

    /// @brief One million bytes, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(megabyte, dimension_t, bytes_of<scale::mega>, "MB")

    /// @brief One thousand million bits, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigabit, dimension_t, scale::giga, "Gb")

    /// @brief One thousand million bytes, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gigabyte, dimension_t, bytes_of<scale::giga>, "GB")

    /// @brief One million million bits, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(terabit, dimension_t, scale::tera, "Tb")

    /// @brief One million million bytes, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(terabyte, dimension_t, bytes_of<scale::tera>, "TB")

    /// @brief One thousand million million bits, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(petabit, dimension_t, scale::peta, "Pb")

    /// @brief One thousand million million bytes, the decimal prefix of the SI.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(petabyte, dimension_t, bytes_of<scale::peta>, "PB")

    /// @brief 1024 bits, the binary prefix of the IEC.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kibibit, dimension_t, scale::kibi, "Kib")

    /// @brief 1024 bytes, the binary prefix of the IEC.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(kibibyte, dimension_t, bytes_of<scale::kibi>, "KiB")

    /// @brief 1024 kibibits, that is 1048576 bits.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(mebibit, dimension_t, scale::mebi, "Mib")

    /// @brief 1024 kibibytes, that is 1048576 bytes.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(mebibyte, dimension_t, bytes_of<scale::mebi>, "MiB")

    /// @brief 1024 mebibits.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gibibit, dimension_t, scale::gibi, "Gib")

    /// @brief 1024 mebibytes.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(gibibyte, dimension_t, bytes_of<scale::gibi>, "GiB")

    /// @brief 1024 gibibits.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(tebibit, dimension_t, scale::tebi, "Tib")

    /// @brief 1024 gibibytes.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(tebibyte, dimension_t, bytes_of<scale::tebi>, "TiB")

    /// @brief 1024 tebibits.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(pebibit, dimension_t, scale::pebi, "Pib")

    /// @brief 1024 tebibytes.
    /// @tparam T The arithmetic type holding the value.
    KMX_UNIT_DEFINE(pebibyte, dimension_t, bytes_of<scale::pebi>, "PiB")
}

/// @brief The literal suffixes building information values, the terse form of this family.
namespace kmx::literals
{
    KMX_UNIT_LITERALS(b, unit::information::bit)
    KMX_UNIT_LITERALS(B, unit::information::byte)
    KMX_UNIT_LITERALS(kb, unit::information::kilobit)
    KMX_UNIT_LITERALS(kB, unit::information::kilobyte)
    KMX_UNIT_LITERALS(Mb, unit::information::megabit)
    KMX_UNIT_LITERALS(MB, unit::information::megabyte)
    KMX_UNIT_LITERALS(Gb, unit::information::gigabit)
    KMX_UNIT_LITERALS(GB, unit::information::gigabyte)
    KMX_UNIT_LITERALS(Tb, unit::information::terabit)
    KMX_UNIT_LITERALS(TB, unit::information::terabyte)
    KMX_UNIT_LITERALS(Pb, unit::information::petabit)
    KMX_UNIT_LITERALS(PB, unit::information::petabyte)
    KMX_UNIT_LITERALS(Kib, unit::information::kibibit)
    KMX_UNIT_LITERALS(KiB, unit::information::kibibyte)
    KMX_UNIT_LITERALS(Mib, unit::information::mebibit)
    KMX_UNIT_LITERALS(MiB, unit::information::mebibyte)
    KMX_UNIT_LITERALS(Gib, unit::information::gibibit)
    KMX_UNIT_LITERALS(GiB, unit::information::gibibyte)
    KMX_UNIT_LITERALS(Tib, unit::information::tebibit)
    KMX_UNIT_LITERALS(TiB, unit::information::tebibyte)
    KMX_UNIT_LITERALS(Pib, unit::information::pebibit)
    KMX_UNIT_LITERALS(PiB, unit::information::pebibyte)
}
