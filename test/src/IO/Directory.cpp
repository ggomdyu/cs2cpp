#include <gtest/gtest.h>

#include "System/Environment.h"
#include "System/Random.h"
#include "System.IO/Directory.h"
#include "System.IO/File.h"
#include "System.IO/Path.h"

#include "TempDirectory.h"

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Directory, CreateDirectory)
{
    auto path = Path::GetRandomFileName();

    EXPECT_FALSE(Directory::Exists(path));
    Directory::CreateDirectory(path);
    EXPECT_TRUE(Directory::Exists(path));
    Directory::Delete(path);
}

TEST(Directory, Delete)
{
    // Delete a empty directory
    TempDirectory directory;
    EXPECT_TRUE(Directory::Exists(directory.GetPath()));
    EXPECT_TRUE(Directory::Delete(directory.GetPath()));
    EXPECT_FALSE(Directory::Exists(directory.GetPath()));
    auto filename1 = Path::GetRandomFileName();
    auto filename2 = Path::GetRandomFileName();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(filename1, filename2))->ToString());
    EXPECT_TRUE(Directory::Exists(directory2.GetPath()));
    EXPECT_TRUE(Directory::Delete(directory2.GetPath(), true));
    EXPECT_FALSE(Directory::Exists(directory2.GetPath()));
    EXPECT_FALSE(Directory::Exists(directory2.GetPath()));
    EXPECT_TRUE(Directory::Exists(filename1));
    EXPECT_TRUE(Directory::Delete(filename1));
    EXPECT_FALSE(Directory::Exists(filename1));

    // 1 depth directory remove
    TempDirectory directory3;
    auto file = directory3.AddTempFile();
    EXPECT_TRUE(File::Exists(file));
    EXPECT_FALSE(Directory::Delete(file, false));
    EXPECT_TRUE(Directory::Exists(directory3.GetPath()));
    EXPECT_TRUE(Directory::Delete(directory3.GetPath(), true));
    EXPECT_FALSE(Directory::Exists(directory3.GetPath()));

    // 2 depth directory remove
    TempDirectory directory4;
    directory4.AddTempFile();
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    directory5.AddTempFile();
    directory5.AddTempFile();
    EXPECT_TRUE(Directory::Exists(directory4.GetPath()));
    EXPECT_TRUE(Directory::Exists(directory5.GetPath()));
    EXPECT_FALSE(Directory::Delete(directory4.GetPath()));
    EXPECT_TRUE(Directory::Delete(directory4.GetPath(), true));
}

TEST(Directory, Move)
{
    TempDirectory directory;
    TempDirectory directory2(Path::GetRandomFileName());

    EXPECT_FALSE(Directory::Exists(directory2.GetPath()));
    Directory::Move(directory.GetPath(), directory2.GetPath());
    EXPECT_TRUE(Directory::Exists(directory2.GetPath()));
    EXPECT_FALSE(Directory::Exists(directory.GetPath()));
}

TEST(Directory, SetCurrentDirectory)
{
    TempDirectory directory;

    auto cd = Directory::GetCurrentDirectory();
    EXPECT_FALSE(cd.back() == 0);
    EXPECT_TRUE(Directory::Exists(cd));
    EXPECT_TRUE(Directory::SetCurrentDirectory(directory.GetPath()));
    EXPECT_EQ(Directory::GetCurrentDirectory(), directory.GetPath());

    // Roll back the current directory
    Directory::SetCurrentDirectory(cd);
}

TEST(Directory, GetParent)
{
    TempDirectory directory;

    auto cd = Directory::GetCurrentDirectory();
    auto root = Path::GetPathRoot(cd);

    EXPECT_FALSE(Directory::GetParent(u"").has_value());
    EXPECT_EQ(Directory::GetParent(u"/Users/Desktop")->ToString(), Path::Combine(root, u"Users"));
    EXPECT_EQ(Directory::GetParent(u"/Users")->ToString(), root);
    EXPECT_EQ(Directory::GetParent(u"Users/Desktop")->ToString(), Path::Combine(cd, u"Users"));
    EXPECT_EQ(Directory::GetParent(u"Users")->ToString(), cd);
}

TEST(Directory, GetDirectories)
{
    TempDirectory directory;
    directory.AddTempFile();
    directory.AddTempFile();
    directory.AddTempFile();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    directory2.AddTempFile();
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), u"abc2"))->ToString());
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    directory5.AddTempFile();

    auto directories = Directory::GetDirectories(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories.size(), 1);
    if (directories.size() == 4)
    {
        EXPECT_EQ(directories[0], directory2.GetPath());
    }

    auto directories2 = Directory::GetDirectories(directory.GetPath(), u"*", SearchOption::AllDirectories);
    EXPECT_EQ(directories2.size(), 4);

    auto directories3 = Directory::GetDirectories(directory.GetPath(), u"*abc*", SearchOption::AllDirectories);
    EXPECT_EQ(directories3.size(), 1);
    if (directories3.size() == 1)
    {
        EXPECT_EQ(directories3[0], directory4.GetPath());
    }
}

TEST(Directory, GetFiles)
{
    TempDirectory directory;
    auto file = directory.AddTempFile(u"png");
    auto file2 = directory.AddTempFile();
    auto file3 = directory.AddTempFile(u"png");
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file4 = directory2.AddTempFile(u"png");
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file5 = directory4.AddTempFile(u"png");
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file6 = directory5.AddTempFile();

    auto directories = Directory::GetFiles(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories.size(), 3);
    if (directories.size() == 3)
    {
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file) != directories.end());
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file2) != directories.end());
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file3) != directories.end());
    }

    auto directories2 = Directory::GetFiles(directory.GetPath(), u"*.png", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories2.size(), 2);
    if (directories2.size() == 2)
    {
        EXPECT_TRUE(std::find(directories2.begin(), directories2.end(), file) != directories2.end());
        EXPECT_TRUE(std::find(directories2.begin(), directories2.end(), file3) != directories2.end());
    }

    auto directories3 = Directory::GetFiles(directory.GetPath(), u"*", SearchOption::AllDirectories);
    EXPECT_EQ(directories3.size(), 9);
    if (directories3.size() == 9)
    {
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file2) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file3) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file6) != directories3.end());
    }

    auto directories4 = Directory::GetFiles(directory.GetPath(), u"*.png", SearchOption::AllDirectories);
    EXPECT_EQ(directories4.size(), 4);
    if (directories4.size() == 4)
    {
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file3) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file4) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file5) != directories4.end());
    }
}

TEST(Directory, GetFileSystemEntries)
{
    TempDirectory directory;
    auto file = directory.AddTempFile();
    auto file2 = directory.AddTempFile(u"png");
    auto file3 = directory.AddTempFile();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    directory2.AddTempFile();
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file4 = directory5.AddTempFile(u"png");

    auto directories = Directory::GetFileSystemEntries(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories.size(), 4);
    if (directories.size() == 4)
    {
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), directory2.GetPath()) != directories.end());
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file) != directories.end());
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file2) != directories.end());
        EXPECT_TRUE(std::find(directories.begin(), directories.end(), file3) != directories.end());
    }

    auto directories2 = Directory::GetFileSystemEntries(directory.GetPath(), u"*.png", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories2.size(), 1);
    if (directories2.size() == 1)
        EXPECT_TRUE(std::find(directories2.begin(), directories2.end(), file2) != directories2.end());

    auto directories3 = Directory::GetFileSystemEntries(directory.GetPath(), u"*.*", SearchOption::TopDirectoryOnly);
    EXPECT_EQ(directories3.size(), 4);
    if (directories3.size() == 4)
    {
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), directory2.GetPath()) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file2) != directories3.end());
        EXPECT_TRUE(std::find(directories3.begin(), directories3.end(), file3) != directories3.end());
    }

    auto directories4 = Directory::GetFileSystemEntries(directory.GetPath(), u"*", SearchOption::AllDirectories);
    EXPECT_EQ(directories4.size(), 13);
    if (directories4.size() == 13)
    {
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), directory2.GetPath()) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), directory3.GetPath()) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), directory4.GetPath()) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), directory5.GetPath()) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file2) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file3) != directories4.end());
        EXPECT_TRUE(std::find(directories4.begin(), directories4.end(), file4) != directories4.end());
    }

    auto directories5 = Directory::GetFileSystemEntries(directory.GetPath(), u"*.*", SearchOption::AllDirectories);
    EXPECT_EQ(directories5.size(), 13);
    if (directories5.size() == 13)
    {
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), directory2.GetPath()) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), directory3.GetPath()) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), directory4.GetPath()) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), directory5.GetPath()) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), file) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), file2) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), file3) != directories5.end());
        EXPECT_TRUE(std::find(directories5.begin(), directories5.end(), file4) != directories5.end());
    }

    auto directories6 = Directory::GetFileSystemEntries(directory.GetPath(), u"*.png", SearchOption::AllDirectories);
    EXPECT_EQ(directories6.size(), 2);
    if (directories6.size() == 2)
    {
        EXPECT_TRUE(std::find(directories6.begin(), directories6.end(), file2) != directories6.end());
        EXPECT_TRUE(std::find(directories6.begin(), directories6.end(), file4) != directories6.end());
    }
}

TEST(Directory, EnumerateDirectories)
{
    TempDirectory directory;
    auto file = directory.AddTempFile();
    auto file2 = directory.AddTempFile();
    auto file3 = directory.AddTempFile();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    directory2.AddTempFile();
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file4 = directory5.AddTempFile();

    auto it = Directory::EnumerateDirectories(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    std::vector<std::u16string> ret(begin(it), end(it));
    EXPECT_EQ(ret.size(), 1);

    it = Directory::EnumerateDirectories(directory.GetPath(), u"*", SearchOption::AllDirectories);
    std::vector<std::u16string> ret2(begin(it), end(it));
    EXPECT_EQ(ret2.size(), 4);
}

TEST(Directory, EnumerateFiles)
{
    TempDirectory directory;
    auto file = directory.AddTempFile();
    auto file2 = directory.AddTempFile();
    auto file3 = directory.AddTempFile();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    directory2.AddTempFile();
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file4 = directory5.AddTempFile();

    auto it = Directory::EnumerateFiles(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    std::vector<std::u16string> ret{begin(it), end(it)};
    EXPECT_EQ(ret.size(), 3);

    it = Directory::EnumerateFiles(directory.GetPath(), u"*", SearchOption::AllDirectories);
    std::vector<std::u16string> ret2{begin(it), end(it)};
    EXPECT_EQ(ret2.size(), 9);
}

TEST(Directory, EnumerateFileSystemEntries)
{
    TempDirectory directory;
    auto file = directory.AddTempFile();
    auto file2 = directory.AddTempFile();
    auto file3 = directory.AddTempFile();
    TempDirectory directory2(Directory::CreateDirectory(Path::Combine(directory.GetPath(), Path::GetRandomFileName()))->ToString());
    directory2.AddTempFile();
    directory2.AddTempFile();
    TempDirectory directory3(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory3.AddTempFile();
    TempDirectory directory4(Directory::CreateDirectory(Path::Combine(directory2.GetPath(), Path::GetRandomFileName()))->ToString());
    directory4.AddTempFile();
    directory4.AddTempFile();
    TempDirectory directory5(Directory::CreateDirectory(Path::Combine(directory4.GetPath(), Path::GetRandomFileName()))->ToString());
    auto file4 = directory5.AddTempFile();

    auto it = Directory::EnumerateFileSystemEntries(directory.GetPath(), u"*", SearchOption::TopDirectoryOnly);
    std::vector<std::u16string> ret{begin(it), end(it)};
    EXPECT_EQ(ret.size(), 4);

    it = Directory::EnumerateFileSystemEntries(directory.GetPath(), u"*", SearchOption::AllDirectories);
    std::vector<std::u16string> ret2{begin(it), end(it)};
    EXPECT_EQ(ret2.size(), 13);
}
