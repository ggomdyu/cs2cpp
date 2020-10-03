#include <array>
#include <System.Text/Encoding.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(ICUEncoding, Convert)
{
    std::byte utf8Bytes[] = {(std::byte)0x48, (std::byte)0xEC, (std::byte)0x99, (std::byte)0x80, (std::byte)0x6C, (std::byte)0x6C};
    auto convertedBytes = Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), utf8Bytes);
    auto convertedUtf16Bytes = reinterpret_cast<const char16_t*>(convertedBytes->data());
    EXPECT_TRUE(convertedBytes.has_value());
    EXPECT_EQ(convertedBytes->size(), 4 * sizeof(char16_t));
    EXPECT_EQ(convertedUtf16Bytes[0], u'H');
    EXPECT_EQ(convertedUtf16Bytes[1], u'\xC640');
    EXPECT_EQ(convertedUtf16Bytes[2], u'l');
    EXPECT_EQ(convertedUtf16Bytes[3], u'l');
    EXPECT_EQ(convertedUtf16Bytes[4], u'\0');

    auto bufferA = std::array<std::byte, 9>{};
    auto convertedByteCount = Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), utf8Bytes, bufferA);
    EXPECT_TRUE(convertedBytes.has_value());

    auto bufferB = std::array<std::byte, 10>{};
    convertedByteCount = Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), utf8Bytes, bufferB);
    convertedUtf16Bytes = reinterpret_cast<const char16_t*>(bufferB.data());
    EXPECT_TRUE(convertedByteCount.has_value());
    EXPECT_EQ(convertedByteCount.value(), 8);
    EXPECT_EQ(convertedUtf16Bytes[0], u'H');
    EXPECT_EQ(convertedUtf16Bytes[1], u'\xC640');
    EXPECT_EQ(convertedUtf16Bytes[2], u'l');
    EXPECT_EQ(convertedUtf16Bytes[3], u'l');
    EXPECT_EQ(convertedUtf16Bytes[4], u'\0');
}

TEST(ICUEncoding, GetEncoding)
{
    EXPECT_EQ(Encoding::GetEncoding(65001), Encoding::GetEncoding(u"UTF-8"));
    EXPECT_EQ(Encoding::GetEncoding(1200), Encoding::GetEncoding(u"UTF-16"));
    EXPECT_EQ(Encoding::GetEncoding(1201), Encoding::GetEncoding(u"UTF-16BE"));
    EXPECT_EQ(Encoding::GetEncoding(12000), Encoding::GetEncoding(u"UTF-32"));
    EXPECT_EQ(Encoding::GetEncoding(12001), Encoding::GetEncoding(u"UTF-32BE"));

    auto encodingA = Encoding::GetEncoding(65001, {}, {});
    auto encodingB = Encoding::GetEncoding(65001, {}, {});
    EXPECT_TRUE(encodingA != nullptr);
    EXPECT_TRUE(encodingB != nullptr);
    EXPECT_NE(&(*encodingA), &(*encodingB));

    auto encodingC = Encoding::GetEncoding(u"UTF-8", {}, {});
    auto encodingD = Encoding::GetEncoding(u"UTF-8", {}, {});
    EXPECT_TRUE(encodingC != nullptr);
    EXPECT_TRUE(encodingD != nullptr);
    EXPECT_NE(&(*encodingC), &(*encodingD));
}

TEST(ICUEncoding, GetEncodings)
{
    EXPECT_GT(Encoding::GetEncodings().size(), 0);
}

TEST(ICUEncoding, Preamble)
{
    auto preamble = Encoding::UTF8().Preamble();
    EXPECT_EQ(preamble.Length(), 3);
    EXPECT_EQ(preamble[0], std::byte(239));
    EXPECT_EQ(preamble[1], std::byte(187));
    EXPECT_EQ(preamble[2], std::byte(191));

    preamble = Encoding::Unicode().Preamble();
    EXPECT_EQ(preamble.Length(), 2);
    EXPECT_EQ(preamble[0], std::byte(255));
    EXPECT_EQ(preamble[1], std::byte(254));

    preamble = Encoding::BigEndianUnicode().Preamble();
    EXPECT_EQ(preamble.Length(), 2);
    EXPECT_EQ(preamble[0], std::byte(254));
    EXPECT_EQ(preamble[1], std::byte(255));

    preamble = Encoding::UTF32().Preamble();
    EXPECT_EQ(preamble.Length(), 4);
    EXPECT_EQ(preamble[0], std::byte(255));
    EXPECT_EQ(preamble[1], std::byte(254));
    EXPECT_EQ(preamble[2], std::byte(0));
    EXPECT_EQ(preamble[3], std::byte(0));
}

TEST(ICUEncoding, BodyName)
{
    EXPECT_NE(Encoding::UTF8().BodyName().find(u"utf-8"), std::string::npos);
    EXPECT_NE(Encoding::Unicode().BodyName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode().BodyName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::UTF32().BodyName().find(u"utf-32"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode32().BodyName().find(u"utf-32"), std::string::npos);
}

TEST(ICUEncoding, EncodingName)
{
    EXPECT_NE(Encoding::UTF8().EncodingName().find(u"UTF-8"), std::string::npos);
    EXPECT_NE(Encoding::Unicode().EncodingName().find(u"Unicode"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode().EncodingName().find(u"Unicode"), std::string::npos);
    EXPECT_NE(Encoding::UTF32().EncodingName().find(u"UTF-32"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode32().EncodingName().find(u"UTF-32"), std::string::npos);
}

TEST(ICUEncoding, HeaderName)
{
    EXPECT_NE(Encoding::UTF8().HeaderName().find(u"utf-8"), std::string::npos);
    EXPECT_NE(Encoding::Unicode().HeaderName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode().HeaderName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::UTF32().HeaderName().find(u"utf-32"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode32().HeaderName().find(u"utf-32"), std::string::npos);
}

TEST(ICUEncoding, WebName)
{
    EXPECT_NE(Encoding::UTF8().WebName().find(u"utf-8"), std::string::npos);
    EXPECT_NE(Encoding::Unicode().WebName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode().WebName().find(u"utf-16"), std::string::npos);
    EXPECT_NE(Encoding::UTF32().WebName().find(u"utf-32"), std::string::npos);
    EXPECT_NE(Encoding::BigEndianUnicode32().WebName().find(u"utf-32"), std::string::npos);
}

TEST(ICUEncoding, WindowsCodePage)
{
    EXPECT_EQ(Encoding::UTF8().WindowsCodePage(), 1200);
    EXPECT_EQ(Encoding::Unicode().WindowsCodePage(), 1200);
    EXPECT_EQ(Encoding::BigEndianUnicode().WindowsCodePage(), 1200);
    EXPECT_EQ(Encoding::UTF32().WindowsCodePage(), 1200);
    EXPECT_EQ(Encoding::BigEndianUnicode32().WindowsCodePage(), 1200);
    EXPECT_EQ(Encoding::ASCII().WindowsCodePage(), 1252);
    EXPECT_EQ(Encoding::Latin1().WindowsCodePage(), 1252);
}

TEST(ICUEncoding, IsBrowserDisplay)
{
    EXPECT_EQ(Encoding::UTF8().IsBrowserDisplay(), true);
    EXPECT_EQ(Encoding::Unicode().IsBrowserDisplay(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode().IsBrowserDisplay(), false);
    EXPECT_EQ(Encoding::UTF32().IsBrowserDisplay(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode32().IsBrowserDisplay(), false);
    EXPECT_EQ(Encoding::ASCII().IsBrowserDisplay(), false);
    EXPECT_EQ(Encoding::Latin1().IsBrowserDisplay(), true);
}

TEST(ICUEncoding, IsBrowserSave)
{
    EXPECT_EQ(Encoding::UTF8().IsBrowserSave(), true);
    EXPECT_EQ(Encoding::Unicode().IsBrowserSave(), true);
    EXPECT_EQ(Encoding::BigEndianUnicode().IsBrowserSave(), false);
    EXPECT_EQ(Encoding::UTF32().IsBrowserSave(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode32().IsBrowserSave(), false);
    EXPECT_EQ(Encoding::ASCII().IsBrowserSave(), false);
    EXPECT_EQ(Encoding::Latin1().IsBrowserSave(), true);
}

TEST(ICUEncoding, IsMailNewsDisplay)
{
    EXPECT_EQ(Encoding::UTF8().IsMailNewsDisplay(), true);
    EXPECT_EQ(Encoding::Unicode().IsMailNewsDisplay(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode().IsMailNewsDisplay(), false);
    EXPECT_EQ(Encoding::UTF32().IsMailNewsDisplay(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode32().IsMailNewsDisplay(), false);
    EXPECT_EQ(Encoding::ASCII().IsMailNewsDisplay(), true);
    EXPECT_EQ(Encoding::Latin1().IsMailNewsDisplay(), true);
}

TEST(ICUEncoding, IsMailNewsSave)
{
    EXPECT_EQ(Encoding::UTF8().IsMailNewsSave(), true);
    EXPECT_EQ(Encoding::Unicode().IsMailNewsSave(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode().IsMailNewsSave(), false);
    EXPECT_EQ(Encoding::UTF32().IsMailNewsSave(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode32().IsMailNewsSave(), false);
    EXPECT_EQ(Encoding::ASCII().IsMailNewsSave(), true);
    EXPECT_EQ(Encoding::Latin1().IsMailNewsSave(), true);
}

TEST(ICUEncoding, IsSingleByte)
{
    EXPECT_EQ(Encoding::UTF8().IsSingleByte(), false);
    EXPECT_EQ(Encoding::Unicode().IsSingleByte(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode().IsSingleByte(), false);
    EXPECT_EQ(Encoding::UTF32().IsSingleByte(), false);
    EXPECT_EQ(Encoding::BigEndianUnicode32().IsSingleByte(), false);
    EXPECT_EQ(Encoding::ASCII().IsSingleByte(), true);
    EXPECT_EQ(Encoding::Latin1().IsSingleByte(), true);
}


TEST(ICUEncoding, CodePage)
{
    EXPECT_EQ(Encoding::UTF8().CodePage(), 65001);
    EXPECT_EQ(Encoding::Unicode().CodePage(), 1200);
    EXPECT_EQ(Encoding::BigEndianUnicode().CodePage(), 1201);
    EXPECT_EQ(Encoding::UTF32().CodePage(), 12000);
    EXPECT_EQ(Encoding::BigEndianUnicode32().CodePage(), 12001);
    EXPECT_EQ(Encoding::ASCII().CodePage(), 20127);
    EXPECT_EQ(Encoding::Latin1().CodePage(), 28591);
}

TEST(ICUEncoding, Clone)
{
    auto clonedEncoding = Encoding::UTF8().Clone();
    EXPECT_FALSE(clonedEncoding->IsReadOnly());
    EXPECT_TRUE(clonedEncoding->EncodingName().find(u"UTF-8") != std::string::npos);
    EXPECT_EQ(Encoding::UTF8(), *clonedEncoding);
    EXPECT_EQ(Encoding::UTF8().GetEncoderFallback(), clonedEncoding->GetEncoderFallback());
    EXPECT_EQ(Encoding::UTF8().GetDecoderFallback(), clonedEncoding->GetDecoderFallback());
}

TEST(ICUEncoding, GetByteCount)
{
    std::byte utf16Bytes[] = {(std::byte)0x48, (std::byte)0x00, (std::byte)0x40, (std::byte)0xC6, (std::byte)0x6C, (std::byte)0x00, (std::byte)0x6C, (std::byte)0x00};
    auto str = ReadOnlySpan(reinterpret_cast<const char16_t*>(utf16Bytes), sizeof(utf16Bytes) / 2);
    EXPECT_EQ(Encoding::UTF8().GetByteCount(str).value(), 6);
    EXPECT_EQ(Encoding::Unicode().GetByteCount(str).value(), 8);
    EXPECT_EQ(Encoding::Unicode().GetByteCount({}).value(), 0);
}

TEST(ICUEncoding, GetCharCount)
{
    EXPECT_EQ(Encoding::UTF8().GetCharCount({}), 0);
    EXPECT_EQ(Encoding::Unicode().GetCharCount({}), 0);

    std::byte utf8Bytes[] = {(std::byte)0x48, (std::byte)0xEC, (std::byte)0x99, (std::byte)0x80, (std::byte)0x6C, (std::byte)0x6C};
    EXPECT_EQ(Encoding::UTF8().GetCharCount(utf8Bytes).value(), 4);

    std::byte utf16Bytes[] = {(std::byte)0x48, (std::byte)0x00, (std::byte)0x40, (std::byte)0xC6, (std::byte)0x6C, (std::byte)0x00, (std::byte)0x6C, (std::byte)0x00};
    EXPECT_EQ(Encoding::Unicode().GetCharCount(utf16Bytes).value(), 4);
}

TEST(ICUEncoding, GetChars)
{
    auto chars = Encoding::Unicode().GetChars({});
    EXPECT_EQ(chars->size(), 0);

    std::byte utf8Bytes[] = {(std::byte)0x48, (std::byte)0xEC, (std::byte)0x99, (std::byte)0x80, (std::byte)0x6C, (std::byte)0x6C};
    chars = Encoding::UTF8().GetChars(utf8Bytes);
    EXPECT_EQ(chars->size(), 4);
    EXPECT_EQ((*chars)[0], u'H');
    EXPECT_EQ((*chars)[1], u'\xC640');
    EXPECT_EQ((*chars)[2], u'l');
    EXPECT_EQ((*chars)[3], u'l');

    std::byte utf16Bytes[] = {(std::byte)0x48, (std::byte)0x00, (std::byte)0x40, (std::byte)0xC6, (std::byte)0x6C, (std::byte)0x00, (std::byte)0x6C, (std::byte)0x00};
    chars = Encoding::Unicode().GetChars(utf16Bytes);
    EXPECT_EQ(chars->size(), 4);
    EXPECT_EQ((*chars)[0], u'H');
    EXPECT_EQ((*chars)[1], u'\xC640');
    EXPECT_EQ((*chars)[2], u'l');
    EXPECT_EQ((*chars)[3], u'l');

    auto lhsBytesArrayA = std::array<char16_t, 4>{};
    auto convertedByteCount = Encoding::UTF8().GetChars(utf8Bytes, lhsBytesArrayA);
    EXPECT_TRUE(convertedByteCount.has_value());
    EXPECT_EQ(*convertedByteCount, 4);
    EXPECT_EQ(lhsBytesArrayA[0], u'H');
    EXPECT_EQ(lhsBytesArrayA[1], u'\xC640');
    EXPECT_EQ(lhsBytesArrayA[2], u'l');
    EXPECT_EQ(lhsBytesArrayA[3], u'l');

    auto lhsBytesArrayB = std::array<char16_t, 3>{};
    convertedByteCount = Encoding::UTF8().GetChars(utf8Bytes, lhsBytesArrayB);
    EXPECT_FALSE(convertedByteCount.has_value());
}

TEST(ICUEncoding, GetBytes)
{
    std::byte utf16Bytes[] = {(std::byte)0x48, (std::byte)0x00, (std::byte)0x40, (std::byte)0xC6, (std::byte)0x6C, (std::byte)0x00, (std::byte)0x6C, (std::byte)0x00};
    std::byte utf8Bytes[] = {(std::byte)0x48, (std::byte)0xEC, (std::byte)0x99, (std::byte)0x80, (std::byte)0x6C, (std::byte)0x6C};
    auto utf16Str = ReadOnlySpan(reinterpret_cast<const char16_t*>(utf16Bytes), sizeof(utf16Bytes) / 2);

    auto lhsBytes = Encoding::UTF8().GetBytes(utf16Str);
    auto rhsBytes = ReadOnlySpan(utf8Bytes);
    EXPECT_EQ(lhsBytes->size(), 6);
    EXPECT_EQ((*lhsBytes)[0], rhsBytes[0]);
    EXPECT_EQ((*lhsBytes)[1], rhsBytes[1]);
    EXPECT_EQ((*lhsBytes)[2], rhsBytes[2]);
    EXPECT_EQ((*lhsBytes)[3], rhsBytes[3]);
    EXPECT_EQ((*lhsBytes)[4], rhsBytes[4]);
    EXPECT_EQ((*lhsBytes)[5], rhsBytes[5]);

    lhsBytes = Encoding::Unicode().GetBytes(utf16Str);
    rhsBytes = utf16Bytes;
    EXPECT_EQ(lhsBytes->size(), 8);
    EXPECT_EQ((*lhsBytes)[0], rhsBytes[0]);
    EXPECT_EQ((*lhsBytes)[1], rhsBytes[1]);
    EXPECT_EQ((*lhsBytes)[2], rhsBytes[2]);
    EXPECT_EQ((*lhsBytes)[3], rhsBytes[3]);
    EXPECT_EQ((*lhsBytes)[4], rhsBytes[4]);
    EXPECT_EQ((*lhsBytes)[5], rhsBytes[5]);
    EXPECT_EQ((*lhsBytes)[6], rhsBytes[6]);
    EXPECT_EQ((*lhsBytes)[7], rhsBytes[7]);

    lhsBytes = Encoding::Unicode().GetBytes({});
    EXPECT_EQ(lhsBytes->size(), 0);

    auto lhsBytesArrayA = std::array<std::byte, 6>{};
    auto convertedByteCount = Encoding::UTF8().GetBytes(utf16Str, lhsBytesArrayA);
    rhsBytes = utf8Bytes;
    EXPECT_TRUE(convertedByteCount.has_value());
    EXPECT_EQ(*convertedByteCount, 6);
    EXPECT_EQ(lhsBytesArrayA[0], rhsBytes[0]);
    EXPECT_EQ(lhsBytesArrayA[1], rhsBytes[1]);
    EXPECT_EQ(lhsBytesArrayA[2], rhsBytes[2]);
    EXPECT_EQ(lhsBytesArrayA[3], rhsBytes[3]);
    EXPECT_EQ(lhsBytesArrayA[4], rhsBytes[4]);
    EXPECT_EQ(lhsBytesArrayA[5], rhsBytes[5]);

    auto lhsBytesArrayB = std::array<std::byte, 5>{};
    convertedByteCount = Encoding::UTF8().GetBytes(utf16Str, lhsBytesArrayB);
    EXPECT_FALSE(convertedByteCount.has_value());
}

TEST(ICUEncoding, GetString)
{
    std::byte utf8Bytes[] = {(std::byte)0x48, (std::byte)0xEC, (std::byte)0x99, (std::byte)0x80, (std::byte)0x6C, (std::byte)0x6C};
    auto utf16Str = Encoding::UTF8().GetString(utf8Bytes);
    EXPECT_EQ(utf16Str->size(), 4);
    EXPECT_EQ((*utf16Str)[0], u'H');
    EXPECT_EQ((*utf16Str)[1], u'\xC640');
    EXPECT_EQ((*utf16Str)[2], u'l');
    EXPECT_EQ((*utf16Str)[3], u'l');

    utf16Str = Encoding::UTF8().GetString(utf8Bytes);
    EXPECT_EQ(utf16Str->size(), 4);
    EXPECT_EQ((*utf16Str)[0], u'H');
    EXPECT_EQ((*utf16Str)[1], u'\xC640');
    EXPECT_EQ((*utf16Str)[2], u'l');
    EXPECT_EQ((*utf16Str)[3], u'l');

    std::byte utf16Bytes[] = {(std::byte)0x48, (std::byte)0x00, (std::byte)0x40, (std::byte)0xC6, (std::byte)0x6C, (std::byte)0x00, (std::byte)0x6C, (std::byte)0x00};
    utf16Str = Encoding::Unicode().GetString(utf16Bytes);
    EXPECT_EQ(utf16Str->size(), 4);
    EXPECT_EQ((*utf16Str)[0], u'H');
    EXPECT_EQ((*utf16Str)[1], u'\xC640');
    EXPECT_EQ((*utf16Str)[2], u'l');
    EXPECT_EQ((*utf16Str)[3], u'l');

    utf16Str = Encoding::Unicode().GetString({});
    EXPECT_EQ(utf16Str->size(), 0);
}