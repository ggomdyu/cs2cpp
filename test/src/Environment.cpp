#include <System.IO/Directory.h>
#include <System/DateTime.h>
#include <System/Environment.h>
#include <fmt/xchar.h>
#include <gtest/gtest.h>
#include <thread>
#if CS2CPP_PLATFORM_WINDOWS
#    include <System/Windows/Windows.h>
#else
#    include <pthread.h>
#endif

using namespace CS2CPP_NAMESPACE_NAME;

TEST(Environment, CurrentManagedThreadId)
{
    Environment::GetCommandLineArgs();
    EXPECT_GT(Environment::CurrentManagedThreadId(), 0);
}

TEST(Environment, ExpandEnvironmentVariables)
{
#if CS2CPP_PLATFORM_WINDOWS
    auto randomName = Path::GetRandomFileName();
    auto randomValue = Path::GetRandomFileName();

    EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, randomValue));
    EXPECT_EQ(Environment::ExpandEnvironmentVariables(randomName), randomName);
    auto programFiles = Environment::ExpandEnvironmentVariables(u"%ProgramFiles%");
    EXPECT_TRUE(programFiles.has_value());
    EXPECT_GT(programFiles->length(), 0);
#endif
}

TEST(Environment, GetCommandLineArgs)
{
    std::u16string commandLine(Environment::CommandLine());

    decltype(auto) args = Environment::GetCommandLineArgs();
    for (const auto& arg : args)
    {
        auto idx = commandLine.find(arg);
        auto removeBeginIt = commandLine.begin() + idx;
        auto removeEndIt = removeBeginIt + arg.length();
        commandLine.erase(removeBeginIt, removeEndIt);
    }

    while (!commandLine.empty() && commandLine.back() == ' ')
    {
        commandLine.pop_back();
    }

    EXPECT_EQ(commandLine.length(), 0);
}

TEST(Environment, GetCurrentDirectory)
{
    auto cd = Environment::GetCurrentDirectory();
    EXPECT_GT(cd.length(), 0);
    EXPECT_TRUE(Directory::Exists(cd));
}

TEST(Environment, GetEnvironmentVariables)
{
    // Process environment variables
    {
        auto randomName = Path::GetRandomFileName();
        auto randomValue = Path::GetRandomFileName();

        EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, randomValue));
        auto envDict = Environment::GetEnvironmentVariables();
        auto it = envDict.find(randomName);
        EXPECT_TRUE(it != envDict.end());
        if (it != envDict.end())
        {
            EXPECT_EQ(it->second, randomValue);
        }
    }

    // Registry environment variables
#if CS2CPP_PLATFORM_WINDOWS
    {
        auto randomName = Path::GetRandomFileName();
        auto randomValue = Path::GetRandomFileName();
        EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, randomValue, EnvironmentVariableTarget::User));
        auto envDict2 = Environment::GetEnvironmentVariables(EnvironmentVariableTarget::User);
        auto it = envDict2.find(randomName);
        EXPECT_TRUE(it != envDict2.end());
        if (it != envDict2.end())
        {
            EXPECT_EQ(it->second, randomValue);
        }
        EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, u"", EnvironmentVariableTarget::User));
        EXPECT_FALSE(Environment::GetEnvironmentVariable(randomName, EnvironmentVariableTarget::User));
    }
#endif
}

TEST(Environment, GetFolderPath)
{
    auto desktop = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
    EXPECT_GT(desktop.length(), 0);
    EXPECT_TRUE(Directory::Exists(desktop));
}

TEST(Environment, GetLogicalDrives)
{
    auto logicalDrives = Environment::GetLogicalDrives();
    EXPECT_FALSE(logicalDrives.empty());
    if (!logicalDrives.empty())
    {
        EXPECT_GT(logicalDrives.back().size(), 0);
    }
}

TEST(Environment, Is64BitOperatingSystem)
{
    if constexpr (sizeof(intptr_t) == 8)
    {
        EXPECT_TRUE(Environment::Is64BitOperatingSystem());
    }
}

TEST(Environment, Is64BitProcess)
{
    if constexpr (sizeof(intptr_t) == 8)
    {
        EXPECT_TRUE(Environment::Is64BitProcess());
    }
    else
    {
        EXPECT_FALSE(Environment::Is64BitProcess());
    }
}

TEST(Environment, MachineName)
{
    EXPECT_GT(Environment::MachineName().length(), 0);
}

TEST(Environment, NewLine)
{
    EXPECT_GT(Environment::NewLine().length(), 0);
}

TEST(Environment, OSVersion)
{
    EXPECT_TRUE(Environment::OSVersion().has_value());
}

TEST(Environment, ProcessorCount)
{
    EXPECT_GT(Environment::ProcessorCount(), 0);
}

TEST(Environment, SetCurrentDirectory)
{
    auto oldCurrentDirectory = Environment::GetCurrentDirectory();
    EXPECT_TRUE(Environment::SetCurrentDirectory(Path::Combine(oldCurrentDirectory, u"/..")));

    auto newCurrentDirectory = Environment::GetCurrentDirectory();
    EXPECT_NE(newCurrentDirectory, oldCurrentDirectory);
    EXPECT_TRUE(Directory::Exists(newCurrentDirectory));
    EXPECT_TRUE(Environment::SetCurrentDirectory(oldCurrentDirectory));
}

TEST(Environment, SetEnvironmentVariable)
{
    // Process environment variable
    {
        auto randomName = Path::GetRandomFileName();
        auto randomValue = Path::GetRandomFileName();
        EXPECT_FALSE(Environment::GetEnvironmentVariable(randomName).has_value());
        EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, randomValue));

        auto envValue = Environment::GetEnvironmentVariable(randomName);
        EXPECT_TRUE(envValue.has_value());
        if (envValue.has_value())
        {
            EXPECT_EQ(envValue.value(), randomValue);
            EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, u""));
            EXPECT_FALSE(Environment::GetEnvironmentVariable(randomName).has_value());
        }
    }

    // Registry environment variable
#if CS2CPP_PLATFORM_WINDOWS
    {
        auto randomName = Path::GetRandomFileName();
        auto randomValue = Path::GetRandomFileName();
        EXPECT_FALSE(Environment::GetEnvironmentVariable(randomName, EnvironmentVariableTarget::User).has_value());
        EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, randomValue, EnvironmentVariableTarget::User));

        auto envValue = Environment::GetEnvironmentVariable(randomName, EnvironmentVariableTarget::User);
        EXPECT_TRUE(envValue.has_value());
        if (envValue.has_value())
        {
            EXPECT_EQ(envValue.value(), randomValue);
            EXPECT_TRUE(Environment::SetEnvironmentVariable(randomName, u"", EnvironmentVariableTarget::User));
            EXPECT_FALSE(Environment::GetEnvironmentVariable(randomName, EnvironmentVariableTarget::User).has_value());
        }
    };
#endif
}

TEST(Environment, StackTrace)
{
    auto stackTrace = Environment::StackTrace(); auto lineNum = __LINE__;
    EXPECT_GT(stackTrace.length(), 0);
#if _DEBUG
    EXPECT_TRUE(stackTrace.find(u"Environment_StackTrace_Test") != std::string::npos);
    EXPECT_TRUE(stackTrace.find(fmt::format(u"Environment.cpp:line {}", lineNum)) != std::string::npos);
#endif
}

TEST(Environment, SystemDirectory)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_GT(Environment::SystemDirectory().length(), 0);
#endif
}

TEST(Environment, SystemPageSize)
{
    EXPECT_GT(Environment::SystemPageSize(), 0);
}

TEST(Environment, UserName)
{
    EXPECT_GT(Environment::UserName().length(), 0);
}

TEST(Environment, TickCount)
{
    auto oldTickCount = Environment::TickCount();
    EXPECT_GT(oldTickCount, 0);

    // Wait 100 milliseconds
    auto oldTime = DateTime::Now();
    while ((DateTime::Now() - oldTime).Milliseconds() < 100);

    auto currentTickCount = Environment::TickCount() - oldTickCount;
    EXPECT_GT(currentTickCount, 0);
    EXPECT_LT(currentTickCount, 200);
}

TEST(Environment, TickCount64)
{
    auto oldTickCount = Environment::TickCount64();
    EXPECT_GT(oldTickCount, 0);

    // Wait 100 milliseconds
    auto oldTime = DateTime::Now();
    while ((DateTime::Now() - oldTime).Milliseconds() < 100);

    auto currentTickCount = Environment::TickCount64() - oldTickCount;
    EXPECT_GT(currentTickCount, 0);
    EXPECT_LT(currentTickCount, 200);
}

TEST(Environment, UserDomainName)
{
    EXPECT_GT(Environment::UserDomainName().length(), 0);
}

TEST(Environment, WorkingSet)
{
#if CS2CPP_PLATFORM_WINDOWS
    EXPECT_GT(Environment::WorkingSet(), 0);
#endif
}
