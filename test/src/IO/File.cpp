#include <System/Environment.h>
#include <System/IO/Directory.h>
#include <System/IO/File.h>
#include <System/Random.h>
#include <gtest/gtest.h>

using namespace tg;

namespace
{

class TempFile
{
public:
    explicit TempFile(std::u16string filePath);
    TempFile();
    ~TempFile();

public:
    [[nodiscard]] const std::u16string& GetFilePath() const;

private:
    std::u16string _filePath;
};

TempFile::TempFile(std::u16string filePath) :
    _filePath(std::move(filePath))
{
}

TempFile::TempFile() :
    TempFile([&]()
    {
        auto cd = Environment::GetCurrentDirectory();
        auto filePath = Path::Combine(cd, Path::GetRandomFileName());

        auto fs = FileStream::Create(filePath, FileMode::OpenOrCreate);
        for (int i = 0; i < 1024; ++i)
            fs->WriteByte(std::byte(i % 256));

        fs->Close();

        return filePath;
    } ())
{
}

TempFile::~TempFile()
{
    File::Delete(_filePath);
}

const std::u16string& TempFile::GetFilePath() const
{
    return _filePath;
}

}

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
    EXPECT_TRUE(File::WriteAllLines(file.GetFilePath(), linesToWrite));
    EXPECT_TRUE(File::AppendAllLines(file.GetFilePath(), linesToWrite));
    auto readLines = File::ReadAllLines(file.GetFilePath());
    EXPECT_EQ(readLines->size(), linesToWrite.size() * 2);
    if (readLines->size() == linesToWrite.size() * 2)
    {
        for (size_t i = 0; i < readLines->size(); ++i)
            EXPECT_EQ(readLines->at(i), linesToWrite[i % linesToWrite.size()]);
    }

    // Test for default encoding
    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::WriteAllLines(file2.GetFilePath(), linesToWrite, Encoding::Unicode()));
    EXPECT_TRUE(File::AppendAllLines(file2.GetFilePath(), linesToWrite, Encoding::Unicode()));
    readLines = File::ReadAllLines(file2.GetFilePath(), Encoding::Unicode());
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
    EXPECT_TRUE(File::AppendAllText(file.GetFilePath(), randomStr.substr(0, 10)));
    EXPECT_TRUE(*File::ReadAllText(file.GetFilePath()) == randomStr.substr(0, 10));
    EXPECT_TRUE(File::WriteAllText(file2.GetFilePath(), randomStr));
    EXPECT_TRUE(File::AppendAllText(file2.GetFilePath(), randomStr.substr(0, 10)));
    EXPECT_TRUE(*File::ReadAllText(file2.GetFilePath()) == std::u16string(randomStr).append(randomStr.substr(0, 10)));

    // Test for UTF16-LE encoding
    TempFile file3(Path::GetRandomFileName());
    TempFile file4(Path::GetRandomFileName());
    EXPECT_TRUE(File::AppendAllText(file3.GetFilePath(), randomStr.substr(0, 10), Encoding::Unicode()));
    EXPECT_TRUE(*File::ReadAllText(file3.GetFilePath(), Encoding::Unicode()) == randomStr.substr(0, 10));
    EXPECT_TRUE(File::WriteAllText(file4.GetFilePath(), randomStr, Encoding::Unicode()));
    EXPECT_TRUE(File::AppendAllText(file4.GetFilePath(), randomStr.substr(0, 10), Encoding::Unicode()));
    EXPECT_TRUE(*File::ReadAllText(file4.GetFilePath(), Encoding::Unicode()) == std::u16string(randomStr).append(randomStr.substr(0, 10)));
}

TEST(File, AppendText)
{
    TempFile file;
    TempFile file2(Path::GetRandomFileName());

    EXPECT_TRUE(File::AppendText(file.GetFilePath()));
    EXPECT_TRUE(File::AppendText(file2.GetFilePath()));
}

TEST(File, Copy)
{
    TempFile file;
    TempFile file2(file.GetFilePath() + u"tmp");

    File::Copy(file.GetFilePath(), file2.GetFilePath());
    EXPECT_TRUE(File::Exists(file.GetFilePath()));
    EXPECT_TRUE(File::Exists(file2.GetFilePath()));
}

TEST(File, Create)
{
    TempFile file(Path::GetRandomFileName());

    EXPECT_FALSE(File::Exists(file.GetFilePath()));
    File::Create(file.GetFilePath());
    EXPECT_TRUE(File::Exists(file.GetFilePath()));
}

TEST(File, CreateText)
{
    TempFile file;
    TempFile file2(Path::GetRandomFileName());

    EXPECT_TRUE(File::CreateText(file.GetFilePath()));
    EXPECT_TRUE(File::CreateText(file2.GetFilePath()));
}

TEST(File, Delete)
{
    TempFile file;

    auto directory = Directory::CreateDirectory(Path::GetRandomFileName());
    EXPECT_FALSE(File::Delete(directory.GetName()));
    Directory::Delete(directory.GetName());

    EXPECT_TRUE(File::Exists(file.GetFilePath()));
    File::Delete(file.GetFilePath());
    EXPECT_FALSE(File::Exists(file.GetFilePath()));
}

#if CS2CPP_PLATFORM_WINDOWS
TEST(File, Encrypt)
{
    TempFile file;

    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetFilePath()) & (size_t)FileAttributes::Encrypted);
    File::Encrypt(file.GetFilePath());
    EXPECT_TRUE((size_t)*File::GetAttributes(file.GetFilePath()) & (size_t)FileAttributes::Encrypted);
    File::Decrypt(file.GetFilePath());
    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetFilePath()) & (size_t)FileAttributes::Encrypted);
}
#endif

TEST(File, Exists)
{
    TempFile file;

    auto directory = Directory::CreateDirectory(Path::GetRandomFileName());
    EXPECT_FALSE(File::Exists(directory.GetName()));
    Directory::Delete(directory.GetName());

    EXPECT_TRUE(File::Exists(file.GetFilePath()));
    EXPECT_FALSE(File::Exists(file.GetFilePath() + u"tmp"));
}

TEST(File, Move)
{
    TempFile file;
    TempFile file2(file.GetFilePath() + u"tmp");

    auto directory = Directory::CreateDirectory(Path::GetRandomFileName());
    EXPECT_FALSE(File::Move(directory.GetName(), Path::GetRandomFileName()));
    Directory::Delete(directory.GetName());

    EXPECT_TRUE(File::Exists(file.GetFilePath()));
    EXPECT_TRUE(File::Move(file.GetFilePath(), file2.GetFilePath()));
    EXPECT_FALSE(File::Exists(file.GetFilePath()));
    EXPECT_TRUE(File::Exists(file2.GetFilePath()));
}

TEST(File, Open)
{
    TempFile file;

    EXPECT_FALSE(File::Open(Path::GetRandomFileName(), FileMode::Open));
    EXPECT_TRUE(File::Open(file.GetFilePath(), FileMode::Open));
}

TEST(File, OpenRead)
{
    TempFile file;

    EXPECT_FALSE(File::OpenRead(Path::GetRandomFileName()));
    EXPECT_TRUE(File::OpenRead(file.GetFilePath()));
}

TEST(File, OpenText)
{
    TempFile file;

    EXPECT_FALSE(File::OpenText(Path::GetRandomFileName()));
    EXPECT_TRUE(File::OpenText(file.GetFilePath()));
}

TEST(File, OpenWrite)
{
    TempFile file;
    TempFile file2(Path::GetRandomFileName());

    EXPECT_TRUE(File::OpenWrite(file.GetFilePath()));
    EXPECT_TRUE(File::OpenWrite(file2.GetFilePath()));
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
    EXPECT_FALSE(File::ReadAllLines(file.GetFilePath()));
    EXPECT_TRUE(File::WriteAllLines(file.GetFilePath(), linesToWrite));

    // Read all lines
    std::vector<std::u16string> readLines;
    EXPECT_TRUE(File::ReadLines(file.GetFilePath(), [&readLines](std::u16string&& line)
    {
        readLines.push_back(std::move(line));
        return true;
    }));
    EXPECT_EQ(readLines.size(), linesToWrite.size());
    for (size_t i = 0; i < linesToWrite.size(); ++i)
        EXPECT_EQ(readLines[i], linesToWrite[i]);

    // Read a single line
    readLines.clear();
    EXPECT_TRUE(File::ReadLines(file.GetFilePath(), [&readLines](std::u16string&& line)
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

    EXPECT_FALSE((size_t)*File::GetAttributes(file.GetFilePath()) & (size_t)FileAttributes::Hidden);
    File::SetAttributes(file.GetFilePath(), FileAttributes::Hidden);
    EXPECT_TRUE((size_t)*File::GetAttributes(file.GetFilePath()) & (size_t)FileAttributes::Hidden);
}

TEST(File, SetCreationTime)
{
    TempFile file;
    
    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetCreationTime(file.GetFilePath(), dtl);
    EXPECT_EQ(File::GetCreationTime(file.GetFilePath()).value(), dtu);
    File::SetCreationTime(file.GetFilePath(), dtu);
    EXPECT_EQ(File::GetCreationTime(file.GetFilePath()).value(), dtu.AddHours(9.0));
}

TEST(File, SetCreationTimeUtc)
{
    TempFile file;
    
    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetCreationTimeUtc(file.GetFilePath(), dtu);
    EXPECT_EQ(File::GetCreationTimeUtc(file.GetFilePath()).value(), dtu);
    File::SetCreationTimeUtc(file.GetFilePath(), dtl);
    EXPECT_EQ(File::GetCreationTimeUtc(file.GetFilePath()).value(), dtu.AddHours(-9.0));
}

TEST(File, SetLastAccessTimeUtc)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastAccessTimeUtc(file.GetFilePath(), dtu);
    EXPECT_EQ(File::GetLastAccessTimeUtc(file.GetFilePath()).value(), dtu);
    File::SetLastAccessTimeUtc(file.GetFilePath(), dtl);
    EXPECT_EQ(File::GetLastAccessTimeUtc(file.GetFilePath()).value(), dtu.AddHours(-9.0));
}

TEST(File, SetLastWriteTime)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastWriteTime(file.GetFilePath(), dtl);
    EXPECT_EQ(File::GetLastWriteTime(file.GetFilePath()).value(), dtu);
    File::SetLastWriteTime(file.GetFilePath(), dtu);
    EXPECT_EQ(File::GetLastWriteTime(file.GetFilePath()).value(), dtu.AddHours(9.0));
}

TEST(File, SetLastWriteTimeUtc)
{
    TempFile file;

    auto dtl = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Local);
    auto dtu = DateTime(2001, 10, 12, 4, 2, 10, DateTimeKind::Utc);
    File::SetLastWriteTimeUtc(file.GetFilePath(), dtu);
    EXPECT_EQ(File::GetLastWriteTimeUtc(file.GetFilePath()).value(), dtu);
    File::SetLastWriteTimeUtc(file.GetFilePath(), dtl);
    EXPECT_EQ(File::GetLastWriteTimeUtc(file.GetFilePath()).value(), dtu.AddHours(-9.0));
}

TEST(File, WriteAllBytes)
{
    Random r;
    std::array<std::byte, 32> buffer{};

    for (size_t i = 0; i < 1; ++i)
    {
        std::generate(buffer.begin(), buffer.end(), [&]() { return (std::byte)r.Next(1, 256); });

        TempFile file(Path::GetRandomFileName());

        EXPECT_FALSE(File::ReadAllBytes(file.GetFilePath()));
        EXPECT_TRUE(File::WriteAllBytes(file.GetFilePath(), {buffer.data(), buffer.size()}));
        EXPECT_EQ(memcmp(File::ReadAllBytes(file.GetFilePath())->data(), buffer.data(), buffer.size()), 0);
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
    EXPECT_FALSE(File::ReadAllLines(file.GetFilePath()));
    EXPECT_TRUE(File::WriteAllLines(file.GetFilePath(), linesToWrite));
    auto readLines = File::ReadAllLines(file.GetFilePath());
    EXPECT_TRUE(readLines);
    EXPECT_EQ(readLines->size(), linesToWrite.size());
    for (size_t i = 0; i < linesToWrite.size(); ++i)
        EXPECT_EQ(readLines->at(i), linesToWrite[i]);

    // Test for UTF-16LE Encoding
    TempFile file2(Path::GetRandomFileName());
    EXPECT_TRUE(File::WriteAllLines(file2.GetFilePath(), linesToWrite, Encoding::Unicode()));
    readLines = File::ReadAllLines(file2.GetFilePath(), Encoding::Unicode());
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

        EXPECT_FALSE(File::ReadAllText(file.GetFilePath()));
        EXPECT_TRUE(File::WriteAllText(file.GetFilePath(), {buffer.data(), buffer.size() - 1}));
        EXPECT_EQ(memcmp(File::ReadAllText(file.GetFilePath())->c_str(), buffer.data(), buffer.size()), 0);
    }
}