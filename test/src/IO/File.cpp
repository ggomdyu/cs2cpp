#include <gtest/gtest.h>

#include "System/TimeZoneInfo.h"
#include "System/Random.h"
#include "System.IO/Directory.h"
#include "System.IO/Path.h"
#include "System.IO/File.h"

#include "TempFile.h"

using namespace CS2CPP_NAMESPACE_NAME;

TEST(File, AppendAllLines)
{
    Random r;
    std::u16string randomStr = Path::GetRandomFileName();
    std::array<std::u16string_view, 4> linesToWrite = {
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
    };

    // Test for default encoding
    TempFile file(Path::GetRandomFileName());
    EXPECT_TRUE(File::WriteAllLines(file.GetPath(), linesToWrite));
    EXPECT_TRUE(File::AppendAllLines(file.GetPath(), linesToWrite));
    auto readLines = File::ReadAllLines(file.GetPath());
    EXPECT_EQ(readLines->size(), linesToWrite.size() * 2);
    if (readLines->size() == linesToWrite.size() * 2)
    {
        for (size_t i = 0; i < readLines->size(); ++i)
            EXPECT_EQ(readLines->at(i), linesToWrite[i % linesToWrite.size()]);
    }

    // Test for default encoding
    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::WriteAllLines(file2.GetPath(), linesToWrite, Encoding::Unicode()));
    EXPECT_TRUE(File::AppendAllLines(file2.GetPath(), linesToWrite, Encoding::Unicode()));
    readLines = File::ReadAllLines(file2.GetPath(), Encoding::Unicode());
    EXPECT_EQ(readLines->size(), linesToWrite.size() * 2);
    if (readLines->size() == linesToWrite.size() * 2)
    {
        for (size_t i = 0; i < readLines->size(); ++i)
            EXPECT_EQ(readLines->at(i), linesToWrite[i % linesToWrite.size()]);
    }
}

TEST(File, AppendAllText)
{
    std::u16string randomStr = Path::GetRandomFileName();

    // Test for default encoding
    TempFile file(Path::GetRandomFileName());
    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::AppendAllText(file.GetPath(), randomStr.substr(0, 10)));
    EXPECT_TRUE(*File::ReadAllText(file.GetPath()) == randomStr.substr(0, 10));
    EXPECT_TRUE(File::WriteAllText(file2.GetPath(), randomStr));
    EXPECT_TRUE(File::AppendAllText(file2.GetPath(), randomStr.substr(0, 10)));
    EXPECT_TRUE(*File::ReadAllText(file2.GetPath()) == std::u16string(randomStr).append(randomStr.substr(0, 10)));

    // Test for UTF16-LE encoding
    TempFile file3(Path::GetRandomFileName());
    TempFile file4(Path::GetRandomFileName());
    EXPECT_TRUE(File::AppendAllText(file3.GetPath(), randomStr.substr(0, 10), Encoding::Unicode()));
    EXPECT_TRUE(*File::ReadAllText(file3.GetPath(), Encoding::Unicode()) == randomStr.substr(0, 10));
    EXPECT_TRUE(File::WriteAllText(file4.GetPath(), randomStr, Encoding::Unicode()));
    EXPECT_TRUE(File::AppendAllText(file4.GetPath(), randomStr.substr(0, 10), Encoding::Unicode()));
    EXPECT_TRUE(*File::ReadAllText(file4.GetPath(), Encoding::Unicode()) == std::u16string(randomStr).append(randomStr.substr(0, 10)));
}

TEST(File, AppendText)
{
    TempFile file;
    TempFile file2(Path::GetRandomFileName());

    EXPECT_TRUE(File::AppendText(file.GetPath()));
    EXPECT_TRUE(File::AppendText(file2.GetPath()));
}

TEST(File, Copy)
{
    TempFile file;
    TempFile file2(file.GetPath() + u"tmp");
    File::Copy(file.GetPath(), file2.GetPath());
    EXPECT_TRUE(File::Exists(file.GetPath()));
    EXPECT_TRUE(File::Exists(file2.GetPath()));
}

TEST(File, Create)
{
    TempFile file(Path::GetRandomFileName());
    EXPECT_FALSE(File::Exists(file.GetPath()));
    File::Create(file.GetPath());
    EXPECT_TRUE(File::Exists(file.GetPath()));
}

TEST(File, CreateText)
{
    TempFile file;
    EXPECT_TRUE(File::CreateText(file.GetPath()));

    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::CreateText(file2.GetPath()));
}

TEST(File, Delete)
{
    TempFile file;

    auto filename = Path::GetRandomFileName();
    filename[0] = 0x3042;
    auto directory = *Directory::CreateDirectory(filename);
    EXPECT_FALSE(File::Delete(directory.GetName()));
    EXPECT_TRUE(Directory::Delete(directory.ToString()));

    EXPECT_TRUE(File::Exists(file.GetPath()));
    EXPECT_TRUE(File::Delete(file.GetPath()));
    EXPECT_FALSE(File::Exists(file.GetPath()));
}

#if CS2CPP_PLATFORM_WINDOWS
TEST(File, Encrypt)
{
    TempFile file;

    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Encrypted);
    File::Encrypt(file.GetPath());
    EXPECT_TRUE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Encrypted);
    File::Decrypt(file.GetPath());
    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Encrypted);
}
#endif

TEST(File, Exists)
{
    auto directory = *Directory::CreateDirectory(Path::GetRandomFileName());
    EXPECT_FALSE(File::Exists(directory.GetName()));
    EXPECT_TRUE(Directory::Delete(directory.ToString()));

    TempFile file;
    EXPECT_TRUE(File::Exists(file.GetPath()));
    EXPECT_FALSE(File::Exists(file.GetPath() + u"tmp"));
}

TEST(File, Move)
{
    auto directory = *Directory::CreateDirectory(Path::GetRandomFileName());
    EXPECT_FALSE(File::Move(directory.GetName(), Path::GetRandomFileName()));
    EXPECT_TRUE(Directory::Delete(directory.ToString()));

    TempFile file;
    TempFile file2(file.GetPath() + u"tmp");
    EXPECT_TRUE(File::Exists(file.GetPath()));
    EXPECT_TRUE(File::Move(file.GetPath(), file2.GetPath()));
    EXPECT_FALSE(File::Exists(file.GetPath()));
    EXPECT_TRUE(File::Exists(file2.GetPath()));
}

TEST(File, Open)
{
    TempFile file;
    EXPECT_FALSE(File::Open(Path::GetRandomFileName(), FileMode::Open));
    EXPECT_TRUE(File::Open(file.GetPath(), FileMode::Open));
}

TEST(File, OpenRead)
{
    TempFile file;
    EXPECT_FALSE(File::OpenRead(Path::GetRandomFileName()));
    EXPECT_TRUE(File::OpenRead(file.GetPath()));
}

TEST(File, OpenText)
{
    TempFile file;

    EXPECT_FALSE(File::OpenText(Path::GetRandomFileName()));
    EXPECT_TRUE(File::OpenText(file.GetPath()));
}

TEST(File, OpenWrite)
{
    TempFile file;
    TempFile file2(Path::GetRandomFileName());

    EXPECT_TRUE(File::OpenWrite(file.GetPath()));
    EXPECT_TRUE(File::OpenWrite(file2.GetPath()));
}

TEST(File, ReadLines)
{
    TempFile file(Path::GetRandomFileName());

    Random r;
    std::u16string randomStr = Path::GetRandomFileName();
    std::array<std::u16string_view, 4> linesToWrite = {
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
    };
    EXPECT_FALSE(File::ReadAllLines(file.GetPath()));
    EXPECT_TRUE(File::WriteAllLines(file.GetPath(), linesToWrite));

    // Read all lines
    std::vector<std::u16string> readLines;
    EXPECT_TRUE(File::ReadLines(file.GetPath(), [&readLines](std::u16string&& line)
    {
        readLines.push_back(std::move(line));
        return true;
    }));
    EXPECT_EQ(readLines.size(), linesToWrite.size());
    for (size_t i = 0; i < linesToWrite.size(); ++i)
        EXPECT_EQ(readLines[i], linesToWrite[i]);

    // Read a single line
    readLines.clear();
    EXPECT_TRUE(File::ReadLines(file.GetPath(), [&readLines](std::u16string&& line)
    {
        readLines.push_back(std::move(line));
        return false;
    }));
    EXPECT_EQ(readLines.size(), 1);
    EXPECT_EQ(readLines[0], linesToWrite[0]);
}

TEST(File, SetAttributes)
{
    TempFile file;

    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Hidden);
    EXPECT_TRUE(File::SetAttributes(file.GetPath(), FileAttributes::Hidden));
    EXPECT_TRUE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Hidden);
    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::ReadOnly);
    EXPECT_TRUE(File::SetAttributes(file.GetPath(), FileAttributes::ReadOnly));
    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::Hidden);
    EXPECT_TRUE((size_t)*File::GetAttributes(file.GetPath()) & (size_t)FileAttributes::ReadOnly);
}

TEST(File, SetCreationTime)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    auto dtu2 = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Unspecified);
    File::SetCreationTime(file.GetPath(), dtl);
    EXPECT_EQ(File::GetCreationTime(file.GetPath()).value(), dtu);
    File::SetCreationTime(file.GetPath(), dtu);
    EXPECT_EQ(File::GetCreationTime(file.GetPath()).value(), dtu.AddHours(TimeZoneInfo::Local().BaseUtcOffset().Hours()));
    File::SetCreationTime(file.GetPath(), dtu2);
    EXPECT_EQ(File::GetCreationTime(file.GetPath()).value(), dtu);
}

TEST(File, SetLastAccessTimeUtc)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastAccessTimeUtc(file.GetPath(), dtu);
    EXPECT_EQ(File::GetLastAccessTimeUtc(file.GetPath()).value(), dtu);
    File::SetLastAccessTimeUtc(file.GetPath(), dtl);
    EXPECT_EQ(File::GetLastAccessTimeUtc(file.GetPath()).value(), dtu.AddHours(-TimeZoneInfo::Local().BaseUtcOffset().Hours()));
}

TEST(File, SetLastWriteTime)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastWriteTime(file.GetPath(), dtl);
    EXPECT_EQ(File::GetLastWriteTime(file.GetPath()).value(), dtu);
    File::SetLastWriteTime(file.GetPath(), dtu);
    EXPECT_EQ(File::GetLastWriteTime(file.GetPath()).value(), dtu.AddHours(TimeZoneInfo::Local().BaseUtcOffset().Hours()));
}

TEST(File, SetLastWriteTimeUtc)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastWriteTimeUtc(file.GetPath(), dtu);
    EXPECT_EQ(File::GetLastWriteTimeUtc(file.GetPath()).value(), dtu);
    File::SetLastWriteTimeUtc(file.GetPath(), dtl);
    EXPECT_EQ(File::GetLastWriteTimeUtc(file.GetPath()).value(), dtu.AddHours(-TimeZoneInfo::Local().BaseUtcOffset().Hours()));
}

TEST(File, WriteAllBytes)
{
    Random r;
    std::array<std::byte, 32> buffer{};

    for (size_t i = 0; i < 1; ++i)
    {
        std::generate(buffer.begin(), buffer.end(), [&]() { return (std::byte)r.Next(1, 256); });

        TempFile file(Path::GetRandomFileName());

        EXPECT_FALSE(File::ReadAllBytes(file.GetPath()));
        EXPECT_TRUE(File::WriteAllBytes(file.GetPath(), {buffer.data(), buffer.size()}));
        EXPECT_EQ(memcmp(File::ReadAllBytes(file.GetPath())->data(), buffer.data(), buffer.size()), 0);
    }
}

TEST(File, WriteAllLines)
{
    Random r;
    std::u16string randomStr = Path::GetRandomFileName();
    std::array<std::u16string_view, 4> linesToWrite = {
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
        std::u16string_view(randomStr.data(), static_cast<size_t>(r.Next(0, static_cast<int32_t>(randomStr.length() + 1)))),
    };

    // Test for default encoding
    TempFile file(Path::GetRandomFileName());
    EXPECT_FALSE(File::ReadAllLines(file.GetPath()));
    EXPECT_TRUE(File::WriteAllLines(file.GetPath(), linesToWrite));
    auto readLines = File::ReadAllLines(file.GetPath());
    EXPECT_TRUE(readLines);
    EXPECT_EQ(readLines->size(), linesToWrite.size());
    for (size_t i = 0; i < linesToWrite.size(); ++i)
        EXPECT_EQ(readLines->at(i), linesToWrite[i]);

    // Test for UTF-16LE Encoding
    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::WriteAllLines(file2.GetPath(), linesToWrite, Encoding::Unicode()));
    readLines = File::ReadAllLines(file2.GetPath(), Encoding::Unicode());
    EXPECT_TRUE(readLines);
    EXPECT_EQ(readLines->size(), linesToWrite.size());
    for (size_t i = 0; i < linesToWrite.size(); ++i)
        EXPECT_EQ(readLines->at(i), linesToWrite[i]);
}

TEST(File, WriteAllText)
{
    Random r;
    std::array<char16_t, 32> buffer{};

    for (size_t i = 0 ; i < 10; ++i)
    {
        std::generate(buffer.begin(), buffer.end() - 1, [&]() { return r.Next('a', 'z'); });
        buffer.back() = u'\0';

        TempFile file(Path::GetRandomFileName());

        EXPECT_FALSE(File::ReadAllText(file.GetPath()));
        EXPECT_TRUE(File::WriteAllText(file.GetPath(), {buffer.data(), buffer.size() - 1}));
        EXPECT_EQ(memcmp(File::ReadAllText(file.GetPath())->c_str(), buffer.data(), buffer.size()), 0);
    }
}