#include <thread>

#include "System/Environment.h"

CS2CPP_NAMESPACE_BEGIN

namespace
{
    
std::u16string InternalGetCommandLine()
{
    std::u16string ret;
    for (std::u16string_view arg : Environment::GetCommandLineArgs())
    {
        ret += arg;
        ret += u' ';
    }

    if (!ret.empty())
    {
        ret.pop_back();
    }

    return ret;
}

}

std::u16string_view Environment::CommandLine()
{
    static auto commandLine = InternalGetCommandLine();
    return commandLine;
}

void Environment::Exit(int32_t exitCode)
{
    exit(exitCode);
}

bool Environment::Is64BitProcess()
{
    return sizeof(intptr_t) == 8;
}

bool Environment::Is64BitOperatingSystem()
{
    return Is64BitProcess() || Is64BitOperatingSystemWhen32BitProcess();
}

int32_t Environment::ProcessorCount()
{
    return static_cast<int32_t>(std::thread::hardware_concurrency());
}

std::u16string Environment::SystemDirectory()
{
    return GetFolderPath(SpecialFolder::System);
}

int32_t Environment::TickCount()
{
    return static_cast<int32_t>(TickCount64());
}

PlatformID Environment::GetPlatform() noexcept
{
#if CS2CPP_PLATFORM_WINDOWS
    return PlatformID::Win32NT;
#elif CS2CPP_PLATFORM_DARWIN
    return PlatformID::MacOSX;
#elif CS2CPP_PLATFORM_XBOX
    return PlatformID::Xbox;
#else
    return PlatformID::Unix;
#endif
}

CS2CPP_NAMESPACE_END
