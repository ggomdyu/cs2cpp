#include <System/Windows/Windows.h>
#include <algorithm>
#include <array>
#ifndef SECURITY_WIN32
#    define SECURITY_WIN32
#endif
#include <Psapi.h>
#include <ShlObj.h>
#include <security.h>
#include <shellapi.h>
#pragma pack(push, before_imagehlp, 8)
#include <ImageHlp.h>
#pragma pack(pop, before_imagehlp)

#include "System/Environment.h"
#include "System/ReadOnlySpan.h"
#include "System/Windows/SafeHandle.h"
#include "System/Windows/SafeRegistryHandle.h"

#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

CS2CPP_NAMESPACE_BEGIN

namespace
{

constexpr auto UserRegistryKeyName = L"Environment";
constexpr auto MachineRegistryKeyName = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";

std::optional<std::u16string> InternalExpandEnvironmentVariables(const std::wstring& name)
{
    auto length = ExpandEnvironmentStringsW(name.data(), nullptr, 0);
    if (length == 0)
    {
        return std::nullopt;
    }

    std::u16string ret;
    ret.resize(static_cast<size_t>(length - 1));
    if (ExpandEnvironmentStringsW(name.data(), reinterpret_cast<LPWSTR>(ret.data()), length) == 0)
    {
        return std::nullopt;
    }

    return ret;
}

std::optional<std::u16string> InternalGetEnvironmentVariable(const std::wstring& name)
{
    auto length = GetEnvironmentVariableW(name.data(), nullptr, 0);
    if (length == 0)
    {
        return std::nullopt;
    }

    std::u16string ret;
    ret.resize(static_cast<size_t>(length));
    if (GetEnvironmentVariableW(name.data(), reinterpret_cast<LPWSTR>(ret.data()), length) == 0)
    {
        return std::nullopt;
    }

    if (ret.length() > 0 && ret.back() == u'\0')
    {
        ret.resize(ret.length() - 1);
    }

    return {std::move(ret)};
}

std::optional<std::u16string> InternalGetRegistryVariable(const SafeRegistryHandle& registryHandle, const std::wstring& name)
{
    DWORD length;
    if (RegQueryValueExW(registryHandle, name.data(), nullptr, nullptr, nullptr, &length) != ERROR_SUCCESS)
    {
        return std::nullopt;
    }

    std::u16string ret;
    ret.resize(length / sizeof(char16_t) - 1);
    if (RegQueryValueExW(registryHandle, name.data(), nullptr, nullptr, reinterpret_cast<LPBYTE>(ret.data()), &length) != ERROR_SUCCESS)
    {
        return std::nullopt;
    }

    return ret;
}

std::optional<std::u16string> InternalGetRegistryVariable(HKEY predefinedKey, LPCWSTR subKey, const std::wstring& name)
{
    HKEY keyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ, &keyHandle) != ERROR_SUCCESS)
    {
        return std::nullopt;
    }

    return InternalGetRegistryVariable(SafeRegistryHandle(keyHandle), name);
}

std::map<std::u16string, std::u16string> InternalGetEnvironmentVariables(HKEY predefinedKey, LPCWSTR subKey)
{
    HKEY keyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ, &keyHandle) != ERROR_SUCCESS)
    {
        return {};
    }

    SafeRegistryHandle registryHandle(keyHandle);

    std::map<std::u16string, std::u16string> ret;
    std::array<wchar_t, 2048> name;
    for (DWORD i = 0;; ++i)
    {
        DWORD nameLen = name.size();
        auto result = RegEnumValueW(registryHandle, i, name.data(), &nameLen, nullptr, nullptr, nullptr, nullptr);
        if (result != ERROR_SUCCESS)
        {
            break;
        }

        auto key = std::u16string_view(reinterpret_cast<const char16_t*>(name.data()), static_cast<size_t>(nameLen));
        auto value = InternalGetRegistryVariable(registryHandle, {name.data(), nameLen}).value_or(std::u16string());

        ret[std::u16string(key)] = std::move(value);
    }

    return ret;
}

bool InternalSetEnvironmentVariable(const std::wstring& name, const std::wstring& value)
{
    return SetEnvironmentVariableW(name.data(), value.data()) == TRUE;
}

bool InternalSetRegistryVariable(HKEY predefinedKey, LPCWSTR subKey, const std::wstring& name, ReadOnlySpan<BYTE> value)
{
    HKEY rawKeyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ | KEY_WRITE, &rawKeyHandle) != ERROR_SUCCESS)
    {
        return false;
    }

    SafeRegistryHandle registryHandle(rawKeyHandle);
    if (value.IsEmpty())
    {
        if (RegDeleteValueW(registryHandle, name.data()) != ERROR_SUCCESS)
        {
            return false;
        }
    }
    else if (RegSetValueExW(registryHandle, name.data(), 0, REG_SZ, &value[0], static_cast<DWORD>(value.Length())) != ERROR_SUCCESS)
    {
        return false;
    }

    return true;
}

std::vector<std::u16string> InternalGetLogicalDrives()
{
    auto driveFlags = GetLogicalDrives();

    std::vector<std::u16string> ret;
    char16_t root[] = u"A:\\";
    for (auto drive = driveFlags; drive != 0; drive >>= 1)
    {
        if ((drive & 1) != 0)
        {
            ret.emplace_back(root, std::extent_v<decltype(root)> - 1);
        }

        ++root[0];
    }

    return ret;
}

std::vector<std::u16string> InternalGetCommandLineArgs()
{
    int numArgs;
    auto args = CommandLineToArgvW(GetCommandLineW(), &numArgs);
    if (!args)
    {
        return {};
    }

    std::vector<std::u16string> ret;
    for (int i = 0; i < numArgs; ++i)
    {
        ret.emplace_back(reinterpret_cast<const char16_t*>(args[i]));
    }

    return ret;
}

}

int32_t Environment::CurrentManagedThreadId()
{
    return static_cast<int32_t>(GetCurrentThreadId());
}

std::optional<std::u16string> Environment::ExpandEnvironmentVariables(std::u16string_view name)
{
    return InternalExpandEnvironmentVariables(std::wstring(name.begin(), name.end()));
}

void Environment::FailFast(std::u16string_view message)
{
    std::wstring wcsMessage;
    wcsMessage.reserve(15 + message.length());
    wcsMessage += L"FailFast:\n";
    wcsMessage += std::wstring_view(reinterpret_cast<const wchar_t*>(message.data()), message.length());
    wcsMessage += L"\n";

    OutputDebugStringW(wcsMessage.data());

    std::terminate();
}

std::u16string Environment::GetCurrentDirectory()
{
    auto length = GetCurrentDirectoryW(0, nullptr);
    if (length <= 0)
    {
        return {};
    }

    std::u16string ret;
    ret.resize(static_cast<size_t>(length - 1));
    GetCurrentDirectoryW(length, reinterpret_cast<wchar_t*>(ret.data()));

    return ret;
}

const std::vector<std::u16string>& Environment::GetCommandLineArgs()
{
    static auto commandLineArgs = InternalGetCommandLineArgs();
    return commandLineArgs;
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    return InternalGetEnvironmentVariable({name.begin(), name.end()});
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return GetEnvironmentVariable(name);
    }

    if (target == EnvironmentVariableTarget::User)
    {
        return InternalGetRegistryVariable(HKEY_CURRENT_USER, UserRegistryKeyName, {name.begin(), name.end()});
    }

    if (target == EnvironmentVariableTarget::Machine)
    {
        return InternalGetRegistryVariable(HKEY_LOCAL_MACHINE, MachineRegistryKeyName, {name.begin(), name.end()});
    }

    return std::nullopt;
}

std::map<std::u16string, std::u16string> Environment::GetEnvironmentVariables()
{
    std::unique_ptr<WCHAR[], decltype(&FreeEnvironmentStringsW)> strings(GetEnvironmentStringsW(), FreeEnvironmentStringsW);
    std::map<std::u16string, std::u16string> ret;

    for (size_t i = 0;; ++i)
    {
        if (strings[i] == L'\0' && strings[i + 1] == L'\0')
        {
            break;
        }

        auto startKey = i;
        while (strings[i] != L'=' && strings[i] != L'\0') ++i;

        if (strings[i] == L'\0')
        {
            continue;
        }

        if (i - startKey == 0)
        {
            while (strings[i] != 0) ++i;
            continue;
        }

        auto key = std::u16string(reinterpret_cast<const char16_t*>(&strings[startKey]), i - startKey);
        auto startValue = ++i;
        while (strings[i] != L'\0') ++i;

        ret[std::move(key)] = std::u16string(reinterpret_cast<const char16_t*>(&strings[startValue]), i - startValue);
    }

    return ret;
}

std::map<std::u16string, std::u16string> Environment::GetEnvironmentVariables(EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
    {
        return GetEnvironmentVariables();
    }

    if (target == EnvironmentVariableTarget::User)
    {
        return InternalGetEnvironmentVariables(HKEY_CURRENT_USER, UserRegistryKeyName);
    }

    if (target == EnvironmentVariableTarget::Machine)
    {
        return InternalGetEnvironmentVariables(HKEY_LOCAL_MACHINE, MachineRegistryKeyName);
    }

    return {};
}

std::u16string Environment::GetFolderPath(SpecialFolder folder)
{
    std::array<wchar_t, 4096> path{};
    if (SHGetFolderPathW(nullptr, static_cast<int>(folder), nullptr, 0, path.data()) != S_OK)
    {
        return {};
    }

    return reinterpret_cast<const char16_t*>(path.data());
}

std::vector<std::u16string> Environment::GetLogicalDrives()
{
    static auto logicalDrives = InternalGetLogicalDrives();
    return logicalDrives;
}

std::u16string Environment::MachineName()
{
    std::array<wchar_t, 2048> name{};
    DWORD nameLen = name.size();
    if (GetComputerNameW(name.data(), &nameLen) == FALSE)
    {
        return {};
    }

    return {reinterpret_cast<const char16_t*>(name.data()), nameLen};
}

std::optional<OperatingSystem> Environment::OSVersion()
{
    HMODULE ntdllHandle = GetModuleHandleW(L"ntdll");
    if (!ntdllHandle)
    {
        return std::nullopt;
    }

    using LPFN_RTLGETVER = NTSTATUS(WINAPI*)(LPOSVERSIONINFOEXW);
    LPFN_RTLGETVER rtlGetVersion = reinterpret_cast<LPFN_RTLGETVER>(GetProcAddress(ntdllHandle, "RtlGetVersion"));
    if (!rtlGetVersion)
    {
        return std::nullopt;
    }

    OSVERSIONINFOEX osInfo{};
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (rtlGetVersion(&osInfo) < 0)
    {
        return std::nullopt;
    }

    Version version(
        static_cast<int32_t>(osInfo.dwMajorVersion),
        static_cast<int32_t>(osInfo.dwMinorVersion),
        static_cast<int32_t>(osInfo.dwBuildNumber),
        (osInfo.wServicePackMajor << 16) | osInfo.wServicePackMinor
    );

    return OperatingSystem(PlatformID::Win32NT, version, osInfo.szCSDVersion[0] != L'\0'
        ? reinterpret_cast<const char16_t*>(&osInfo.szCSDVersion[0])
        : std::u16string());
}

bool Environment::SetCurrentDirectory(std::u16string_view path)
{
    return SetCurrentDirectoryW(reinterpret_cast<LPCWSTR>(std::u16string(path).c_str())) == TRUE;
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    if (!IsValidEnvironmentVariableName(name))
    {
        return false;
    }

    return InternalSetEnvironmentVariable({name.begin(), name.end()}, {value.begin(), value.end()});
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target)
{
    if (!IsValidEnvironmentVariableName(name))
    {
        return false;
    }

    if (target == EnvironmentVariableTarget::Process)
    {
        return InternalSetEnvironmentVariable({name.begin(), name.end()}, {value.begin(), value.end()});
    }

    if (target == EnvironmentVariableTarget::Machine)
    {
        if (name.length() >= MaxSystemEnvVariableLength)
        {
            return false;
        }

        auto subKey = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
        return InternalSetRegistryVariable(HKEY_LOCAL_MACHINE, subKey, {name.begin(), name.end()},
            {reinterpret_cast<const BYTE*>(value.data()), static_cast<int32_t>(value.size() * sizeof(value[0]))});
    }

    if (target == EnvironmentVariableTarget::User)
    {
        if (name.length() >= MaxUserEnvVariableLength)
        {
            return false;
        }

        auto subKey = L"Environment";
        return InternalSetRegistryVariable(HKEY_CURRENT_USER, subKey, {name.begin(), name.end()},
            {reinterpret_cast<const BYTE*>(value.data()), static_cast<int32_t>(value.size() * sizeof(value[0]))});
    }

    return false;
}

CS2CPP_NOINLINE std::u16string Environment::StackTrace()
{
    auto processHandle = GetCurrentProcess();
    if (SymInitialize(processHandle, nullptr, TRUE) == FALSE)
    {
        return {};
    }

    SymSetOptions(SYMOPT_LOAD_LINES);

    constexpr DWORD MaxStackTraceLineNum = 2048;

    DWORD frameHash;
    auto frames = std::array<void*, MaxStackTraceLineNum>{};
    auto frameCount = RtlCaptureStackBackTrace(0, MaxStackTraceLineNum, frames.data(), &frameHash);
    if (frameCount < 1)
    {
        return {};
    }

    // The stack backtrace requires many buffer spaces most of the time,
    // so we will reserve the size of the buffer as many as possible.
    std::u16string stackTrace;
    stackTrace.reserve(static_cast<size_t>(frameCount) * 64);

    struct SymbolInfo : SYMBOL_INFOW { std::byte buffer[1024]; };
    SymbolInfo symbol{};
    symbol.MaxNameLen = 1024;
    symbol.SizeOfStruct = sizeof(SYMBOL_INFOW);

    IMAGEHLP_LINEW64 line;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    wchar_t wcsLineBuffer[2048];

    // Begin index at 1 to ignore the trace of the current function.
    for (WORD i = 1; i < frameCount; ++i)
    {
        auto frame = reinterpret_cast<DWORD64>(frames[i]);
        if (SymFromAddrW(processHandle, frame, nullptr, &symbol) == FALSE)
        {
            continue;
        }

        DWORD displacement;
        size_t wcsLineLen = SymGetLineFromAddrW64(processHandle, frame, &displacement, &line) == TRUE
            ? wsprintfW(wcsLineBuffer, L"   at %s in %s:line %d\n", symbol.Name, line.FileName, line.LineNumber)
            : wsprintfW(wcsLineBuffer, L"   at %s\n", symbol.Name);

        stackTrace += std::u16string_view(reinterpret_cast<const char16_t*>(wcsLineBuffer), wcsLineLen);
    }

    // Remove the last newline.
    if (!stackTrace.empty() && stackTrace.back() == u'\n')
    {
        stackTrace.pop_back();
    }

    return stackTrace;
}

int32_t Environment::SystemPageSize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    return static_cast<int32_t>(si.dwPageSize);
}

int64_t Environment::TickCount64()
{
    return static_cast<int64_t>(::GetTickCount64());
}

int32_t Environment::ProcessId()
{
    return static_cast<int32_t>(GetCurrentProcessId());
}

std::u16string Environment::UserDomainName()
{
    std::array<wchar_t, 2048> name{};
    DWORD nameLen = name.size();
    if (GetUserNameExW(NameSamCompatible, name.data(), &nameLen) == 0)
    {
        return {};
    }

    auto str = wcschr(name.data(), L'\\');
    if (!str)
    {
        return {};
    }

    return std::u16string(reinterpret_cast<const char16_t*>(name.data()), str - name.data());
}

bool Environment::UserInteractive()
{
    static auto cachedWindowStationHandle = HWINSTA(nullptr);
    static auto isUserNonInteractive = false;

    auto windowStationHandle = GetProcessWindowStation();
    if (windowStationHandle && windowStationHandle != cachedWindowStationHandle)
    {
        USEROBJECTFLAGS flags;
        DWORD lengthNeeded;
        if (GetUserObjectInformationW(windowStationHandle, UOI_FLAGS, reinterpret_cast<PVOID>(&flags), sizeof(flags), &lengthNeeded) == TRUE)
        {
            if ((flags.dwFlags & WSF_VISIBLE) == 0)
            {
                isUserNonInteractive = true;
            }
        }

        cachedWindowStationHandle = windowStationHandle;
    }

    return !isUserNonInteractive;
}

std::u16string Environment::UserName()
{
    std::array<wchar_t, 2048> name;
    DWORD nameLen = name.size();
    if (GetUserNameW(name.data(), &nameLen) == FALSE)
    {
        return {};
    }

    return {reinterpret_cast<const char16_t*>(name.data()), nameLen};
}

int64_t Environment::WorkingSet()
{
    auto processHandle = SafeHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId()));

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(processHandle, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc)) == FALSE)
    {
        return 0;
    }

    return pmc.WorkingSetSize;
}

bool Environment::Is64BitOperatingSystemWhen32BitProcess()
{
#if defined(_WIN64)
    // 64-bit programs run only on 64-bit
    return true;
#else
    BOOL isWow64 = FALSE;
    return IsWow64Process(GetCurrentProcess(), &isWow64) && isWow64;
#endif
}

CS2CPP_NAMESPACE_END
