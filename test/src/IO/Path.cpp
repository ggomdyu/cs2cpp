#include <gtest/gtest.h>
#include <System/IO/Path.h>

using namespace tg;

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
#elif CS2CPP_PLATFORM_MACOS
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

TEST(Path, GetExtension)
{
    EXPECT_EQ(Path::GetExtension(u"/Users/chajunho/Desktop/1.png"), u".png");
    EXPECT_EQ(Path::GetExtension(u"/Users/chajunho/Desktop/1"), u"");
    EXPECT_EQ(Path::GetExtension(u"1.png"), u".png");
    EXPECT_EQ(Path::GetExtension(u"1"), u"");
    EXPECT_EQ(Path::GetExtension(u""), u"");
}

TEST(Path, GetFileName)
{
    EXPECT_EQ(Path::GetFileName(u"/Users/chajunho/Desktop/1.png"), u"1.png");
    EXPECT_EQ(Path::GetFileName(u""), u"");
}

TEST(Path, GetFileNameWithoutExtension)
{
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u""), u"");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/123"), u"123");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"123123"), u"123123");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"1"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"1.png"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1.png"), u"1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1.1.png"), u"1.1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/chajunho/Desktop/1.1.png"), u"1.1");
    EXPECT_EQ(Path::GetFileNameWithoutExtension(u"/Users/123123.png"), u"123123");
}

TEST(Path, GetDirectoryName)
{
    EXPECT_EQ(Path::GetDirectoryName(u"C:/1.png"), u"C:");
    EXPECT_EQ(Path::GetDirectoryName(u"C:\\1.png"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"C:/Dir/1.png"), u"C:/Dir");
    EXPECT_EQ(Path::GetDirectoryName(u"/Users/chajunho/Desktop/1.png"), u"/Users/chajunho/Desktop");
    EXPECT_EQ(Path::GetDirectoryName(u""), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"abc.png"), u"");
    EXPECT_EQ(Path::GetDirectoryName(u"abc"), u"");
}

TEST(Path, HasExtension)
{
    EXPECT_EQ(Path::HasExtension(u"/Users/chajunho/Desktop/1.png"), true);
    EXPECT_EQ(Path::HasExtension(u"/Users/chajunho/Desktop/1"), false);
    EXPECT_EQ(Path::HasExtension(u"/Users/chajunho/Desktop/"), false);
    EXPECT_EQ(Path::HasExtension(u"/Users/chajunho/Desktop"), false);
    EXPECT_EQ(Path::HasExtension(u"1.png"), true);
    EXPECT_EQ(Path::HasExtension(u".png"), true);
    EXPECT_EQ(Path::HasExtension(u"png"), false);
}

TEST(Path, ChangeExtension)
{
    EXPECT_EQ(Path::ChangeExtension(u"/Users/chajunho/Desktop/1.png", u"ext"), u"/Users/chajunho/Desktop/1.ext");
    EXPECT_EQ(Path::ChangeExtension(u".png", u"ext"), u".ext");
    EXPECT_EQ(Path::ChangeExtension(u"/Users/chajunho/Desktop", u"ext"), u"/Users/chajunho/Desktop.ext");
    EXPECT_EQ(Path::ChangeExtension(u"/Users/chajunho/Desktop/", u"ext"), u"/Users/chajunho/Desktop/.ext");
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
    EXPECT_EQ(Path::GetFullPath(u""), u"");
    EXPECT_EQ(Path::GetFullPath(u"/"), u"/");
}

TEST(Path, GetPathRoot)
{
    EXPECT_EQ(Path::GetPathRoot(u"/Users/chajunho/"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"/"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"/Users"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u""), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C://"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:\\"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\Users\\chajunho\\"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\"), u"");
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_EQ(Path::GetPathRoot(u"/Users/chajunho/"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"/"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"/Users"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u""), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:"), u"C:");
    EXPECT_EQ(Path::GetPathRoot(u"C://"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"C:\\"), u"C:\\");
    EXPECT_EQ(Path::GetPathRoot(u"\\Users\\chajunho\\"), u"\\");
    EXPECT_EQ(Path::GetPathRoot(u"\\"), u"\\");
#elif CS2CPP_PLATFORM_MACOS
    EXPECT_EQ(Path::GetPathRoot(u"/Users/chajunho/"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"/"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u"/Users"), u"/");
    EXPECT_EQ(Path::GetPathRoot(u""), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:/"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C://"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"C:\\"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\Users\\chajunho\\"), u"");
    EXPECT_EQ(Path::GetPathRoot(u"\\"), u"");
#endif
}
