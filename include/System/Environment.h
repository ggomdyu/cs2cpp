#pragma once

#include <map>
#include <optional>
#include <vector>

#include "OperatingSystem.h"

CS2CPP_NAMESPACE_BEGIN

enum class EnvironmentVariableTarget
{
    Process = 0,
    User = 1,
    Machine = 2,
};

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
    static bool SetEnvironmentVariable(std::u16string_view name, std::u16string_view value);
    static bool SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target);
    [[nodiscard]] static std::optional<std::u16string> GetEnvironmentVariable(std::u16string_view name);
    [[nodiscard]] static std::optional<std::u16string> GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target);
    [[nodiscard]] static std::u16string GetCurrentDirectory();
    [[nodiscard]] static std::u16string GetFolderPath(SpecialFolder folder);
    [[nodiscard]] static constexpr std::u16string_view GetNewLine() noexcept;
    [[nodiscard]] static int32_t GetSystemPageSize();
    [[nodiscard]] static int32_t GetCurrentManagedThreadId();
    [[nodiscard]] static std::u16string GetUserName();
    [[nodiscard]] static std::u16string GetMachineName();
    [[nodiscard]] static std::u16string GetUserDomainName();
    [[nodiscard]] static const std::u16string& GetCommandLine();
    [[nodiscard]] static const std::vector<std::u16string>& GetCommandLineArgs();
    [[noreturn]] static void Exit(int32_t exitCode);
    [[nodiscard]] static int64_t GetTickCount();
    [[nodiscard]] static int32_t GetProcessorCount();
    [[nodiscard]] static bool Is64BitProcess();
    [[nodiscard]] static bool Is64BitOperatingSystem();
    [[noreturn]] static void FailFast(std::u16string_view message);
    [[nodiscard]] static std::u16string GetStackTrace();
    [[nodiscard]] static std::u16string GetSystemDirectory();
    [[nodiscard]] static std::map<std::u16string, std::u16string> GetEnvironmentVariables();
    [[nodiscard]] static OperatingSystem GetOSVersion();
    [[nodiscard]] static bool GetUserInteractive();
    [[nodiscard]] static int64_t GetWorkingSet();
    [[nodiscard]] static std::vector<std::u16string> GetLogicalDrives();

    //static std::map<String, String> GetEnvironmentVariables(EnvironmentVariableTarget target);
    //public static string ExpandEnvironmentVariables(string name);

private:
    static constexpr bool IsValidEnvironmentVariableName(std::u16string_view name) noexcept;

private:
    static constexpr int32_t MaxEnvVariableValueLength = 32767;
    static constexpr int32_t MaxSystemEnvVariableLength = 1024;
    static constexpr int32_t MaxUserEnvVariableLength = 255;
};

constexpr bool Environment::IsValidEnvironmentVariableName(std::u16string_view name) noexcept
{
    if (name.length() == 0 || name[0] == u'\0')
        return false;

    if (name.length() >= MaxEnvVariableValueLength)
        return false;

    if (name.find(u'=') != std::u16string_view::npos)
        return false;

    return true;
}

CS2CPP_NAMESPACE_END

#if CS2CPP_PLATFORM_WINDOWS
#    include "Environment.Windows.inl"
#elif CS2CPP_PLATFORM_MACOS
#    include "Environment.MacOS.inl"
#endif
