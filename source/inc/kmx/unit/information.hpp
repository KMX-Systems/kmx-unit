/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/information.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
#endif

namespace kmx::unit::information
{
    /// @brief A unique dimension tag for units of information.
    struct dimension
    {
    };

    // Integer-based scale factors for perfect precision
    constexpr std::uint64_t bits_in_byte = 8ull;
    // Decimal (SI)
    constexpr std::uint64_t kilo = 1000ull;
    constexpr std::uint64_t mega = kilo * 1000ull;
    constexpr std::uint64_t giga = mega * 1000ull;
    constexpr std::uint64_t tera = giga * 1000ull;
    constexpr std::uint64_t peta = tera * 1000ull;
    // Binary (IEC)
    constexpr std::uint64_t kibi = 1024ull;
    constexpr std::uint64_t mebi = kibi * 1024ull;
    constexpr std::uint64_t gibi = mebi * 1024ull;
    constexpr std::uint64_t tebi = gibi * 1024ull;
    constexpr std::uint64_t pebi = tebi * 1024ull;

    // Base Units
    template <typename T = double>
    struct bit: base<bit<T>, dimension, T>
    {
        using base<bit<T>, dimension, T>::base;
        template <typename U>
        using rebind = bit<U>;
    };

    template <typename T = double>
    struct byte: base<byte<T>, dimension, T, static_cast<double>(bits_in_byte)>
    {
        using base<byte<T>, dimension, T, static_cast<double>(bits_in_byte)>::base;
        template <typename U>
        using rebind = byte<U>;
    };

    // Decimal (SI) Prefixes
    template <typename T = double>
    struct kilobit: base<kilobit<T>, dimension, T, static_cast<double>(kilo)>
    {
        using base<kilobit<T>, dimension, T, static_cast<double>(kilo)>::base;
        template <typename U>
        using rebind = kilobit<U>;
    };

    template <typename T = double>
    struct kilobyte: base<kilobyte<T>, dimension, T, static_cast<double>(bits_in_byte* kilo)>
    {
        using base<kilobyte<T>, dimension, T, static_cast<double>(bits_in_byte* kilo)>::base;
        template <typename U>
        using rebind = kilobyte<U>;
    };

    template <typename T = double>
    struct megabit: base<megabit<T>, dimension, T, static_cast<double>(mega)>
    {
        using base<megabit<T>, dimension, T, static_cast<double>(mega)>::base;
        template <typename U>
        using rebind = megabit<U>;
    };

    template <typename T = double>
    struct megabyte: base<megabyte<T>, dimension, T, static_cast<double>(bits_in_byte* mega)>
    {
        using base<megabyte<T>, dimension, T, static_cast<double>(bits_in_byte* mega)>::base;
        template <typename U>
        using rebind = megabyte<U>;
    };
    template <typename T = double>
    struct gigabit: base<gigabit<T>, dimension, T, static_cast<double>(giga)>
    {
        using base<gigabit<T>, dimension, T, static_cast<double>(giga)>::base;
        template <typename U>
        using rebind = gigabit<U>;
    };

    template <typename T = double>
    struct gigabyte: base<gigabyte<T>, dimension, T, static_cast<double>(bits_in_byte* giga)>
    {
        using base<gigabyte<T>, dimension, T, static_cast<double>(bits_in_byte* giga)>::base;
        template <typename U>
        using rebind = gigabyte<U>;
    };

    template <typename T = double>
    struct terabit: base<terabit<T>, dimension, T, static_cast<double>(tera)>
    {
        using base<terabit<T>, dimension, T, static_cast<double>(tera)>::base;
        template <typename U>
        using rebind = terabit<U>;
    };

    template <typename T = double>
    struct terabyte: base<terabyte<T>, dimension, T, static_cast<double>(bits_in_byte* tera)>
    {
        using base<terabyte<T>, dimension, T, static_cast<double>(bits_in_byte* tera)>::base;
        template <typename U>
        using rebind = terabyte<U>;
    };

    template <typename T = double>
    struct petabit: base<petabit<T>, dimension, T, static_cast<double>(peta)>
    {
        using base<petabit<T>, dimension, T, static_cast<double>(peta)>::base;
        template <typename U>
        using rebind = petabit<U>;
    };

    template <typename T = double>
    struct petabyte: base<petabyte<T>, dimension, T, static_cast<double>(bits_in_byte* peta)>
    {
        using base<petabyte<T>, dimension, T, static_cast<double>(bits_in_byte* peta)>::base;
        template <typename U>
        using rebind = petabyte<U>;
    };

    // Binary (IEC) Prefixes
    template <typename T = double>
    struct kibibit: base<kibibit<T>, dimension, T, static_cast<double>(kibi)>
    {
        using base<kibibit<T>, dimension, T, static_cast<double>(kibi)>::base;
        template <typename U>
        using rebind = kibibit<U>;
    };

    template <typename T = double>
    struct kibibyte: base<kibibyte<T>, dimension, T, static_cast<double>(bits_in_byte* kibi)>
    {
        using base<kibibyte<T>, dimension, T, static_cast<double>(bits_in_byte* kibi)>::base;
        template <typename U>
        using rebind = kibibyte<U>;
    };

    template <typename T = double>
    struct mebibit: base<mebibit<T>, dimension, T, static_cast<double>(mebi)>
    {
        using base<mebibit<T>, dimension, T, static_cast<double>(mebi)>::base;
        template <typename U>
        using rebind = mebibit<U>;
    };

    template <typename T = double>
    struct mebibyte: base<mebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* mebi)>
    {
        using base<mebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* mebi)>::base;
        template <typename U>
        using rebind = mebibyte<U>;
    };

    template <typename T = double>
    struct gibibit: base<gibibit<T>, dimension, T, static_cast<double>(gibi)>
    {
        using base<gibibit<T>, dimension, T, static_cast<double>(gibi)>::base;
        template <typename U>
        using rebind = gibibit<U>;
    };

    template <typename T = double>
    struct gibibyte: base<gibibyte<T>, dimension, T, static_cast<double>(bits_in_byte* gibi)>
    {
        using base<gibibyte<T>, dimension, T, static_cast<double>(bits_in_byte* gibi)>::base;
        template <typename U>
        using rebind = gibibyte<U>;
    };

    template <typename T = double>
    struct tebibit: base<tebibit<T>, dimension, T, static_cast<double>(tebi)>
    {
        using base<tebibit<T>, dimension, T, static_cast<double>(tebi)>::base;
        template <typename U>
        using rebind = tebibit<U>;
    };

    template <typename T = double>
    struct tebibyte: base<tebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* tebi)>
    {
        using base<tebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* tebi)>::base;
        template <typename U>
        using rebind = tebibyte<U>;
    };

    template <typename T = double>
    struct pebibit: base<pebibit<T>, dimension, T, static_cast<double>(pebi)>
    {
        using base<pebibit<T>, dimension, T, static_cast<double>(pebi)>::base;
        template <typename U>
        using rebind = pebibit<U>;
    };

    template <typename T = double>
    struct pebibyte: base<pebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* pebi)>
    {
        using base<pebibyte<T>, dimension, T, static_cast<double>(bits_in_byte* pebi)>::base;
        template <typename U>
        using rebind = pebibyte<U>;
    };
}

namespace kmx
{
    KMX_UNIT_FACTORY_FUNCTIONS(_b, unit::information::bit)
    KMX_UNIT_FACTORY_FUNCTIONS(_B, unit::information::byte)
    KMX_UNIT_FACTORY_FUNCTIONS(_kb, unit::information::kilobit)
    KMX_UNIT_FACTORY_FUNCTIONS(_kB, unit::information::kilobyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Mb, unit::information::megabit)
    KMX_UNIT_FACTORY_FUNCTIONS(_MB, unit::information::megabyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Gb, unit::information::gigabit)
    KMX_UNIT_FACTORY_FUNCTIONS(_GB, unit::information::gigabyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Tb, unit::information::terabit)
    KMX_UNIT_FACTORY_FUNCTIONS(_TB, unit::information::terabyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Pb, unit::information::petabit)
    KMX_UNIT_FACTORY_FUNCTIONS(_PB, unit::information::petabyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Kib, unit::information::kibibit)
    KMX_UNIT_FACTORY_FUNCTIONS(_KiB, unit::information::kibibyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Mib, unit::information::mebibit)
    KMX_UNIT_FACTORY_FUNCTIONS(_MiB, unit::information::mebibyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Gib, unit::information::gibibit)
    KMX_UNIT_FACTORY_FUNCTIONS(_GiB, unit::information::gibibyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Tib, unit::information::tebibit)
    KMX_UNIT_FACTORY_FUNCTIONS(_TiB, unit::information::tebibyte)
    KMX_UNIT_FACTORY_FUNCTIONS(_Pib, unit::information::pebibit)
    KMX_UNIT_FACTORY_FUNCTIONS(_PiB, unit::information::pebibyte)
}
