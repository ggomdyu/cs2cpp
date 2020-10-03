#include <array>
#include <System.Text/OSEncoding.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

#if CS2CPP_PLATFORM_WINDOWS
TEST(OSEncoding, Convert)
{
    OSEncoding e(949);
    auto bytes = std::array<std::byte, 5>{(std::byte)0xBE, (std::byte)0xC8, (std::byte)0xB3, (std::byte)0xE7, (std::byte)0x61};
    auto convertedUtf16Bytes = Encoding::Convert(e, Encoding::Unicode(), bytes);
    EXPECT_EQ((*convertedUtf16Bytes)[0], (std::byte)0x48);
    EXPECT_EQ((*convertedUtf16Bytes)[1], (std::byte)0xC5);
    EXPECT_EQ((*convertedUtf16Bytes)[2], (std::byte)0x55);
    EXPECT_EQ((*convertedUtf16Bytes)[3], (std::byte)0xB1);
    EXPECT_EQ((*convertedUtf16Bytes)[4], (std::byte)0x61);
    EXPECT_EQ((*convertedUtf16Bytes)[5], (std::byte)0x00);
    auto convertedCP949Bytes = Encoding::Convert(Encoding::Unicode(), e, *convertedUtf16Bytes);
    EXPECT_EQ((*convertedCP949Bytes)[0], bytes[0]);
    EXPECT_EQ((*convertedCP949Bytes)[1], bytes[1]);
    EXPECT_EQ((*convertedCP949Bytes)[2], bytes[2]);
    EXPECT_EQ((*convertedCP949Bytes)[3], bytes[3]);
    EXPECT_EQ((*convertedCP949Bytes)[4], bytes[4]);
}

TEST(OSEncoding, Preamble)
{
    EXPECT_TRUE(OSEncoding(949).Preamble().IsEmpty());
}

TEST(OSEncoding, BodyName)
{
    EXPECT_TRUE(OSEncoding(949).BodyName().empty());
}

TEST(OSEncoding, EncodingName)
{
    EXPECT_EQ(OSEncoding(949).EncodingName(), u"Codepage - 949");
}

TEST(OSEncoding, HeaderName)
{
    EXPECT_TRUE(OSEncoding(949).HeaderName().empty());
}

TEST(OSEncoding, WebName)
{
    EXPECT_EQ(OSEncoding(949).WebName(), u"Codepage - 949");
}

TEST(OSEncoding, WindowsCodePage)
{
    EXPECT_EQ(OSEncoding(949).WindowsCodePage(), 0);
}

TEST(OSEncoding, IsBrowserDisplay)
{
    EXPECT_FALSE(OSEncoding(949).IsBrowserDisplay());
}

TEST(OSEncoding, IsBrowserSave)
{
    EXPECT_FALSE(OSEncoding(949).IsBrowserSave());
}

TEST(OSEncoding, IsMailNewsDisplay)
{
    EXPECT_FALSE(OSEncoding(949).IsMailNewsDisplay());
}

TEST(OSEncoding, IsMailNewsSave)
{
    EXPECT_FALSE(OSEncoding(949).IsMailNewsSave());
}

TEST(OSEncoding, IsSingleByte)
{
    EXPECT_FALSE(OSEncoding(949).IsSingleByte());
}

TEST(OSEncoding, CodePage)
{
    EXPECT_EQ(OSEncoding(949).CodePage(), 949);
}

TEST(OSEncoding, Clone)
{
    OSEncoding e(949);
    auto cloned = e.Clone();
    EXPECT_EQ(e.CodePage(), cloned->CodePage());
}

TEST(OSEncoding, GetByteCount)
{
    OSEncoding e(949);
    auto utf16Bytes = std::array<std::byte, 6>{(std::byte)0x48, (std::byte)0xC5, (std::byte)0x55, (std::byte)0xB1, (std::byte)0x61, (std::byte)0x00};
    EXPECT_EQ(e.GetByteCount({(const char16_t*)utf16Bytes.data(), 3}), 5);
}

TEST(OSEncoding, GetCharCount)
{
    OSEncoding e(949);
    auto bytes = std::array<std::byte, 5>{(std::byte)0xBE, (std::byte)0xC8, (std::byte)0xB3, (std::byte)0xE7, (std::byte)0x61};
    EXPECT_EQ(e.GetCharCount(bytes), 3);
}

TEST(OSEncoding, GetChars)
{
    OSEncoding e(949);
    auto bytes = std::array<std::byte, 5>{(std::byte)0xBE, (std::byte)0xC8, (std::byte)0xB3, (std::byte)0xE7, (std::byte)0x61};
    auto chars = e.GetChars(bytes);
    EXPECT_EQ(chars->size(), 3);
    EXPECT_EQ(((std::byte*)chars->data())[0], (std::byte)0x48);
    EXPECT_EQ(((std::byte*)chars->data())[1], (std::byte)0xC5);
    EXPECT_EQ(((std::byte*)chars->data())[2], (std::byte)0x55);
    EXPECT_EQ(((std::byte*)chars->data())[3], (std::byte)0xB1);
    EXPECT_EQ(((std::byte*)chars->data())[4], (std::byte)0x61);
    EXPECT_EQ(((std::byte*)chars->data())[5], (std::byte)0x00);
    auto buffer = std::array<char16_t, 3>{};
    EXPECT_EQ(e.GetChars(bytes, buffer), 3);
    EXPECT_EQ(((std::byte*)buffer.data())[0], (std::byte)0x48);
    EXPECT_EQ(((std::byte*)buffer.data())[1], (std::byte)0xC5);
    EXPECT_EQ(((std::byte*)buffer.data())[2], (std::byte)0x55);
    EXPECT_EQ(((std::byte*)buffer.data())[3], (std::byte)0xB1);
    EXPECT_EQ(((std::byte*)buffer.data())[4], (std::byte)0x61);
    EXPECT_EQ(((std::byte*)buffer.data())[5], (std::byte)0x00);
}

TEST(OSEncoding, GetBytes)
{
    OSEncoding e(949);
    auto utf16Bytes = std::array<std::byte, 6>{(std::byte)0x48, (std::byte)0xC5, (std::byte)0x55, (std::byte)0xB1, (std::byte)0x61, (std::byte)0x00};
    auto bytes = e.GetBytes({(const char16_t*)utf16Bytes.data(), 3});
    EXPECT_TRUE(bytes.has_value());
    EXPECT_EQ((*bytes)[0], (std::byte)0xBE);
    EXPECT_EQ((*bytes)[1], (std::byte)0xC8);
    EXPECT_EQ((*bytes)[2], (std::byte)0xB3);
    EXPECT_EQ((*bytes)[3], (std::byte)0xE7);
    EXPECT_EQ((*bytes)[4], (std::byte)0x61);
    EXPECT_EQ(bytes->size(), 5);
    auto buffer = std::array<std::byte, 5>{};
    EXPECT_EQ(e.GetBytes({(const char16_t*)utf16Bytes.data(), 3}, buffer), 5);
    EXPECT_EQ(buffer[0], (std::byte)0xBE);
    EXPECT_EQ(buffer[1], (std::byte)0xC8);
    EXPECT_EQ(buffer[2], (std::byte)0xB3);
    EXPECT_EQ(buffer[3], (std::byte)0xE7);
    EXPECT_EQ(buffer[4], (std::byte)0x61);
}

TEST(OSEncoding, GetString)
{
    OSEncoding e(949);
    auto bytes = std::array<std::byte, 5>{(std::byte)0xBE, (std::byte)0xC8, (std::byte)0xB3, (std::byte)0xE7, (std::byte)0x61};
    auto str = e.GetString(bytes);
    EXPECT_EQ((*str)[0], u'\xC548');
    EXPECT_EQ((*str)[1], u'\xB155');
    EXPECT_EQ((*str)[2], u'a');
    EXPECT_EQ(str->size(), 3);
}
#endif
