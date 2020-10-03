#include <optional>
#include <sys/stat.h>

#include "System/IO/File.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::file
{

std::optional<struct stat> CreateStat(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length()});

    struct stat s;
    if (stat(reinterpret_cast<const char*>(path.data()), &s) != 0)
    {
        return {};
    }

    return s;
}

}

bool File::Delete(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * path.length()});

    struct stat s;
    if (stat(reinterpret_cast<const char*>(utf8Path->data()), &s) != 0 || !S_ISREG(s.st_mode))
    {
        return false;
    }
    
    return remove(reinterpret_cast<const char*>(utf8Path->data())) == 0;
}

bool File::Exists(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }
    
    return true;
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto utf8SrcPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(srcPath.data()), sizeof(srcPath[0]) * srcPath.length()});

    struct stat s;
    if (stat(reinterpret_cast<const char*>(utf8SrcPath->data()), &s) != 0 || !S_ISREG(s.st_mode))
    {
        return false;
    }

    auto utf8DestPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(destPath.data()), sizeof(destPath[0]) * destPath.length()});

    return rename(reinterpret_cast<const char*>(utf8SrcPath->data()), reinterpret_cast<const char*>(utf8DestPath->data())) == 0;
}

bool File::Decrypt(std::u16string_view path)
{
    return false;
}

bool File::Encrypt(std::u16string_view path)
{
    return false;
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
    {
        return {};
    }

#if CS2CPP_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_birthtimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_ctime);
#endif
}

std::optional<DateTime> File::GetLastAccessTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
    {
        return {};
    }

#if CS2CPP_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_atimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_atime);
#endif
}

std::optional<DateTime> File::GetLastWriteTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
    {
        return {};
    }

#if CS2CPP_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtime);
#endif
}

CS2CPP_NAMESPACE_END