/// @copyright Copyright (c) 2025 - present KMX Systems. All rights reserved.
/// @file inc/kmx/unit/text.hpp
#pragma once
#ifndef PCH
    #include <kmx/unit/base.hpp>
    #include <array>
    #include <cstddef>
    #include <format>
    #include <ostream>
#endif

// The implementation details of the textual representation of a unit value; see kmx/unit/base.hpp for
// the namespace itself.
namespace kmx::unit::detail
{
    /// @brief Detects the unit types carrying a symbol, that is a static `text` member.
    /// @tparam T Candidate type.
    /// @tparam Enable Detection parameter, never named by a caller.
    template <typename T, typename Enable = void>
    struct has_text: std::false_type
    {
    };

    template <typename T>
    struct has_text<T, std::void_t<decltype(T::text)>>: std::true_type
    {
    };

    /// @brief Finds the brace closing a format specification.
    /// @details The end of a parse context is the end of the whole format string, not of the specification,
    /// and a width may itself be a nested argument such as `{2}`, so the closing brace is the first one at
    /// depth zero rather than simply the first one.
    /// @param first The first character of the specification.
    /// @param last One past the last character available.
    /// @return The position of the closing brace, or @p last when there is none.
    [[nodiscard]] constexpr const char* find_spec_end(const char* first, const char* const last) noexcept
    {
        std::size_t depth {};
        for (; first != last; ++first)
        {
            if (*first == '{')
                ++depth;
            else if (*first == '}')
            {
                if (depth == 0u)
                    break;

                --depth;
            }
        }

        return first;
    }

    /// @brief Tells whether a character is one of the three alignment indicators.
    /// @param value The character to test.
    /// @return True for '<', '>' and '^'.
    [[nodiscard]] constexpr bool is_alignment(const char value) noexcept
    {
        return (value == '<') || (value == '>') || (value == '^');
    }

    /// @brief A width or a precision as it was written: absent, a number, or a nested argument.
    /// @details `{:{}}` and `{:.{}f}` take the number from a formatting argument rather than from the
    /// specification. The argument is claimed while the specification is parsed, because that is when the
    /// automatic numbering runs and when the standard checks that the index exists, and it is read while
    /// the value is formatted, because that is when the arguments are at hand.
    struct format_number
    {
        /// @brief How the number was written.
        enum class kind_t : std::uint8_t
        {
            absent,  ///< The specification gave none.
            literal, ///< A number written in the specification itself.
            nested   ///< A number taken from a formatting argument.
        };

        /// @brief How the number was written.
        kind_t kind {kind_t::absent};

        /// @brief The number itself, or the index of the argument holding it.
        std::size_t value {};

        /// @brief Tells whether the specification gave a number at all.
        [[nodiscard]] constexpr bool given() const noexcept { return kind != kind_t::absent; }
    };

    /// @brief Reads a width or a precision, which may be written as a nested argument.
    /// @tparam ParseContext The parsing context type.
    /// @param target The number to fill in.
    /// @param context The parsing context, which numbers and checks a nested argument.
    /// @param it The first character to read.
    /// @param end One past the last character available.
    /// @return One past the last character consumed.
    /// @throws std::format_error when a nested argument is not closed or names nothing.
    template <typename ParseContext>
    constexpr const char* parse_format_number(format_number& target, ParseContext& context, const char* it,
                                              const char* const end) noexcept(false)
    {
        if ((it != end) && (*it == '{'))
        {
            ++it;
            if ((it != end) && (*it == '}'))
            {
                target = {format_number::kind_t::nested, context.next_arg_id()};
                return ++it;
            }

            const char* const first_digit = it;
            std::size_t index {};
            for (; (it != end) && (*it >= '0') && (*it <= '9'); ++it)
                index = (index * 10u) + static_cast<std::size_t>(*it - '0');

            if ((it == first_digit) || (it == end) || (*it != '}'))
                throw std::format_error {"a nested width or precision is not closed"};

            context.check_arg_id(index);
            target = {format_number::kind_t::nested, index};
            return ++it;
        }

        const char* const first_digit = it;
        std::size_t value {};
        for (; (it != end) && (*it >= '0') && (*it <= '9'); ++it)
            value = (value * 10u) + static_cast<std::size_t>(*it - '0');

        if (it != first_digit)
            target = {format_number::kind_t::literal, value};

        return it;
    }

    /// @brief A format specification for a unit value, split into the part belonging to the value as a
    /// whole and the part belonging to its number.
    /// @details A unit value prints as a number, a space and a symbol. A width applies to all three, so it
    /// is held here and the rest of the specification is rebuilt for the formatter of the number.
    /// @note Zero padding is the one width that stays with the number: it is written between the sign and
    /// the digits, which only the formatter of the number can do.
    struct unit_format_spec
    {
        /// @brief The character the padding is made of.
        char fill {' '};

        /// @brief The alignment, or the null character when the caller asked for none.
        char align {};

        /// @brief The sign, or the null character when the caller asked for none.
        char sign {};

        /// @brief The presentation type, or the null character when the caller asked for none.
        char type {};

        /// @brief The alternate form flag.
        bool alternate {};

        /// @brief The zero padding flag, which keeps the width with the number.
        bool zero_padded {};

        /// @brief The locale flag.
        bool localized {};

        /// @brief The width the whole value is padded to.
        format_number width {};

        /// @brief The precision handed to the formatter of the number.
        format_number precision {};
    };

    /// @brief Parses a standard format specification into its parts.
    /// @details The grammar is the standard one, so the parts arrive in the order the standard gives them:
    /// fill and alignment, sign, alternate form, zero padding, width, precision, locale, presentation type.
    /// @tparam ParseContext The parsing context type.
    /// @param spec The specification to fill in.
    /// @param context The parsing context, which numbers and checks a nested argument.
    /// @param it The first character of the specification.
    /// @param end One past its last character.
    /// @return One past the last character consumed; a caller treats anything left as an error.
    /// @throws std::format_error when a nested number or a precision is malformed.
    template <typename ParseContext>
    constexpr const char* parse_unit_format_spec(unit_format_spec& spec, ParseContext& context, const char* it,
                                                 const char* const end) noexcept(false)
    {
        if (((end - it) >= 2) && is_alignment(it[1]))
        {
            spec.fill = it[0];
            spec.align = it[1];
            it += 2;
        }
        else if ((it != end) && is_alignment(it[0]))
        {
            spec.align = *it++;
        }

        if ((it != end) && ((*it == '+') || (*it == '-') || (*it == ' ')))
            spec.sign = *it++;

        if ((it != end) && (*it == '#'))
        {
            spec.alternate = true;
            ++it;
        }

        if ((it != end) && (*it == '0'))
        {
            spec.zero_padded = true;
            ++it;
        }

        it = parse_format_number(spec.width, context, it, end);

        if ((it != end) && (*it == '.'))
        {
            it = parse_format_number(spec.precision, context, it + 1, end);
            if (!spec.precision.given())
                throw std::format_error {"a '.' must be followed by a precision"};
        }

        if ((it != end) && (*it == 'L'))
        {
            spec.localized = true;
            ++it;
        }

        // The presentation type is a single character and the last thing a specification holds, so whatever
        // follows it belongs to no part of the grammar and the caller rejects it.
        if (it != end)
            spec.type = *it++;

        return it;
    }

    /// @brief The format specification handed to the formatter of the number, assembled from the parts.
    /// @details It is rebuilt rather than copied out of the caller's specification, because a nested width
    /// or precision is a number by the time it is needed and copying it would leave an argument reference
    /// the formatter of the number cannot resolve: it is given the value alone.
    struct number_format
    {
        /// @brief The characters of the specification, braces included.
        /// @details Two flags, a zero, two numbers of at most twenty digits, a point, a locale flag, a
        /// presentation type and the four characters of `{:` and `}`.
        std::array<char, 64u> characters {};

        /// @brief The number of characters in use.
        std::size_t length {};

        /// @brief Appends one character.
        /// @param value The character to append.
        constexpr void append(const char value) noexcept { characters[length++] = value; }

        /// @brief Appends the decimal representation of a number.
        /// @param value The number to append.
        constexpr void append(std::size_t value) noexcept
        {
            std::array<char, 20u> digits {};
            std::size_t count {};
            do
            {
                digits[count++] = static_cast<char>('0' + (value % 10u));
                value /= 10u;
            } while (value != 0u);

            while (count != 0u)
                append(digits[--count]);
        }

        /// @brief The whole replacement field, as `std::vformat_to` wants it.
        [[nodiscard]] constexpr std::string_view text() const noexcept { return {characters.data(), length}; }

        /// @brief The specification alone, as a parsing context wants it.
        [[nodiscard]] constexpr std::string_view body() const noexcept { return {characters.data() + 2u, length - 3u}; }
    };

    /// @brief Assembles the format specification of the number.
    /// @param spec The parsed specification.
    /// @param width The resolved width; used only when the caller asked for a zero padding.
    /// @param precision The resolved precision.
    /// @return The specification of the number.
    [[nodiscard]] constexpr number_format make_number_format(const unit_format_spec& spec, const std::size_t width,
                                                             const std::size_t precision) noexcept
    {
        number_format result {};
        result.append('{');
        result.append(':');
        if (spec.sign != char {})
            result.append(spec.sign);

        if (spec.alternate)
            result.append('#');

        if (spec.zero_padded)
        {
            result.append('0');
            if (spec.width.given())
                result.append(width);
        }

        if (spec.precision.given())
        {
            result.append('.');
            result.append(precision);
        }

        if (spec.localized)
            result.append('L');

        if (spec.type != char {})
            result.append(spec.type);

        result.append('}');
        return result;
    }

    /// @brief Rejects a specification the formatter of the number would not accept.
    /// @details Which presentation types exist, which of them admit a precision and which admit the
    /// alternate form are the rules of the value type rather than of this library, so they are checked by
    /// handing the formatter of the number the very specification it will later be given. Doing it while
    /// parsing is what makes an invalid specification a compile-time error, exactly as it is for the value
    /// type itself: `std::format("{:Z}", 1.0)` does not compile, and neither should `{:Z}` on a metre.
    /// @note A nested width or precision is not known yet and stands in as a number that is always well
    /// formed, which cannot change whether the rest of the specification is. The width stands in as one
    /// rather than zero: a zero padding followed by a width of zero is not a specification the grammar
    /// admits, and validating it would reject `{:08.2f}` for being what it is not.
    /// @tparam Value The arithmetic type of the number.
    /// @param spec The parsed specification.
    /// @throws std::format_error when the formatter of the number rejects it.
    template <typename Value>
    constexpr void validate_number_format(const unit_format_spec& spec) noexcept(false)
    {
        const auto number = make_number_format(spec, 1u, 0u);
        std::basic_format_parse_context<char> context {number.body()};
        std::formatter<Value, char> number_formatter {};

        // The specification is assembled here rather than copied from the caller, so it holds no closing
        // brace and nothing else the formatter of the number would stop short of: it either consumes the
        // whole of it or throws, and the throw is the answer this function exists to obtain.
        static_cast<void>(number_formatter.parse(context));
    }

    /// @brief Reads a width or a precision out of a formatting argument.
    /// @details The standard admits any integer type and rejects everything else, a negative value
    /// included.
    struct format_number_reader
    {
        /// @brief Reads one argument.
        /// @tparam T The type the argument holds.
        /// @param value The argument.
        /// @return The width or precision it holds.
        /// @throws std::format_error when the argument is not a non-negative integer.
        template <typename T>
        [[nodiscard]] constexpr std::size_t operator()(const T value) const noexcept(false)
        {
            if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>)
            {
                if constexpr (std::is_signed_v<T>)
                {
                    if (value < T {})
                        throw std::format_error {"a width or a precision must not be negative"};
                }

                return static_cast<std::size_t>(value);
            }
            else
            {
                throw std::format_error {"a width or a precision must be an integer"};
            }
        }
    };

    /// @brief Resolves a width or a precision against the formatting arguments.
    /// @tparam Context The formatting context type.
    /// @param number The width or precision as it was written.
    /// @param context The formatting context.
    /// @return The number; zero when the specification gave none.
    /// @throws std::format_error when a nested argument is not a non-negative integer.
    template <typename Context>
    [[nodiscard]] constexpr std::size_t resolve_format_number(const format_number& number, Context& context) noexcept(false)
    {
        if (number.kind != format_number::kind_t::nested)
            return number.value;

        const auto argument = context.arg(number.value);

        // std::visit_format_arg is the C++20 spelling and is deprecated by the member the C++26 one adds.
        if constexpr (requires { argument.visit(format_number_reader {}); })
            return argument.visit(format_number_reader {});
        else
            return std::visit_format_arg(format_number_reader {}, argument);
    }

    /// @brief Writes a run of one character.
    /// @tparam Out The output iterator type.
    /// @param out The destination.
    /// @param value The character to write.
    /// @param count The number of copies to write.
    /// @return The iterator past the last character written.
    template <typename Out>
    constexpr Out write_fill(Out out, const char value, const std::size_t count) noexcept(false)
    {
        for (std::size_t i {}; i != count; ++i)
            *out++ = value;

        return out;
    }
}

namespace kmx::unit::detail
{
    /// @brief The capacity of the buffer a dimension symbol is assembled in.
    /// @details Eight base quantities, each with a symbol of at most three characters, a separator and an
    /// exponent of a few digits. The bound is generous rather than tight, and it is only ever occupied
    /// during constant evaluation.
    inline constexpr std::size_t dimension_symbol_capacity = 128u;

    /// @brief A fixed capacity character buffer a dimension symbol is assembled in at compile time.
    struct symbol_buffer
    {
        /// @brief The characters written so far.
        std::array<char, dimension_symbol_capacity> characters {};

        /// @brief The number of characters written so far.
        std::size_t length {};

        /// @brief Appends text.
        /// @param text The text to append.
        constexpr void append(const std::string_view text) noexcept
        {
            for (const char item: text)
                characters[length++] = item;
        }

        /// @brief Appends the decimal representation of a number.
        /// @param number The number to append.
        constexpr void append(std::intmax_t number) noexcept
        {
            if (number < 0)
            {
                characters[length++] = '-';
                number = -number;
            }

            std::array<char, 24u> digits {};
            std::size_t count {};
            do
            {
                digits[count++] = static_cast<char>('0' + (number % 10));
                number /= 10;
            } while (number != 0);

            while (count != 0)
                characters[length++] = digits[--count];
        }
    };

    /// @brief Appends one dimensional component, such as "kg", "m^2" or "s^-1".
    /// @details A component whose exponent is zero appends nothing, so a dimension names only the base
    /// quantities it is actually built from.
    /// @tparam Ratio The exponent of the component, a std::ratio instantiation.
    /// @param buffer The buffer to append to.
    /// @param symbol The symbol of the base quantity.
    /// @param is_first Tells whether a separator is still to be written; set to false once one is.
    template <typename Ratio>
    constexpr void append_component(symbol_buffer& buffer, const std::string_view symbol, bool& is_first) noexcept
    {
        if constexpr (Ratio::num != 0)
        {
            if (!is_first)
                buffer.append("\u00b7"); // a middle dot separates two components

            buffer.append(symbol);
            if constexpr ((Ratio::num != 1) || (Ratio::den != 1))
            {
                buffer.append("^");
                buffer.append(static_cast<std::intmax_t>(Ratio::num));
                if constexpr (Ratio::den != 1)
                {
                    buffer.append("/");
                    buffer.append(static_cast<std::intmax_t>(Ratio::den));
                }
            }

            is_first = false;
        }
    }

    /// @brief Assembles the symbol of a dimension.
    /// @details The components follow the conventional order of the base quantities, kg m s A K mol cd,
    /// with the information exponent last, so that a dimension always reads the same way.
    /// @tparam Dimension The dimension to name.
    /// @return The buffer holding the symbol.
    template <typename Dimension>
    [[nodiscard]] constexpr symbol_buffer make_dimension_symbol() noexcept
    {
        symbol_buffer buffer {};
        bool is_first {true};
        using params_t = dimension::params<Dimension>;
        append_component<typename params_t::m_t>(buffer, "kg", is_first);
        append_component<typename params_t::l_t>(buffer, "m", is_first);
        append_component<typename params_t::t_t>(buffer, "s", is_first);
        append_component<typename params_t::i_t>(buffer, "A", is_first);
        append_component<typename params_t::th_t>(buffer, "K", is_first);
        append_component<typename params_t::n_t>(buffer, "mol", is_first);
        append_component<typename params_t::j_t>(buffer, "cd", is_first);
        append_component<typename params_t::b_t>(buffer, "b", is_first);
        return buffer;
    }

    /// @brief The storage backing the symbol of a dimension.
    /// @tparam Dimension The dimension.
    template <typename Dimension>
    inline constexpr symbol_buffer dimension_symbol_storage = make_dimension_symbol<Dimension>();

    /// @brief The symbol of a dimension, assembled once at compile time.
    /// @details A value that has no named unit prints through this rather than by walking its exponents on
    /// every call, which is also what lets such a value be formatted as well as streamed.
    /// @tparam Dimension The dimension.
    template <typename Dimension>
    inline constexpr std::string_view dimension_symbol {dimension_symbol_storage<Dimension>.characters.data(),
                                                        dimension_symbol_storage<Dimension>.length};

    /// @brief The width a piece of text occupies, in characters rather than in bytes.
    /// @details A symbol such as "N·m" holds a middle dot, which UTF-8 spells in two bytes and a terminal
    /// draws in one column. Padding by the size in bytes would indent such a value by one too few. Every
    /// character this library prints is a single column wide, so counting the bytes that begin one is
    /// exactly the width.
    /// @param text The text to measure.
    /// @return The number of characters in @p text.
    [[nodiscard]] constexpr std::size_t text_width(const std::string_view text) noexcept
    {
        std::size_t width {};
        for (const char item: text)
            width += ((static_cast<unsigned char>(item) & 0xc0u) != 0x80u) ? 1u : 0u;

        return width;
    }

    /// @brief The width a symbol occupies after a number, the space separating the two included.
    /// @details An empty symbol writes nothing at all rather than a trailing space, so a value with no
    /// named unit and no dimension prints as a plain number.
    /// @param symbol The symbol, written without its separating space.
    /// @return The number of characters kmx::unit::detail::write_symbol writes for @p symbol.
    [[nodiscard]] constexpr std::size_t symbol_width(const std::string_view symbol) noexcept
    {
        return symbol.empty() ? 0u : (1u + text_width(symbol));
    }

    /// @brief Writes a symbol after a number, preceded by the space separating the two.
    /// @details The symbol is written straight to the output iterator rather than assembled in a buffer
    /// first, so a unit of any symbol length formats. A fixed buffer would have to bound a symbol that
    /// KMX_UNIT_DEFINE lets a caller choose freely, and a UTF-8 one spends several bytes per character.
    /// @tparam Out The output iterator type.
    /// @param out The destination.
    /// @param symbol The symbol, written without its separating space; an empty one writes nothing.
    /// @return The iterator past the last character written.
    /// @throws Whatever @p out throws.
    template <typename Out>
    constexpr Out write_symbol(Out out, const std::string_view symbol) noexcept(false)
    {
        if (!symbol.empty())
        {
            *out++ = ' ';
            for (const char item: symbol)
                *out++ = item;
        }

        return out;
    }

    /// @brief The shared behaviour of the two formatters: parsing a specification and rendering a value
    /// with its symbol, padded as one.
    /// @details A named unit and a value with no named unit differ only in where their symbol comes from,
    /// so everything else is written once here and both formatters derive from it.
    /// @tparam Value The arithmetic type of the number.
    template <typename Value>
    struct unit_formatter
    {
        /// @brief The parsed specification.
        unit_format_spec spec {};

        /// @brief Reads the format specification.
        /// @details A nested width or precision claims its argument here, which is what numbers it
        /// correctly among the arguments of the whole format string, and the formatter of the number
        /// rejects anything it would not accept, which is what makes an invalid specification a
        /// compile-time error.
        /// @tparam ParseContext The parsing context type.
        /// @param context The parsing context.
        /// @return The iterator past the last character consumed.
        /// @throws std::format_error when the specification is not one this formatter accepts.
        template <typename ParseContext>
        constexpr auto parse(ParseContext& context) noexcept(false)
        {
            const auto* const first = context.begin();
            const auto* const stop = find_spec_end(first, context.end());
            if (parse_unit_format_spec(spec, context, first, stop) != stop)
                throw std::format_error {"invalid format specification for a unit value"};

            validate_number_format<Value>(spec);
            return stop;
        }

        /// @brief Writes a number and a symbol, padded to the width as one value.
        /// @tparam Context The formatting context type.
        /// @param value The number.
        /// @param symbol The symbol written after the number, without its separating space.
        /// @param context The formatting context.
        /// @return The iterator past the last character written.
        /// @throws Whatever the underlying formatter or the output iterator throws.
        template <typename Context>
        auto format_value(const Value& value, const std::string_view symbol, Context& context) const noexcept(false)
        {
            const auto width = resolve_format_number(spec.width, context);
            const auto number = make_number_format(spec, width, resolve_format_number(spec.precision, context));
            const auto format_string = number.text();

            // A zero padding is written between the sign and the digits, so that width was handed to the
            // number and the value as a whole is not padded a second time.
            const std::size_t total_width = spec.zero_padded ? 0u : width;

            auto out = context.out();
            if (total_width == 0u)
            {
                out = std::vformat_to(out, format_string, std::make_format_args(value));
                return write_symbol(out, symbol);
            }

            // A width on the whole value no longer formats the number twice: it is rendered once, measured,
            // then padded and copied out.
            const auto rendered_number = std::vformat(format_string, std::make_format_args(value));
            const std::size_t total = rendered_number.size() + symbol_width(symbol);
            const std::size_t padding = (total < total_width) ? (total_width - total) : 0u;

            // A number is right-aligned by default, and a unit value is a number, so it keeps that default.
            const std::size_t before = (spec.align == '<') ? 0u : ((spec.align == '^') ? (padding / 2u) : padding);
            out = write_fill(out, spec.fill, before);
            for (const char item: rendered_number)
                *out++ = item;

            out = write_symbol(out, symbol);
            return write_fill(out, spec.fill, padding - before);
        }
    };
}

namespace kmx::unit
{
    /// @brief Writes a named unit value as its native value followed by the symbol of its unit.
    /// @tparam Unit A unit type carrying a `text` member.
    /// @param os The destination stream.
    /// @param unit The value to write.
    /// @return @p os.
    /// @throws Whatever @p os throws; a stream may be configured to report a failure by an exception.
    template <typename Unit>
    std::ostream& operator<<(std::ostream& os, const Unit& unit) noexcept(false)
        requires(detail::has_text<Unit>::value)
    {
        return os << unit.as_native() << ' ' << Unit::text;
    }

    /// @brief Writes a value that has no named unit as its value followed by the symbols of the base
    /// quantities its dimension is built from.
    /// @tparam Dimension The dimension of the value.
    /// @tparam T The arithmetic type holding the value.
    /// @param os The destination stream.
    /// @param unit The value to write.
    /// @return @p os.
    /// @throws Whatever @p os throws; a stream may be configured to report a failure by an exception.
    template <typename Dimension, typename T>
    std::ostream& operator<<(std::ostream& os, const si_unit<Dimension, T>& unit) noexcept(false)
    {
        os << unit.as_native();

        // A dimensionless value is a plain number, so naming a base quantity after it would be misleading.
        if constexpr (!detail::dimension_symbol<Dimension>.empty())
            os << ' ' << detail::dimension_symbol<Dimension>;

        return os;
    }
}

namespace std
{
    /// @brief Formats any named unit value as its native value followed by the unit text.
    /// @details The format specification is the one of the underlying value type, so `{:.1f}` on a
    /// kmx::unit::distance::meter value formats the number and leaves the symbol untouched, and one the
    /// value type would reject is rejected here too, at compile time.
    /// @tparam Unit A unit type carrying a `text` member.
    template <typename Unit>
        requires(kmx::unit::unit_type<Unit> && kmx::unit::detail::has_text<Unit>::value)
    struct formatter<Unit, char>: kmx::unit::detail::unit_formatter<typename Unit::value_t>
    {
        /// @brief Writes the formatted number, a space and the unit text, padded to the width as one value.
        /// @tparam Context The formatting context type.
        /// @param unit The value to format.
        /// @param context The formatting context.
        /// @return The iterator past the last character written.
        /// @throws Whatever the underlying formatter or the output iterator throws.
        template <typename Context>
        auto format(const Unit& unit, Context& context) const noexcept(false)
        {
            const auto value = unit.as_native();
            return this->format_value(value, Unit::text, context);
        }
    };

    /// @brief Formats a value that has no named unit as its value followed by the symbols of the base
    /// quantities its dimension is built from.
    /// @details The format specification is the one of the underlying value type, so `{:.2f}` on the
    /// product of two lengths formats the number and leaves the symbol untouched.
    /// @tparam Dimension The dimension of the value.
    /// @tparam T The arithmetic type holding the value.
    template <typename Dimension, typename T>
    struct formatter<kmx::unit::si_unit<Dimension, T>, char>: kmx::unit::detail::unit_formatter<T>
    {
        /// @brief Writes the formatted number and the symbol of the dimension, padded as one value.
        /// @tparam Context The formatting context type.
        /// @param unit The value to format.
        /// @param context The formatting context.
        /// @return The iterator past the last character written.
        /// @throws Whatever the underlying formatter or the output iterator throws.
        template <typename Context>
        auto format(const kmx::unit::si_unit<Dimension, T>& unit, Context& context) const noexcept(false)
        {
            const auto value = unit.as_native();

            // A dimensionless value is a plain number, so naming a base quantity after it would mislead.
            // Its symbol is empty and kmx::unit::detail::write_symbol then writes nothing, not a trailing space.
            return this->format_value(value, kmx::unit::detail::dimension_symbol<Dimension>, context);
        }
    };
}
