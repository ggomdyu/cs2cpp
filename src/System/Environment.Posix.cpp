#include <algorithm>
#include <array>
#include <execinfo.h>
#include <fmt/format.h>
#include <memory>
#include <pwd.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "System/Diagnostics/Debug.h"
#include "System/Environment.h"
#include "System/Text/Encoding.h"

CS2CPP_NAMESPACE_BEGIN

namespace
{

std::optional<passwd> GetNativeUserInfo()
{
    passwd pwd{};
    passwd* tempPwd;
    std::array<char, 1024> tempBuf{};

    if (getpwuid_r(getuid(), &pwd, tempBuf.data(), tempBuf.size(), &tempPwd) != 0 || tempPwd == nullptr)
        return {};

    return pwd;
}

}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    auto utf8Name = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(name.data()), sizeof(name[0]) * name.length()});
    if (!utf8Name)
        return false;

    auto utf8Value = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(value.data()), sizeof(value[0]) * value.length()});
    if (!utf8Value)
        return false;

    return setenv(reinterpret_cast<const char*>(utf8Name->data()), reinterpret_cast<const char*>(utf8Value->data()),
        true) == 0;
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
        return false;

    return SetEnvironmentVariable(name, value);
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    auto utf8Name = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(name.data()), sizeof(name[0]) * name.length()});
    if (!utf8Name)
        return {};

    auto envValue = getenv(reinterpret_cast<const char*>(utf8Name->data()));
    if (!envValue)
        return {};

    return Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(envValue), strlen(envValue));
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
        return {};

    return GetEnvironmentVariable(name);
}

std::u16string Environment::GetUserName()
{
    auto userInfo = GetNativeUserInfo();
    if (!userInfo)
        return {};

    auto utf16Str = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(userInfo->pw_name),
        strlen(userInfo->pw_name));
    if (!utf16Str)
        return {};

    return std::move(utf16Str.value());
}

std::u16string Environment::GetMachineName()
{
    utsname buf{};
    if (uname(&buf) != 0)
        return {};

    auto utf16Value = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(buf.nodename),
        static_cast<int32_t>(strlen(buf.nodename)));
    if (!utf16Value)
        return {};

    return std::move(utf16Value.value());
}

std::u16string Environment::GetUserDomainName()
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
    Debug::Write(fmt::format(u"FailFast:\n{}", message));
    assert(false);
}

std::u16string Environment::GetStackTrace()
{
    auto addr = std::array<void*, 2048>{};
    auto frameCount = backtrace(addr.data(), addr.size());
    if (frameCount <= 0)
        return {};

    std::u16string stackTrace;
    auto utf8Symbols = std::unique_ptr<char*, decltype(&std::free)>(backtrace_symbols(addr.data(), frameCount),
        &std::free);
  
    for (int i = 0; i < frameCount; ++i)
    {
        auto utf8Symbol = utf8Symbols.get()[i];
        auto utf16Symbol = Encoding::UTF8().GetString(reinterpret_cast<const std::byte*>(utf8Symbol), strlen(utf8Symbol));
        if (!utf16Symbol)
            continue;

        fmt::format_to(std::back_inserter(stackTrace), u"{}\n", *utf16Symbol);
    }

    return stackTrace;
}

CS2CPP_NAMESPACE_END
