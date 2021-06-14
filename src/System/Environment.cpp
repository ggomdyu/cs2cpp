#include <thread>
#include <sstream>
#include <string>

#include "System/Environment.h"
#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Environment::GetCurrentDirectory()
{
    return Directory::GetCurrentDirectory();
}

void Environment::Exit(int32_t exitCode)
{
    exit(exitCode);
}

int32_t Environment::GetProcessorCount()
{
    return static_cast<int32_t>(std::thread::hardware_concurrency());
}

std::u16string Environment::GetSystemDirectory()
{
    return GetFolderPath(SpecialFolder::System);
}

//todo: fix
//const std::vector<std::u16string>& Environment::GetCommandLineArgs()
//{
//    static auto commandLineArgs = []()
//    {
//        std::vector<std::u16string> ret;
//
//        std::basic_stringstream<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>> ss(GetCommandLine());
//        std::u16string commandLineArg;
//        while (std::getline(ss, commandLineArg, u' '))
//        {
//            ret.push_back(std::move(commandLineArg));
//        }
//
//        return ret;
//    } ();
//
//    return commandLineArgs;
//}

std::vector<std::u16string> Environment::GetLogicalDrives()
{
    return Directory::GetLogicalDrives();
}

CS2CPP_NAMESPACE_END
