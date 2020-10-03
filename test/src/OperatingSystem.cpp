#include <System/OperatingSystem.h>
#include <gtest/gtest.h>

using namespace CS2CPP_NAMESPACE_NAME;

TEST(OperatingSystem, OperatorEqual)
{
    auto a = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"");
    auto b = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"service");
    EXPECT_EQ(a, a);
    EXPECT_EQ(b, b);
}

TEST(OperatingSystem, OperatorNotEqual)
{
    auto a = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"");
    auto b = OperatingSystem(PlatformID::Unix, Version(1, 0, 1), u"");
    auto c = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1, 2), u"");
    auto d = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"service");
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST(OperatingSystem, GetPlatform)
{
    auto os = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"");
    EXPECT_EQ(os.GetPlatform(), PlatformID::Win32NT);
}

TEST(OperatingSystem, GetVersion)
{
    auto os = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"");
    EXPECT_EQ(os.GetVersion(), Version(1, 0, 1));
}

TEST(OperatingSystem, GetServicePack)
{
    auto os = OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"service");
    EXPECT_EQ(os.GetServicePack(), u"service");
}

TEST(OperatingSystem, ToString)
{
    EXPECT_EQ(OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"").ToString(), u"Microsoft Windows NT 1.0.1");
    EXPECT_EQ(OperatingSystem(PlatformID::Win32NT, Version(1, 0, 1), u"service").ToString(), u"Microsoft Windows NT 1.0.1 service");
}
