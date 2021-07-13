#include <array>
#include <System/Windows/Windows.h>
#ifndef SECURITY_WIN32
#    define SECURITY_WIN32
#endif
#include <Psapi.h>
#include <security.h>
#include <shlobj.h>
#pragma pack(push, before_imagehlp, 8)
#   include <imagehlp.h>
#pragma pack(pop, before_imagehlp)

#include "System/Environment.h"
#include "System/Windows/SafeHandle.h"
#include "System/Windows/SafeRegistryHandle.h"

#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

#define CS2CPP_THROW_SEH_EXCEPTION() volatile int temp = 0; temp = 1 / temp;

CS2CPP_NAMESPACE_BEGIN

namespace detail::environment
{

bool InternalSetEnvironmentVariable(HKEY key, LPCWSTR subKey, std::u16string_view name, std::u16string_view value)
{
    HKEY rawKeyHandle;
    if (RegOpenKeyExW(key, subKey, 0, KEY_READ | KEY_WRITE, &rawKeyHandle) != ERROR_SUCCESS)
        return false;

    SafeRegistryHandle keyHandle(rawKeyHandle);

    auto wideCharName = reinterpret_cast<LPCWSTR>(name.data());
    if (!value.empty())
    {
        if (RegSetValueExW(keyHandle, wideCharName, 0, REG_SZ, reinterpret_cast<const BYTE*>(value.data()),
            static_cast<DWORD>(value.length()) + 1) != ERROR_SUCCESS)
            return false;
    }
    else
    {
        if (RegDeleteValueW(keyHandle, wideCharName) != ERROR_SUCCESS)
            return false;
    }

    return true;
}

std::optional<std::u16string> InternalGetEnvironmentVariable(HKEY predefinedKey, LPCWSTR subKey, std::u16string_view name)
{
    HKEY nativeKeyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ, &nativeKeyHandle) != ERROR_SUCCESS)
        return {};

    SafeRegistryHandle keyHandle(nativeKeyHandle);

    DWORD type = 0;
    auto buffer = std::array<char16_t, 8192>{};
    auto bufferSize = static_cast<DWORD>(buffer.size());

    if (RegQueryValueExW(keyHandle, reinterpret_cast<LPCWSTR>(name.data()), nullptr, &type,
        reinterpret_cast<LPBYTE>(buffer.data()), &bufferSize) != ERROR_SUCCESS)
        return {};

    return buffer.data();
}

std::vector<HMODULE> GetAllProcessModule(HANDLE processHandle)
{
    DWORD tempModuleByteCount = 0;
    if (EnumProcessModules(processHandle, nullptr, 0, &tempModuleByteCount) == 0)
        return {};

    std::vector<HMODULE> moduleHandles(tempModuleByteCount / sizeof(HMODULE));
    if (EnumProcessModules(processHandle, &moduleHandles[0], static_cast<DWORD>(moduleHandles.size()) * sizeof(HMODULE),
        &tempModuleByteCount) == 0)
        return {};

    return moduleHandles;
}

CS2CPP_NOINLINE DWORD InternalGetStackTrace(EXCEPTION_POINTERS* ep, std::u16string& destStr)
{
    using namespace detail::environment;

    // The stack backtrace requires many buffer spaces most of the time,
    // so we will reserve the size of the buffer as many as possible.
    destStr.reserve(1024);

    HANDLE threadHandle = GetCurrentThread();
    HANDLE processHandle = GetCurrentProcess();
    if (SymInitialize(processHandle, nullptr, false) == FALSE)
        return EXCEPTION_EXECUTE_HANDLER;

    SymSetOptions(SymGetOptions() | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

    PCONTEXT context = ep->ContextRecord;
#ifdef _M_X64
    STACKFRAME64 frame{};
    frame.AddrPC.Offset = context->Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Rsp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;
#else
    STACKFRAME64 frame{};
    frame.AddrPC.Offset = context->Eip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Offset = context->Esp;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrFrame.Mode = AddrModeFlat;
#endif

    std::vector<HMODULE> moduleHandles = GetAllProcessModule(processHandle);
    void* baseModuleAddress = nullptr;

    // Load all symbol data from the pdb file.
    for (auto it = moduleHandles.rbegin(); it != moduleHandles.rend(); ++it)
    {
        auto moduleHandle = *it;
        MODULEINFO moduleInfo;
        GetModuleInformation(processHandle, moduleHandle, &moduleInfo, sizeof(moduleInfo));

        std::array<wchar_t, 2048> imageName{}, moduleName{};
        GetModuleFileNameExW(processHandle, moduleHandle, &imageName[0], sizeof(imageName));
        GetModuleBaseNameW(processHandle, moduleHandle, &moduleName[0], sizeof(moduleName));

        SymLoadModuleExW(processHandle, nullptr, &imageName[0], &moduleName[0],
            reinterpret_cast<DWORD64>(moduleInfo.lpBaseOfDll), moduleInfo.SizeOfImage, nullptr, 0);

        baseModuleAddress = moduleInfo.lpBaseOfDll;
    }

    std::array<wchar_t, 2048> undecoratedName{};
    DWORD64 displacement = 0;
    DWORD offsetFromSymbol = 0;
    PIMAGE_NT_HEADERS image = ImageNtHeader(baseModuleAddress);

    auto walkStack = [&]()
    {
        return StackWalk64(image->FileHeader.Machine, processHandle, threadHandle, &frame, context, nullptr,
            SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
    };

    // Some stack trace lines are unnecessary, so we will discard them.
    int32_t stackIgnoreCount = 3;
    while (stackIgnoreCount--)
        walkStack();

    do
    {
        // Write the current stack metadata into the buffer.
        auto symbolBytes = std::array<std::byte, sizeof(SYMBOL_INFOW) + 1024>{};
        auto* si = reinterpret_cast<SYMBOL_INFOW*>(symbolBytes.data());
        si->SizeOfStruct = sizeof(SYMBOL_INFOW);
        si->MaxNameLen = static_cast<DWORD>(undecoratedName.size());
        SymFromAddrW(processHandle, frame.AddrPC.Offset, &displacement, si);

        // Get the file name and the line.
        auto line = IMAGEHLP_LINEW64{};
        line.SizeOfStruct = sizeof(line);

        auto isLineParsed = !!SymGetLineFromAddrW64(processHandle, frame.AddrPC.Offset, &offsetFromSymbol, &line);
        if (isLineParsed)
            fmt::format_to(std::back_inserter(destStr), L"   at {} in {}:line {}\n", si->Name, line.FileName, line.LineNumber);
        else
            fmt::format_to(std::back_inserter(destStr), L"   at {}\n", si->Name);

        walkStack();

    } while (frame.AddrReturn.Offset != 0);

    if (!destStr.empty() && destStr.back() == u'\n')
        destStr.pop_back();

    SymCleanup(processHandle);

    return EXCEPTION_EXECUTE_HANDLER;
}

CS2CPP_NOINLINE void InternalGetStackTrace(std::u16string& destStr)
{
    __try
    {
        CS2CPP_THROW_SEH_EXCEPTION();
    }
    __except (InternalGetStackTrace(GetExceptionInformation(), destStr))
    {
    }
}

}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    return !!SetEnvironmentVariableW(
        reinterpret_cast<LPCWSTR>(name.data()),
        reinterpret_cast<LPCWSTR>(value.data()));
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target)
{
    if (!IsValidEnvironmentVariableName(name))
        return false;

    if (target == EnvironmentVariableTarget::Process)
        return SetEnvironmentVariable(name, value);

    if (target == EnvironmentVariableTarget::Machine)
    {
        if (name.length() >= MaxSystemEnvVariableLength)
            return false;

        constexpr auto subKey = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
        return detail::environment::InternalSetEnvironmentVariable(HKEY_LOCAL_MACHINE, subKey, name, value);
    }

    if (name.length() >= MaxUserEnvVariableLength)
        return false;

    return detail::environment::InternalSetEnvironmentVariable(HKEY_CURRENT_USER, L"Environment", name, value);
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    auto length = GetEnvironmentVariableW(reinterpret_cast<LPCWSTR>(name.data()), nullptr, 0);
    if (length == 0)
        return {};

    std::u16string ret;
    ret.resize(length);

    if (GetEnvironmentVariableW(reinterpret_cast<LPCWSTR>(name.data()), reinterpret_cast<LPWSTR>(ret.data()), length) == 0)
        return {};

    return std::optional<std::u16string>(std::move(ret));
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
        return GetEnvironmentVariable(name);

    if (target == EnvironmentVariableTarget::Machine)
        return detail::environment::InternalGetEnvironmentVariable(HKEY_LOCAL_MACHINE,
            L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", name);

    return detail::environment::InternalGetEnvironmentVariable(HKEY_CURRENT_USER, L"Environment", name);
}

std::u16string Environment::GetFolderPath(SpecialFolder folder)
{
    std::array<wchar_t, 8192> path{};
    if (SHGetFolderPathW(nullptr, static_cast<int>(folder), nullptr, 0, path.data()) != S_OK)
        return {};

    return reinterpret_cast<const char16_t*>(path.data());
}

const std::u16string& Environment::GetCommandLine()
{
    static std::u16string commandLine(reinterpret_cast<const char16_t*>(GetCommandLineW()));
    return commandLine;
}

int64_t Environment::GetTickCount()
{
    return static_cast<int64_t>(::GetTickCount64());
}

bool Environment::Is64BitProcess()
{
#if defined(_WIN64)
    return true;
#else
    return false;
#endif
}

bool Environment::Is64BitOperatingSystem()
{
#if defined(_WIN64)
    // 64-bit programs run only on 64-bit
    return true;
#else
    BOOL isWow64 = FALSE;
    return IsWow64Process(GetCurrentProcess(), &isWow64) && isWow64;
#endif
}

void Environment::FailFast(std::u16string_view message)
{
    OutputDebugStringW(L"FailFast:\n");
    OutputDebugStringW(reinterpret_cast<LPCWSTR>(message.data()));

    std::terminate();
}

int32_t Environment::GetSystemPageSize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    return static_cast<int32_t>(si.dwPageSize);
}

int32_t Environment::GetCurrentManagedThreadId()
{
    return static_cast<int32_t>(GetCurrentThreadId());
}

std::u16string Environment::GetUserName()
{
    std::array<wchar_t, 4096> name{};
    auto nameLen = static_cast<DWORD>(name.size());
    if (GetUserNameW(name.data(), &nameLen) == FALSE)
        return {};

    return {reinterpret_cast<const char16_t*>(name.data()), nameLen};
}

std::u16string Environment::GetMachineName()
{
    std::array<wchar_t, 4096> name{};
    auto nameLen = static_cast<DWORD>(name.size());
    if (GetComputerNameW(name.data(), &nameLen) == FALSE)
        return {};

    return {reinterpret_cast<const char16_t*>(name.data()), nameLen};
}

std::u16string Environment::GetUserDomainName()
{
    std::array<wchar_t, 4096> name{};
    auto nameLen = static_cast<DWORD>(name.size());
    if (GetUserNameExW(NameSamCompatible, name.data(), &nameLen) == 0)
        return {};

    auto str = wcschr(name.data(), L'\\');
    if (!str)
        return {};

    return std::u16string(reinterpret_cast<const char16_t*>(name.data()), str - name.data());
}

CS2CPP_NOINLINE std::u16string Environment::GetStackTrace()
{
    using namespace detail::environment;

    std::u16string str;
    InternalGetStackTrace(str);

    return str;
}

std::map<std::u16string, std::u16string> Environment::GetEnvironmentVariables()
{
    auto strings = std::unique_ptr<WCHAR[], decltype(&FreeEnvironmentStringsW)>(GetEnvironmentStringsW(),
        FreeEnvironmentStringsW);
    std::map<std::u16string, std::u16string> ret;

    for (size_t i = 0;; ++i)
    {
        if (strings[i] == L'\0' && strings[i + 1] == L'\0')
            break;

        auto startKey = i;
        while (strings[i] != L'=' && strings[i] != L'\0')
            ++i;

        if (strings[i] == L'\0')
            continue;

        if (i - startKey == 0)
        {
            while (strings[i] != 0)
                i++;
            continue;
        }

        auto key = std::u16string(reinterpret_cast<const char16_t*>(&strings[startKey]), i - startKey);
        auto startValue = ++i;
        while (strings[i] != L'\0')
            ++i;

        ret[std::move(key)] = std::u16string(reinterpret_cast<const char16_t*>(&strings[startValue]), i - startValue);
    }

    return ret;
}

bool Environment::GetUserInteractive()
{
    static auto cachedWindowStationHandle = HWINSTA(nullptr);
    static auto isUserNonInteractive = false;

    auto windowStationHandle = GetProcessWindowStation();
    if (windowStationHandle && windowStationHandle != cachedWindowStationHandle)
    {
        USEROBJECTFLAGS flags;
        DWORD lengthNeeded;
        if (GetUserObjectInformationW(windowStationHandle, UOI_FLAGS, reinterpret_cast<PVOID>(&flags), sizeof(flags),
            &lengthNeeded) == TRUE)
        {
            if ((flags.dwFlags & WSF_VISIBLE) == 0)
                isUserNonInteractive = true;
        }

        cachedWindowStationHandle = windowStationHandle;
    }

    return !isUserNonInteractive;
}

int64_t Environment::GetWorkingSet()
{
    auto processHandle = SafeHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId()));

    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(processHandle, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc)) == TRUE)
        return pmc.WorkingSetSize;

    return 0;
}

std::optional<OperatingSystem> Environment::GetOSVersion()
{
    auto vi = OSVERSIONINFOEX{};
    vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    if (GetVersionExW(reinterpret_cast<OSVERSIONINFO*>(&vi)) == FALSE)
        return {};

    Version version(static_cast<int32_t>(vi.dwMajorVersion), static_cast<int32_t>(vi.dwMinorVersion),
        static_cast<int32_t>(vi.dwBuildNumber), (vi.wServicePackMajor << 16) | vi.wServicePackMinor);

    return OperatingSystem(PlatformID::Win32NT, version, reinterpret_cast<const char16_t*>(&vi.szCSDVersion[0]));
}

CS2CPP_NAMESPACE_END

#undef CS2CPP_THROW_SEH_EXCEPTION