#include <thread>

#include "System/Environment.h"
#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

String Environment::GetCurrentDirectory()
{
    return Directory::GetCurrentDirectory();
}

void Environment::Exit(int32_t exitCode)
{
    std::exit(exitCode);
}

int32_t Environment::GetProcessorCount()
{
    return static_cast<int32_t>(std::thread::hardware_concurrency());
}

String Environment::GetSystemDirectory()
{
    return GetFolderPath(SpecialFolder::System);
}

const std::vector<String>& Environment::GetCommandLineArgs()
{
    static auto commandLineArgs = GetCommandLine().Split(u' ');
    return commandLineArgs;
}

std::vector<String> Environment::GetLogicalDrives()
{
    return Directory::GetLogicalDrives();
}

CS2CPP_NAMESPACE_END