#include <gtest/gtest.h>

#include "System.IO/Directory.h"
#include "System.IO/Path.h"

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Path, Combine)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_EQ(Path::Combine(u"dec", u"col"), u"dec\\col");
    EXPECT_EQ(Path::Combine(u"dec/", u"col"), u"dec/col");
    EXPECT_EQ(Path::Combine(u"dec", u"col/"), u"dec\\col/");
    EXPECT_EQ(Path::Combine(u"dec/", u"col/.png"), u"dec/col/.png");
    EXPECT_EQ(Path::Combine(u"C:/col/2.png", u"C:/col/.png"), u"C:/col/.png");
    EXPECT_EQ(Path::Combine(u"col/2.png", u""), u"col/2.png");
    EXPECT_EQ(Path::Combine(u"", u"col/2.png"), u"col/2.png");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"ibs"), u"/User/Sys\\ibs");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"/ibs"), u"/ibs");
    EXPECT_EQ(Path::Combine(u"/User/Sys/", u"/ibs"), u"/ibs");
    EXPECT_EQ(Path::Combine(u"", u""), u"");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"C:/ibs"), u"C:/ibs");
    EXPECT_EQ(Path::Combine(u"C:/User/Sys", u"C:/ibs"), u"C:/ibs");
#else
    EXPECT_EQ(Path::Combine(u"dec", u"col"), u"dec/col");
    EXPECT_EQ(Path::Combine(u"dec/", u"col"), u"dec/col");
    EXPECT_EQ(Path::Combine(u"dec", u"col/"), u"dec/col/");
    EXPECT_EQ(Path::Combine(u"dec/", u"col/.png"), u"dec/col/.png");
    EXPECT_EQ(Path::Combine(u"col/2.png", u""), u"col/2.png");
    EXPECT_EQ(Path::Combine(u"", u"col/2.png"), u"col/2.png");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"ibs"), u"/User/Sys/ibs");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"/ibs"), u"/ibs");
    EXPECT_EQ(Path::Combine(u"/User/Sys/", u"/ibs"), u"/ibs");
    EXPECT_EQ(Path::Combine(u"", u""), u"");
    EXPECT_EQ(Path::Combine(u"C:/col/2.png", u"C:/col/.png"), u"C:/col/2.png/C:/col/.png");
    EXPECT_EQ(Path::Combine(u"/User/Sys", u"C:/ibs"), u"/User/Sys/C:/ibs");
    EXPECT_EQ(Path::Combine(u"C:/User/Sys", u"C:/ibs"), u"C:/User/Sys/C:/ibs");
#endif
}

TEST(Path, IsPathRooted)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_TRUE(Path::IsPathRooted(u"C:\\Temp"));
    EXPECT_TRUE(Path::IsPathRooted(u"C:/Temp"));
#else
    EXPECT_FALSE(Path::IsPathRooted(u"C:\\Temp"));
    EXPECT_FALSE(Path::IsPathRooted(u"C:/Temp"));
#endif
    EXPECT_TRUE(Path::IsPathRooted(u"/User/Temp"));
    EXPECT_FALSE(Path::IsPathRooted(u"User/Temp"));
    EXPECT_FALSE(Path::IsPathRooted(u"User"));
}

TEST(Path, GetExtension)
{
    EXPECT_EQ(Path::GetExtension(u"/users/desktop/1.png"), u".png");
    EXPECT_EQ(Path::GetExtension(u"/users/desktop/1"), u"");
    EXPECT_EQ(Path::GetExtension(u"1.png"), u".png");
    EXPECT_EQ(Path::GetExtension(u"1.1.png"), u".png");
    EXPECT_EQ(Path::GetExtension(u"1"), u"");
    EXPECT_EQ(Path::GetExtension(u""), u"");
}

TEST(Path, GetFileName)
{
    EXPECT_EQ(Path::GetFileName(u"/users/desktop/1.png"), u"1.png");
    EXPECT_EQ(Path::GetFileName(u"/users/desktop/1.1.png"), u"1.1.png");
    EXPECT_EQ(Path::GetFileName(u""), u"");
}

TEST(Path, GetFileNameWithoutExtension)
{
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u""), u"");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/users/desktop/123"), u"123");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"123123"), u"123123");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"1"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"1.png"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/users/desktop/1"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/users/desktop/1.png"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/users/desktop/1.1.png"), u"1.1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/123123.png"), u"123123");
}

TEST(Path, GetDirectoryName)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_EQ(Path::GetDirectoryName(u"C:/1.png"), u"C:\\");
    EXPECT_EQ(Path::GetDirectoryName(u"C:\\1.png"), u"C:\\");
    EXPECT_EQ(Path::GetDirectoryName(u"C:/dir/1.png"), u"C:\\dir");
    EXPECT_EQ(Path::GetDirectoryName(u"C:\\dir/1.png"), u"C:\\dir");
    EXPECT_EQ(Path::GetDirectoryName(u"/users/desktop/1.png"), u"\\users\\desktop");
    EXPECT_EQ(Path::GetDirectoryName(u"/users\\desktop\\1.png"), u"\\users\\desktop");
    EXPECT_EQ(Path::GetDirectoryName(u"abc.png"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"abc"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u""), u"");
#else
    EXPECT_EQ(Path::GetDirectoryName(u"C:/1.png"), u"C:");
    EXPECT_EQ(Path::GetDirectoryName(u"C:\\1.png"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"C:/dir/1.png"), u"C:/dir");
    EXPECT_EQ(Path::GetDirectoryName(u"C:\\dir/1.png"), u"C:\\dir");
    EXPECT_EQ(Path::GetDirectoryName(u"/users/desktop/1.png"), u"/users/desktop");
    EXPECT_EQ(Path::GetDirectoryName(u"/users\\desktop\\1.png"), u"/");
    EXPECT_EQ(Path::GetDirectoryName(u"abc.png"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"abc"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u""), u"");
#endif
}

TEST(Path, HasExtension)
{
    EXPECT_TRUE(Path::HasExtension(u"/users/desktop/1.png"));
    EXPECT_FALSE(Path::HasExtension(u"/users/desktop/1"));
    EXPECT_FALSE(Path::HasExtension(u"/users/desktop/"));
    EXPECT_FALSE(Path::HasExtension(u"/users/desktop"));
    EXPECT_TRUE(Path::HasExtension(u"1.png"));
    EXPECT_TRUE(Path::HasExtension(u".png"));
    EXPECT_FALSE(Path::HasExtension(u"png"));
    EXPECT_FALSE(Path::HasExtension(u""));
}

TEST(Path, ChangeExtension)
{
    EXPECT_EQ(Path::ChangeExtension(u"/users/desktop/1.png", u"ext"), u"/users/desktop/1.ext");
    EXPECT_EQ(Path::ChangeExtension(u".png", u"ext"), u".ext");
    EXPECT_EQ(Path::ChangeExtension(u"/users/desktop", u"ext"), u"/users/desktop.ext");
    EXPECT_EQ(Path::ChangeExtension(u"/users/desktop/", u"ext"), u"/users/desktop/.ext");
    EXPECT_EQ(Path::ChangeExtension(u"1.png", u"ext"), u"1.ext");
    EXPECT_EQ(Path::ChangeExtension(u"png", u"ext"), u"png.ext");
    EXPECT_EQ(Path::ChangeExtension(u"", u"ext"), u"");
    EXPECT_EQ(Path::ChangeExtension(u"", u"..ext"), u"");
    EXPECT_EQ(Path::ChangeExtension(u"", u""), u"");
    EXPECT_EQ(Path::ChangeExtension(u"a", u".ext"), u"a.ext");
    EXPECT_EQ(Path::ChangeExtension(u"a", u"ext"), u"a.ext");
    EXPECT_EQ(Path::ChangeExtension(u"a", u"..ext"), u"a..ext");
}

TEST(Path, GetFullPath)
{
    auto cd = Directory::GetCurrentDirectory();
    auto root = Directory::GetDirectoryRoot(cd);

    EXPECT_EQ(Path::GetFullPath(u"/"), root);
    EXPECT_EQ(Path::GetFullPath(u""), u"");
    EXPECT_EQ(Path::GetFullPath(u"/Users"), Path::Combine(root, u"Users"));
    EXPECT_EQ(Path::GetFullPath(u"Users"), Path::Combine(cd, u"Users"));
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_EQ(Path::GetFullPath(u"\\"), root);
    EXPECT_EQ(Path::GetFullPath(u"/Users/Desktop"), Path::Combine(root, u"Users\\Desktop"));
    EXPECT_EQ(Path::GetFullPath(u"Users/Desktop"), Path::Combine(cd, u"Users\\Desktop"));
#else
    EXPECT_EQ(Path::GetFullPath(u"/Users/Desktop"), Path::Combine(root, u"Users/Desktop"));
    EXPECT_EQ(Path::GetFullPath(u"Users/Desktop"), Path::Combine(cd, u"Users/Desktop"));
#endif
}

TEST(Path, GetPathRoot)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_EQ(Path::GetPathRoot(u"C:\\user/temp"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:/user/temp"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:/a"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:/"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:"), u"C:");
    EXPECT_EQ(Path::GetPathRoot(u"user/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"user/temp/image.png"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"/user/temp"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"//user/temp"), u"\\\\user\\temp");
    EXPECT_EQ(Path::GetPathRoot(u"//user/temp/image.png"), u"\\\\user\\temp");
    EXPECT_EQ(Path::GetPathRoot(u"\\User\\temp"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"\\volume/temp"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"\\\\volume\\temp\\"), u"\\\\volume\\temp");
    EXPECT_EQ(Path::GetPathRoot(u"\\\\volume\\temp\\temp\\image.png"), u"\\\\volume\\temp");
#else
    EXPECT_EQ(Path::GetPathRoot(u"C:\\user/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/user/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/a"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"user/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"user/temp/image.png"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"/user/temp"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"//user/temp"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"\\User\\temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\volume/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\\\volume\\temp\\"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\\\volume\\temp\\temp\\image.png"), u"");
#endif
    EXPECT_EQ(Path::GetPathRoot(u"user"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"user/temp"), u"");
    EXPECT_EQ(Path::GetPathRoot(u""), u"");
}

TEST(Path, GetRandomFileName)
{
    auto filename = Path::GetRandomFileName();
    for (size_t j = 0; j < filename.size(); ++j)
    {
        EXPECT_PRED1([j](auto&& c) { return j == 8 ? c == '.' : isalpha(c) || isdigit(c); }, filename[j]);
    }
}