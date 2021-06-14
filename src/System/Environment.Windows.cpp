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

thread_local std::vector<wchar_t> GlobalWideCharBuffer(16384);

namespace detail::environment
{

bool InternalSetEnvironmentVariable(HKEY key, LPCWSTR subKey, std::u16string_view name, std::u16string_view value)
{
    HKEY rawKeyHandle;
    if (RegOpenKeyExW(key, subKey, 0, KEY_READ | KEY_WRITE, &rawKeyHandle) != ERROR_SUCCESS)
        return false;

    SafeRegistryHandle keyHandle(rawKeyHandle);

    auto wideCharName = reinterpret_cast<const wchar_t*>(name.data());
    if (!value.empty())
    {
        if (RegSetValueExW(keyHandle, wideCharName, 0, REG_SZ, reinterpret_cast<const BYTE*>(value.data()),
            value.length() + 1) != ERROR_SUCCESS)
            return false;
    }
    else
    {
        if (RegDeleteValueW(keyHandle, wideCharName) != ERROR_SUCCESS)
            return false;
    }

    return true;
}

std::optional<String> InternalGetEnvironmentVariable(HKEY predefinedKey, LPCWSTR subKey, std::u16string_view name)
{
    SafeRegistryHandle keyHandle;
    if (RegOpenKeyExW(predefinedKey, subKey, 0, KEY_READ, keyHandle) != ERROR_SUCCESS)
        return {};

    std::array<char16_t, 2048> tempData{};
    DWORD tempCbData = tempData.size();
    DWORD tempType = 0;

    if (RegQueryValueExW(keyHandle, reinterpret_cast<const wchar_t*>(name.data()), nullptr, &tempType,
        reinterpret_cast<LPBYTE>(tempData.data()), &tempCbData) != ERROR_SUCCESS)
        return {};

    return String(tempData.data());
}

std::vector<HMODULE> GetAllProcessModule(HANDLE processHandle)
{
    DWORD tempModuleByteCount = 0;
    if (EnumProcessModules(processHandle, nullptr, 0, &tempModuleByteCount) == 0)
        return {};

    std::vector<HMODULE> moduleHandles(tempModuleByteCount / sizeof(HMODULE));
    if (EnumProcessModules(processHandle, &moduleHandles[0], moduleHandles.size() * sizeof(HMODULE),
        &tempModuleByteCount) == 0)
        return {};

    return moduleHandles;
}

DWORD InternalGetStackTrace(EXCEPTION_POINTERS* ep, wchar_t* destStr, int32_t* destStrLen)
{
    HANDLE threadHandle = GetCurrentThread();
    HANDLE processHandle = GetCurrentProcess();

    if (SymInitialize(processHandle, nullptr, false) == FALSE)
    {
        *destStrLen = 0;
        return EXCEPTION_EXECUTE_HANDLER;
    }

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

    auto moduleHandles = detail::environment::GetAllProcessModule(processHandle);

    void* baseModuleAddress = nullptr;
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
    const auto* image = ImageNtHeader(baseModuleAddress);

    *destStrLen = 0;

    int32_t stackIgnoreCount = 3;
    do
    {
        if (frame.AddrPC.Offset != 0)
        {
            std::array<std::byte, sizeof(SYMBOL_INFOW) + 1024> symbolBytes{};

            auto* si = reinterpret_cast<SYMBOL_INFOW*>(symbolBytes.data());
            si->SizeOfStruct = sizeof(SYMBOL_INFOW);
            si->MaxNameLen = static_cast<DWORD>(undecoratedName.size());
            SymFromAddrW(processHandle, frame.AddrPC.Offset, &displacement, si);

            // Get the file name and line.
            IMAGEHLP_LINEW64 line{};
            line.SizeOfStruct = sizeof(line);
            SymGetLineFromAddrW64(processHandle, frame.AddrPC.Offset, &offsetFromSymbol, &line);

            // Write the current stack metadata into the buffer.
            if (stackIgnoreCount == 0)
                *destStrLen += wsprintf(reinterpret_cast<wchar_t*>(destStr) + *destStrLen,
                    L"   at %s in %s:line %d\n", si->Name, line.FileName, line.LineNumber);
            else
                --stackIgnoreCount;
        }

        if (StackWalk64(image->FileHeader.Machine, processHandle, threadHandle, &frame, context, nullptr,
            SymFunctionTableAccess64, SymGetModuleBase64, nullptr) == FALSE)
            break;
    } while (frame.AddrReturn.Offset != 0);

    SymCleanup(processHandle);

    return EXCEPTION_EXECUTE_HANDLER;
}

int32_t InternalGetStackTrace(wchar_t* destStr)
{
    int32_t destStrLen = 0;
    __try
    {
        CS2CPP_THROW_SEH_EXCEPTION();
    }
    __except (InternalGetStackTrace(GetExceptionInformation(), destStr, &destStrLen))
    {
    }

    return destStrLen;
}

}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    return !!SetEnvironmentVariableW(
        reinterpret_cast<const wchar_t*>(name.data()),
        reinterpret_cast<const wchar_t*>(value.data()));
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

std::optional<String> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    auto wideCharName = reinterpret_cast<const wchar_t*>(name.data());
    auto length = GetEnvironmentVariableW(wideCharName, nullptr, 0);
    if (length == 0)
        return {};

    std::vector<wchar_t> str(length);
    if (GetEnvironmentVariableW(wideCharName, str.data(), length) == 0)
        return {};

    return String(reinterpret_cast<const char16_t*>(str.data()), length);
}

std::optional<String> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target == EnvironmentVariableTarget::Process)
        return GetEnvironmentVariable(name);

    if (target == EnvironmentVariableTarget::Machine)
        return detail::environment::InternalGetEnvironmentVariable(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", name);

    return detail::environment::InternalGetEnvironmentVariable(HKEY_CURRENT_USER, L"Environment", name);
}

String Environment::GetFolderPath(SpecialFolder folder)
{
    std::array<wchar_t, 2048> wideCharPath{};

    if (SHGetFolderPathW(nullptr, static_cast<int>(folder), nullptr, 0, wideCharPath.data()) != S_OK)
        return String::Empty;

    return String(reinterpret_cast<const char16_t*>(wideCharPath.data()));
}

const String& Environment::GetCommandLine()
{
    static String commandLine(reinterpret_cast<const char16_t*>(GetCommandLineW()));
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
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(message.data()));

    std::terminate();
}

int32_t Environment::GetSystemPageSize()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    return si.dwPageSize;
}

int32_t Environment::GetCurrentManagedThreadId()
{
    return GetCurrentThreadId();
}

String Environment::GetUserName()
{
    std::array<wchar_t, 2048> name{};
    DWORD nameLen = name.size();

    if (GetUserNameW(name.data(), &nameLen) == FALSE)
        return String::Empty;

    return String(reinterpret_cast<const char16_t*>(name.data()), nameLen);
}

String Environment::GetMachineName()
{
    std::array<wchar_t, 2048> name{};
    DWORD nameLen = name.size();

    if (GetComputerNameW(name.data(), &nameLen) == FALSE)
        return String::Empty;

    return String(reinterpret_cast<const char16_t*>(name.data()), nameLen);
}

String Environment::GetUserDomainName()
{
    std::array<wchar_t, 2048> name{};
    DWORD nameLen = name.size();

    if (GetUserNameExW(NameSamCompatible, name.data(), &nameLen) == 0)
        return String::Empty;

    auto str = wcschr(name.data(), L'\\');
    if (!str)
        return String::Empty;

    return String(reinterpret_cast<const char16_t*>(name.data()), static_cast<int32_t>(str - name.data()));
}

String Environment::GetStackTrace()
{
    auto str = reinterpret_cast<const char16_t*>(GlobalWideCharBuffer.data());
    auto strLen = detail::environment::InternalGetStackTrace(GlobalWideCharBuffer.data());

    return String(str, strLen);
}

std::map<String, String> Environment::GetEnvironmentVariables()
{
    auto strings = std::unique_ptr<WCHAR[], decltype(&FreeEnvironmentStringsW)>(GetEnvironmentStringsW(),
        FreeEnvironmentStringsW);
    std::map<String, String> ret;

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

        auto key = String(reinterpret_cast<const char16_t*>(&strings[startKey]), i - startKey);
        auto startValue = ++i;
        while (strings[i] != L'\0')
            ++i;

        ret[std::move(key)] = String(reinterpret_cast<const char16_t*>(&strings[startValue]), i - startValue);
    }

    return ret;
}

OperatingSystem Environment::GetOSVersion()
{
    OSVERSIONINFOEX vi {
        vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX)
    };
    GetVersionExW(reinterpret_cast<OSVERSIONINFO*>(&vi));

    Version version(vi.dwMajorVersion, vi.dwMinorVersion, vi.dwBuildNumber,
        (vi.wServicePackMajor << 16) | vi.wServicePackMinor);

    return OperatingSystem(PlatformID::Win32NT, version, reinterpret_cast<const char16_t*>(&vi.szCSDVersion[0]));
}

bool Environment::GetUserInteractive()
{
    static auto cachedWindowStationHandle = HWINSTA(nullptr);
    static auto isUserNonInteractive = false;

    auto windowStationHandle = GetProcessWindowStation();
    if (windowStationHandle && windowStationHandle != cachedWindowStationHandle)
    {
        USEROBJECTFLAGS flags;
        DWORD needLength;
        if (GetUserObjectInformationW(windowStationHandle, UOI_FLAGS, reinterpret_cast<PVOID>(&flags), sizeof(flags), &needLength) == TRUE)
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

CS2CPP_NAMESPACE_END

#undef CS2CPP_THROW_SEH_EXCEPTION