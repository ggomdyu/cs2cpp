#include <gtest/gtest.h>
#include <System/Environment.h>
#include <System/String.h>

using tg::Environment;
using tg::String;

TEST(Environment, EnvironmentVariable)
{
    EXPECT_FALSE(Environment::GetEnvironmentVariable(u"CS2CPP_pokw9per23d55").has_value());

    Environment::SetEnvironmentVariable(u"CS2CPP_pokw9per23d55", u"C:/HU7AI");
    EXPECT_TRUE(Environment::GetEnvironmentVariable(u"CS2CPP_pokw9per23d55").has_value());
    EXPECT_EQ(*Environment::GetEnvironmentVariable(u"CS2CPP_pokw9per23d55"), String(u"C:/HU7AI"));
}


////
////class EnvironmentTest :
////    public Test
////{
////public:
////    void Evaluate() override
////    {
////        assert(Environment::GetSystemPageSize() == 4096);
////        assert(Environment::Is64BitProcess() == true);
////        assert(Environment::Is64BitOperatingSystem() == true);
////        assert(Environment::GetNewLine() == "\n");
////#endif
////        auto a = Environment::GetCommandLineArgs();
////        auto b = Environment::GetMachineName();
////        auto c = Environment::GetUserDomainName();
////        auto d = Environment::GetUserName();
////        auto e = Environment::GetEnvironmentVariable(u"PATH_TGON");
////        assert(e.has_value() == false);
////        auto f = Environment::SetEnvironmentVariable(u"PATH_TGON", u"C:/HU7AI");
////        auto g = Environment::GetEnvironmentVariable(u"PATH_TGON");
////        assert(g.value() == u"C:/HU7AI");
////        //auto h = UEnvironment::GetCurrentDirectory();
////        auto i = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
////        auto j = Environment::GetCurrentManagedThreadId();
////    }
////};
//
//
