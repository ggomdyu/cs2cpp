#include <array>
#include <execinfo.h>
#include <memory>
#include <pwd.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "System/Diagnostics/Debug.h"
#include "System/Environment.h"
#include "System/Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::environment
{

std::optional<passwd> GetNativeUserInfo()
{
    passwd pwd{};

    passwd* tempPwd;
    std::array<char, 1024> tempBuf{};
    if (getpwuid_r(getuid(), &pwd, tempBuf.data(), tempBuf.size(), &tempPwd) != 0 || tempPwd == nullptr)
    {
        return {};
    }

    return pwd;
}

}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    std::array<std::byte, 2048> utf8Name{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(name.data()), sizeof(name[0]) * name.length()}, utf8Name))
    {
        return false;
    }

    std::array<std::byte, 2048> utf8Value{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(value.data()), sizeof(value[0]) * value.length()}, utf8Value))
    {
        return false;
    }

    return setenv(reinterpret_cast<const char*>(utf8Name.data()),
        reinterpret_cast<const char*>(utf8Value.data()), true) == 0;
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
    {
        return false;
    }

    return SetEnvironmentVariable(name, value);
}

std::optional<String> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    std::array<std::byte, 2048> utf8Name{};
    if (!Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(name.data()), name.size() * sizeof(name[0])}, utf8Name))
    {
        return {};
    }

    const char* envValue = getenv(reinterpret_cast<const char*>(utf8Name.data()));
    if (envValue == nullptr)
    {
        return {};
    }

    return Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(envValue), strlen(envValue));
}

std::optional<String> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
    {
        return {};
    }

    return GetEnvironmentVariable(name);
}

String Environment::GetUserName()
{
    auto userInfo = detail::environment::GetNativeUserInfo();
    if (!userInfo)
    {
        return {};
    }

    auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(userInfo->pw_name),
        strlen(userInfo->pw_name));
    if (!utf16Str)
    {
        return {};
    }

    return std::move(utf16Str.value());
}

String Environment::GetMachineName()
{
    utsname buf{};
    if (uname(&buf) != 0)
    {
        return {};
    }

    auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(buf.nodename),
        static_cast<int32_t>(strlen(buf.nodename)));
    if (!utf16Str)
    {
        return {};
    }

    return std::move(utf16Str.value());
}

String Environment::GetUserDomainName()
{
    return GetMachineName();
}

bool Environment::Is64BitProcess()
{
    return sizeof(intptr_t) == 8;
}

bool Environment::Is64BitOperatingSystem()
{
    return Is64BitProcess();
}

int32_t Environment::GetSystemPageSize()
{
    return getpagesize();
}

void Environment::FailFast(std::u16string_view message)
{
    Debug::Write(String::Format(u"FailFast:\n{}", message));
    assert(false);
}

String Environment::GetStackTrace()
{
    std::array<void*, 2048> addr{};
    const auto frameCount = backtrace(addr.data(), addr.size());
    if (frameCount <= 0)
    {
        return {};
    }

    String stackTrace;
    auto utf8Symbols = std::unique_ptr<char*, decltype(&std::free)>(
        backtrace_symbols(addr.data(), frameCount), &std::free);
    for (int i = 0; i < frameCount; ++i)
    {
        auto utf16Symbol = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8Symbols.get()[i]),
            strlen(utf8Symbols.get()[i]));
        if (!utf16Symbol)
        {
            continue;
        }

        stackTrace += String::Format(u"{}\n", utf16Symbol.value());
    }

    return stackTrace;
}

CS2CPP_NAMESPACE_END