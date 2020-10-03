#pragma once

#include <map>
#include <optional>
#include <vector>

#include "EnvironmentVariableTarget.h"
#include "OperatingSystem.h"

CS2CPP_NAMESPACE_BEGIN

class Environment final
{
public:
    Environment() = delete;

public:
    enum class SpecialFolder
    {
        Desktop = 0,
        Programs = 2,
        MyDocuments = 5,
        Personal = 5,
        Favorites = 6,
        Startup = 7,
        Recent = 8,
        SendTo = 9,
        StartMenu = 11,
        MyMusic = 13,
        MyVideos = 14,
        DesktopDirectory = 16,
        MyComputer = 17,
        NetworkShortcuts = 19,
        Fonts = 20,
        Templates = 21,
        CommonStartMenu = 22,
        CommonPrograms = 23,
        CommonStartup = 24,
        CommonDesktopDirectory = 25,
        ApplicationData = 26,
        PrinterShortcuts = 27,
        LocalApplicationData = 28,
        InternetCache = 32,
        Cookies = 33,
        History = 34,
        CommonApplicationData = 35,
        Windows = 36,
        System = 37,
        ProgramFiles = 38,
        MyPictures = 39,
        UserProfile = 40,
        SystemX86 = 41,
        ProgramFilesX86 = 42,
        CommonProgramFiles = 43,
        CommonProgramFilesX86 = 44,
        CommonTemplates = 45,
        CommonDocuments = 46,
        CommonAdminTools = 47,
        AdminTools = 48,
        CommonMusic = 53,
        CommonPictures = 54,
        CommonVideos = 55,
        Resources = 56,
        LocalizedResources = 57,
        CommonOemLinks = 58,
        CDBurning = 59,
    };

public:
    static std::u16string_view CommandLine();
    static int32_t CurrentManagedThreadId();
    [[noreturn]] static void Exit(int32_t exitCode);
    static std::optional<std::u16string> ExpandEnvironmentVariables(std::u16string_view name);
    [[noreturn]] static void FailFast(std::u16string_view message);
    static const std::vector<std::u16string>& GetCommandLineArgs();
    static std::u16string GetCurrentDirectory();
    static std::optional<std::u16string> GetEnvironmentVariable(std::u16string_view name);
    static std::optional<std::u16string> GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target);
    static std::map<std::u16string, std::u16string> GetEnvironmentVariables();
    static std::map<std::u16string, std::u16string> GetEnvironmentVariables(EnvironmentVariableTarget target);
    static std::u16string GetFolderPath(SpecialFolder folder);
    static std::vector<std::u16string> GetLogicalDrives();
    static bool Is64BitOperatingSystem();
    static bool Is64BitProcess();
    static std::u16string MachineName();
    static constexpr std::u16string_view NewLine() noexcept;
    static std::optional<OperatingSystem> OSVersion();
    static int32_t ProcessorCount();
    static int32_t ProcessId();
    static bool SetCurrentDirectory(std::u16string_view path);
    static bool SetEnvironmentVariable(std::u16string_view name, std::u16string_view value);
    static bool SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target);
    static std::u16string StackTrace();
    static std::u16string SystemDirectory();
    static int32_t SystemPageSize();
    static int32_t TickCount();
    static int64_t TickCount64();
    static std::u16string UserDomainName();
    static bool UserInteractive();
    static std::u16string UserName();
    static int64_t WorkingSet();

private:
    static constexpr bool IsValidEnvironmentVariableName(std::u16string_view name) noexcept;
    static bool Is64BitOperatingSystemWhen32BitProcess();
    static PlatformID GetPlatform() noexcept;

private:
    static constexpr int32_t MaxEnvVariableValueLength = 32767;
    static constexpr int32_t MaxSystemEnvVariableLength = 1024;
    static constexpr int32_t MaxUserEnvVariableLength = 255;
};

constexpr bool Environment::IsValidEnvironmentVariableName(std::u16string_view name) noexcept
{
    if (name.empty() || name[0] == u'\0')
    {
        return false;
    }

    if (name.length() >= MaxEnvVariableValueLength)
    {
        return false;
    }

    if (name.find(u'=') != std::u16string_view::npos)
    {
        return false;
    }

    return true;
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "Environment.Windows.inl"
#else
#    include "Environment.Posix.inl"
#endif
