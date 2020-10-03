#include <System/Version.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Version, OperatorEqual)
{
    EXPECT_EQ(Version(1,2,3,4), Version(1,2,3,4));
}

TEST(Version, OperatorNotEqual)
{
    EXPECT_NE(Version(1,2,3,5), Version(1,2,3,4));
}

TEST(Version, OperatorGreaterThan)
{
    EXPECT_GT(Version(1,2,3,5), Version(1,2,3,4));
}

TEST(Version, OperatorGreaterThanOrEqual)
{
    EXPECT_GE(Version(1,2,3,5), Version(1,2,3,4));
    EXPECT_GE(Version(1,2,3,5), Version(1,2,3,5));
}

TEST(Version, OperatorLessThan)
{
    EXPECT_LT(Version(1,2,3,4), Version(1,2,3,5));
}

TEST(Version, OperatorLessThanOrEqual)
{
    EXPECT_LE(Version(1,2,3,4), Version(1,2,3,5));
    EXPECT_LE(Version(1,2,3,5), Version(1,2,3,5));
}

TEST(Version, GetMajor)
{
    EXPECT_EQ(Version(1,2,3,4).GetMajor(), 1);
}

TEST(Version, GetMinor)
{
    EXPECT_EQ(Version(1,2,3,4).GetMinor(), 2);
}

TEST(Version, GetBuild)
{
    EXPECT_EQ(Version(1,2,3,4).GetBuild(), 3);
}

TEST(Version, GetRevision)
{
    EXPECT_EQ(Version(1,2,3,4).GetRevision(), 4);
}

TEST(Version, GetHashCode)
{
    EXPECT_EQ(Version(1,2,3,4).GetHashCode(), 270544900);
}

TEST(Version, CompareTo)
{
    Version a(1,2,3,4), b(1,2,3,5);
    EXPECT_EQ(a.CompareTo(b), -1);
    EXPECT_EQ(b.CompareTo(a), 1);
    EXPECT_EQ(a.CompareTo(a), 0);
}

TEST(Version, ToString)
{
    EXPECT_EQ(Version(1,2).ToString(), u"1.2");
    EXPECT_EQ(Version(1,2,3).ToString(), u"1.2.3");
    EXPECT_EQ(Version(1,2,3,4).ToString(), u"1.2.3.4");
}
