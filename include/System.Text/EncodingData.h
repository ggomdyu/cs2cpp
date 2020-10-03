#pragma once

#include <array>
#include <string_view>

#include "System/ReadOnlySpan.h"

CS2CPP_NAMESPACE_BEGIN

class EncodingData final
{
public:
    EncodingData() = delete;

public:
    static constexpr size_t MIMECONTF_MAILNEWS = 0x00000001;
    static constexpr size_t MIMECONTF_BROWSER = 0x00000002;
    static constexpr size_t MIMECONTF_SAVABLE_MAILNEWS = 0x00000100;
    static constexpr size_t MIMECONTF_SAVABLE_BROWSER = 0x00000200;

    static constexpr std::array<std::u16string_view, 42> EncodingNames {
        u"ansi_x3.4-1968", // 20127
        u"ansi_x3.4-1986", // 20127
        u"ascii", // 20127
        u"cp367", // 20127
        u"cp819", // 28591
        u"csascii", // 20127
        u"csisolatin1", // 28591
        u"csunicode11utf7", // 65000
        u"ibm367", // 20127
        u"ibm819", // 28591
        u"iso-10646-ucs-2", // 1200
        u"iso-8859-1", // 28591
        u"iso-ir-100", // 28591
        u"iso-ir-6", // 20127
        u"iso646-us", // 20127
        u"iso8859-1", // 28591
        u"iso_646.irv:1991", // 20127
        u"iso_8859-1", // 28591
        u"iso_8859-1:1987", // 28591
        u"l1", // 28591
        u"latin1", // 28591
        u"ucs-2", // 1200
        u"unicode", // 1200
        u"unicode-1-1-utf-7", // 65000
        u"unicode-1-1-utf-8", // 65001
        u"unicode-2-0-utf-7", // 65000
        u"unicode-2-0-utf-8", // 65001
        u"unicodefffe", // 1201
        u"us", // 20127
        u"us-ascii", // 20127
        u"utf-16", // 1200
        u"utf-16be", // 1201
        u"utf-16le", // 1200
        u"utf-32", // 12000
        u"utf-32be", // 12001
        u"utf-32le", // 12000
        u"utf-7", // 65000
        u"utf-8", // 65001
        u"x-unicode-1-1-utf-7", // 65000
        u"x-unicode-1-1-utf-8", // 65001
        u"x-unicode-2-0-utf-7", // 65000
        u"x-unicode-2-0-utf-8", // 65001
    };

    static constexpr std::array<std::u16string_view, 8> WebNames {
        u"utf-16", // 1200
        u"utf-16BE", // 1201
        u"utf-32", // 12000
        u"utf-32BE", // 12001
        u"us-ascii", // 20127
        u"iso-8859-1", // 28591
        u"utf-7", // 65000
        u"utf-8", // 65001
    };

    static constexpr std::array<std::u16string_view, 8> EnglishNames {
        u"Unicode", // 1200
        u"Unicode (Big-Endian)", // 1201
        u"Unicode (UTF-32)", // 12000
        u"Unicode (UTF-32 Big-Endian)", // 12001
        u"US-ASCII", // 20127
        u"Western European (ISO)", // 28591
        u"Unicode (UTF-7)", // 65000
        u"Unicode (UTF-8)", // 65001
    };

    static constexpr std::array<int32_t, 42> CodePagesByName {
        20127, // ansi_x3.4-1968
        20127, // ansi_x3.4-1986
        20127, // ascii
        20127, // cp367
        28591, // cp819
        20127, // csascii
        28591, // csisolatin1
        65000, // csunicode11utf7
        20127, // ibm367
        28591, // ibm819
        1200, // iso-10646-ucs-2
        28591, // iso-8859-1
        28591, // iso-ir-100
        20127, // iso-ir-6
        20127, // iso646-us
        28591, // iso8859-1
        20127, // iso_646.irv:1991
        28591, // iso_8859-1
        28591, // iso_8859-1:1987
        28591, // l1
        28591, // latin1
        1200, // ucs-2
        1200, // unicode
        65000, // unicode-1-1-utf-7
        65001, // unicode-1-1-utf-8
        65000, // unicode-2-0-utf-7
        65001, // unicode-2-0-utf-8
        1201, // unicodefffe
        20127, // us
        20127, // us-ascii
        1200, // utf-16
        1201, // utf-16be
        1200, // utf-16le
        12000, // utf-32
        12001, // utf-32be
        12000, // utf-32le
        65000, // utf-7
        65001, // utf-8
        65000, // x-unicode-1-1-utf-7
        65001, // x-unicode-1-1-utf-8
        65000, // x-unicode-2-0-utf-7
        65001 // x-unicode-2-0-utf-8
    };

    static constexpr std::array<int32_t, 8> MappedCodePages {
        1200, // utf-16
        1201, // utf-16be
        12000, // utf-32
        12001, // utf-32be
        20127, // us-ascii
        28591, // iso-8859-1
        65000, // utf-7
        65001 // utf-8
    };

    static constexpr std::array<int32_t, 8> MappedCCSIDs {
        1202, // utf-16
        1200, // utf-16be
        1234, // utf-32
        1232, // utf-32be
        367, // us-ascii
        819, // iso-8859-1
        -1, // utf-7
        1208, // utf-8
    };

    static constexpr std::array<int32_t, 8> UiFamilyCodePages {
        1200,
        1200,
        1200,
        1200,
        1252,
        1252,
        1200,
        1200
    };

    inline static std::array<ReadOnlySpan<std::byte>, 8> Preambles {
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\xff\xfe"), 2), // utf-16
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\xfe\xff"), 2), // utf-16be
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\xff\xfe\x0\x0"), 4), // utf-32
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\x0\x0\xfe\xff"), 4), // utf-32be
        ReadOnlySpan<std::byte>(), // us-ascii
        ReadOnlySpan<std::byte>(), // iso-8859-1
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\x2b\x2f\x76"), 3), // utf-7
        ReadOnlySpan(reinterpret_cast<const std::byte*>("\xef\xbb\xbf"), 3), // utf-8
    };

    static constexpr std::array<uint32_t, 8> Flags {
        MIMECONTF_SAVABLE_BROWSER,
        0,
        0,
        0,
        MIMECONTF_MAILNEWS | MIMECONTF_SAVABLE_MAILNEWS,
        MIMECONTF_MAILNEWS | MIMECONTF_BROWSER | MIMECONTF_SAVABLE_MAILNEWS | MIMECONTF_SAVABLE_BROWSER,
        MIMECONTF_MAILNEWS | MIMECONTF_SAVABLE_MAILNEWS,
        MIMECONTF_MAILNEWS | MIMECONTF_BROWSER | MIMECONTF_SAVABLE_MAILNEWS | MIMECONTF_SAVABLE_BROWSER
    };
};

CS2CPP_NAMESPACE_END