#include <array>
#include <execinfo.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <unistd.h>
#if CS2CPP_PLATFORM_DARWIN
#    include <crt_externs.h>
#    include <sys/mount.h>
#else
#    include <fcntl.h>
#endif

#include "System.IO/File.h"
#include "System.IO/Path.h"
#include "System.Text/Encoding.h"
#include "System/Console.h"
#include "System/Environment.h"

CS2CPP_NAMESPACE_BEGIN

thread_local std::u16string NullTerminatedStr;

static std::u16string InternalGetHomeDirectory()
{
    struct passwd pw{};
    struct passwd* result = nullptr;
    std::array<char, 2048> buffer{};
    if (getpwuid_r(getuid(), &pw, buffer.data(), buffer.size(), &result) != 0 || !result)
    {
        return {};
    }

    auto bytes = ReadOnlySpan(reinterpret_cast<const std::byte*>(pw.pw_dir), static_cast<int32_t>(strlen(pw.pw_dir)));
    return Encoding::UTF8().GetString(bytes).value_or(std::u16string());
}

static std::u16string InternalReadXdgUserDir(std::u16string_view key, std::u16string_view fallback)
{
    return Environment::GetEnvironmentVariable(key).value_or(Path::Combine(InternalGetHomeDirectory(), fallback));
}

static std::u16string InternalCreateOSVersionString()
{
    utsname name{};
    if (uname(&name) == -1)
    {
        return {};
    }

    auto bytes = ReadOnlySpan(reinterpret_cast<const std::byte*>(name.release), static_cast<int32_t>(strlen(name.release)));
    return Encoding::UTF8().GetString(bytes).value_or(std::u16string());
}

#if CS2CPP_PLATFORM_DARWIN
static std::vector<std::u16string> InternalGetLogicalDrives()
{
    std::vector<std::u16string> logicalDrives;
    struct statfs* m = nullptr;
    for (auto i = 0; i < getmntinfo(&m, MNT_WAIT); ++i)
    {
        auto logicalDrive = Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(m[i].f_mntonname),
            static_cast<int32_t>(strnlen(m[i].f_mntonname, MAXPATHLEN))});
        if (!logicalDrive)
        {
            continue;
        }

        logicalDrives.push_back(std::move(logicalDrive.value()));
    }

    return logicalDrives;
}
#else
static std::vector<std::u16string> InternalGetLogicalDrives(std::u16string_view text, int32_t tokenNum)
{
    std::vector<std::u16string> ret;

    int32_t tempTokenNum = tokenNum;
    while (!text.empty())
    {
        auto index = text.find(' ');
        if (index == std::string::npos)
        {
            break;
        }

        // If the nth token is found, then move to the next line.
        if (tempTokenNum-- == 0)
        {
            ret.emplace_back(text.substr(0, index));

            tempTokenNum = tokenNum;
            text = text.substr(text.find('\n') + 1);
            continue;
        }

        // Move to the next token.
        text = text.substr(index + 1);
    }

    return ret;
}
#endif

#if CS2CPP_PLATFORM_LINUX
static std::vector<std::u16string> InternalGetCommandLineArgs()
{
    auto bytes = File::ReadAllBytes(u"/proc/self/cmdline");

    return {};
}
#else
static std::vector<std::u16string> InternalGetCommandLineArgs()
{
    return {};
}
#endif

int32_t Environment::CurrentManagedThreadId()
{
#if CS2CPP_PLATFORM_DARWIN
    uint64_t tid;
    pthread_threadid_np(pthread_self(), &tid);
    return static_cast<int32_t>(tid);
#else
    return gettid();
#endif
}

void Environment::FailFast(std::u16string_view message)
{
    std::u16string fullMessage;
    fullMessage.reserve(15 + message.length());
    fullMessage += u"FailFast:\n";
    fullMessage += message;

    Console::WriteLine(fullMessage);
    assert(false);
}

const std::vector<std::u16string>& Environment::GetCommandLineArgs()
{
    static auto ret = InternalGetCommandLineArgs();
    return ret;
}


std::u16string Environment::GetCurrentDirectory()
{
    std::unique_ptr<char> utf8Path(getcwd(nullptr, 0));
    if (!utf8Path)
    {
        return {};
    }

    return Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(utf8Path.get()), static_cast<int32_t>(strlen(utf8Path.get()))}).value_or(std::u16string());
}


std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name)
{
    auto utf8Bytes = Encoding::UTF8().GetBytes(name);
    if (!utf8Bytes)
    {
        return std::nullopt;
    }

    auto envValue = getenv(reinterpret_cast<const char*>(utf8Bytes.value().data()));
    if (!envValue)
    {
        return std::nullopt;
    }

    return Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(envValue), static_cast<int32_t>(strlen(envValue))});
}

std::optional<std::u16string> Environment::GetEnvironmentVariable(std::u16string_view name, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
    {
        return std::nullopt;
    }

    return GetEnvironmentVariable(name);
}

std::map<std::u16string, std::u16string> Environment::GetEnvironmentVariables()
{
    std::map<std::u16string, std::u16string> ret;

#if CS2CPP_PLATFORM_DARWIN
    auto env = *_NSGetEnviron();
#else
    auto env = environ;
#endif

    for (; *env != nullptr; ++env)
    {
        auto environStr = std::string_view(*env);
        auto separatorIdx = environStr.find('=');
        if (separatorIdx == std::string_view::npos)
        {
            continue;
        }

        auto utf8Key = environStr.substr(0, separatorIdx);
        auto utf16Key = Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(utf8Key.data()), static_cast<int32_t>(utf8Key.length())});
        if (!utf16Key)
        {
            continue;
        }

        auto utf8Value = environStr.substr(separatorIdx + 1);
        auto utf16Value = Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(utf8Value.data()), static_cast<int32_t>(utf8Value.length())});
        if (!utf16Value)
        {
            continue;
        }

        ret[std::move(*utf16Key)] = std::move(*utf16Value);
    }

    return ret;
}

std::map<std::u16string, std::u16string> Environment::GetEnvironmentVariables(EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
    {
        return {};
    }

    return GetEnvironmentVariables();
}

std::u16string Environment::GetFolderPath(SpecialFolder folder)
{
    switch (folder)
    {
    case SpecialFolder::ApplicationData:
        return InternalReadXdgUserDir(u"XDG_CONFIG_HOME", u".config");
    case SpecialFolder::CommonApplicationData:
        return u"/usr/share";
    case SpecialFolder::CommonTemplates:
        return u"/usr/share/templates";
    case SpecialFolder::Desktop:
    case SpecialFolder::DesktopDirectory:
        return InternalReadXdgUserDir(u"XDG_DESKTOP_DIR", u"Desktop");
    case SpecialFolder::LocalApplicationData:
        return InternalReadXdgUserDir(u"XDG_DATA_HOME", u".local/share");
    case SpecialFolder::MyVideos:
        return InternalReadXdgUserDir(u"XDG_VIDEOS_DIR", u"Videos");
    case SpecialFolder::Templates:
        return InternalReadXdgUserDir(u"XDG_TEMPLATES_DIR", u"Templates");
    case SpecialFolder::Personal:
    case SpecialFolder::UserProfile:
        return InternalGetHomeDirectory();

#if CS2CPP_PLATFORM_DARWIN
    case SpecialFolder::Favorites:
        return Path::Combine(InternalGetHomeDirectory(), u"Library/Favorites");
    case SpecialFolder::Fonts:
        return Path::Combine(InternalGetHomeDirectory(), u"Library/Fonts");
    case SpecialFolder::InternetCache:
        return Path::Combine(InternalGetHomeDirectory(), u"Library/Caches");
    case SpecialFolder::MyMusic:
        return Path::Combine(InternalGetHomeDirectory(), u"Music");
    case SpecialFolder::MyPictures:
        return Path::Combine(InternalGetHomeDirectory(), u"Pictures");
    case SpecialFolder::ProgramFiles:
        return u"/Applications";
#else
    case SpecialFolder::Fonts:
        return u"/usr/share/fonts";
    case SpecialFolder::MyMusic:
        return InternalReadXdgUserDir(u"XDG_MUSIC_DIR", u"Music");
    case SpecialFolder::MyPictures:
        return InternalReadXdgUserDir(u"XDG_PICTURES_DIR", u"Pictures");
#endif

    default:
        return {};
    }
}

std::vector<std::u16string> Environment::GetLogicalDrives()
{
#if CS2CPP_PLATFORM_DARWIN
    return InternalGetLogicalDrives();
#else
    auto m = File::ReadAllText(u"/proc/self/mountinfo");
    if (m && !m->empty())
    {
        return InternalGetLogicalDrives(*m, 4);
    }

    m = File::ReadAllText(u"/proc/mounts");
    if (m && !m->empty())
    {
        return InternalGetLogicalDrives(*m, 1);
    }

    return {};
#endif
}

std::u16string Environment::MachineName()
{
    utsname buf{};
    if (uname(&buf) != 0)
    {
        return {};
    }

#ifdef CS2CPP_PLATFORM_DARWIN
    constexpr size_t SysNameLen = _SYS_NAMELEN;
#else
    constexpr size_t SysNameLen = SYS_NMLN;
#endif

    return Encoding::UTF8().GetString({reinterpret_cast<const std::byte*>(buf.nodename), static_cast<int32_t>(strnlen(buf.nodename, SysNameLen))}).value_or(std::u16string());
}

std::optional<OperatingSystem> Environment::OSVersion()
{
    auto platform = GetPlatform();
    if (platform == PlatformID::MacOSX)
    {
        platform = PlatformID::Unix;
    }

    auto version = Version::Parse(InternalCreateOSVersionString()).value_or(Version(0, 0));
    return OperatingSystem(platform, version, {});
}

bool Environment::SetCurrentDirectory(std::u16string_view path)
{
    auto utf8Bytes = Encoding::UTF8().GetBytes(path);
    if (!utf8Bytes)
    {
        return false;
    }

    return chdir(reinterpret_cast<const char*>(utf8Bytes->data())) == 0;

}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value)
{
    auto utf8Name = Encoding::UTF8().GetBytes(name);
    if (!utf8Name)
    {
        return false;
    }

    // If the value is empty, the environment variable will be removed.
    if (value.empty())
    {
        return unsetenv(reinterpret_cast<const char*>(utf8Name->data())) == 0;
    }

    auto utf8Value = Encoding::UTF8().GetBytes(value);
    if (!utf8Value)
    {
        return false;
    }

    return setenv(reinterpret_cast<const char*>(utf8Name->data()), reinterpret_cast<const char*>(utf8Value->data()), true) == 0;
}

bool Environment::SetEnvironmentVariable(std::u16string_view name, std::u16string_view value, EnvironmentVariableTarget target)
{
    if (target != EnvironmentVariableTarget::Process)
    {
        return false;
    }

    return SetEnvironmentVariable(name, value);
}

CS2CPP_NOINLINE std::u16string Environment::StackTrace()
{
    constexpr int MaxStackTraceLineNum = 2048;

    auto frames = std::array<void*, MaxStackTraceLineNum>{};
    auto frameCount = backtrace(frames.data(), frames.size());
    if (frameCount <= 0)
    {
        return {};
    }

    // The stack backtrace requires many buffer spaces most of the time,
    // so we will reserve the size of the buffer as many as possible.
    std::string stackTrace;
    stackTrace.reserve(frameCount * 64);

    auto utf8Symbols = std::unique_ptr<char*, decltype(&std::free)>(backtrace_symbols(frames.data(), frameCount), &std::free);
    for (int i = 0; i < frameCount; ++i)
    {
        stackTrace += (*utf8Symbols)[i];
        stackTrace += '\n';
    }

    // Remove the last newline.
    if (!stackTrace.empty() && stackTrace.back() == '\n')
    {
        stackTrace.pop_back();
    }

    auto bytes = ReadOnlySpan(reinterpret_cast<const std::byte*>(stackTrace.c_str()), static_cast<int32_t>(stackTrace.size()));
    return Encoding::UTF8().GetString(bytes).value_or(std::u16string());
}

int32_t Environment::SystemPageSize()
{
    return getpagesize();
}

int32_t Environment::ProcessId()
{
    return getpid();
}

int64_t Environment::TickCount64()
{
    struct timespec tp{};
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);
}

std::u16string Environment::UserDomainName()
{
    return MachineName();
}

bool Environment::UserInteractive()
{
    return false;
}

std::u16string Environment::UserName()
{
    struct passwd pw{};
    struct passwd* result = nullptr;
    std::array<char, 2048> buffer{};
    if (getpwuid_r(getuid(), &pw, buffer.data(), buffer.size(), &result) != 0 || !result)
    {
        return {};
    }

    auto bytes = ReadOnlySpan(reinterpret_cast<const std::byte*>(pw.pw_name), static_cast<int32_t>(strlen(pw.pw_name)));
    return Encoding::UTF8().GetString(bytes).value_or(std::u16string());
}

int64_t Environment::WorkingSet()
{
    return 0;
}

bool Environment::Is64BitOperatingSystemWhen32BitProcess()
{
    return false;
}

CS2CPP_NAMESPACE_END