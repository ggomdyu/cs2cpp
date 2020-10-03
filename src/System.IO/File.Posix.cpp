#include <optional>
#include <unistd.h>
#include <sys/stat.h>

#include "System.IO/File.h"
#include "System.IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail
{

std::optional<struct stat> CreateStat(const std::u16string& path)
{
    auto utf8Path = Encoding::UTF8().GetBytes(path);
    if (!utf8Path)
    {
        return std::nullopt;
    }

    struct stat s{};
    if (stat(reinterpret_cast<const char*>(utf8Path->data()), &s) != 0)
    {
        return std::nullopt;
    }

    return s;
}

bool IsReadOnly(const struct stat& s) noexcept
{
    if (s.st_uid == geteuid())
    {
        return (s.st_mode & S_IRUSR) != 0 && (s.st_mode & S_IWUSR) == 0;
    }
    if (s.st_gid == getegid())
    {
        return (s.st_mode & S_IRGRP) != 0 && (s.st_mode & S_IWGRP) == 0;
    }

    return (s.st_mode & S_IROTH) != 0 && (s.st_mode & S_IWOTH) == 0;
}

}

bool File::Decrypt([[maybe_unused]] std::u16string_view path)
{
    return false;
}

bool File::Delete(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    auto utf8FullPath = Encoding::UTF8().GetBytes(fullPath);
    if (!utf8FullPath)
    {
        return false;
    }

    return unlink(reinterpret_cast<const char*>(utf8FullPath->data())) == 0;
}

bool File::Encrypt([[maybe_unused]] std::u16string_view path)
{
    return false;
}

bool File::Exists(std::u16string_view path)
{
    auto s = detail::CreateStat(Path::GetFullPath(path));
    return s && S_ISREG(s->st_mode);
}

std::optional<FileAttributes> File::GetAttributes(std::u16string_view path)
{
    auto s = detail::CreateStat(Path::GetFullPath(path));
    if (!s)
    {
        return std::nullopt;
    }

    size_t attributes = 0;
    if (detail::IsReadOnly(*s))
    {
        attributes |= static_cast<size_t>(FileAttributes::ReadOnly);
    }
    if (S_ISLNK(s->st_mode))
    {
        attributes |= static_cast<size_t>(FileAttributes::ReparsePoint);
    }
    if (S_ISDIR(s->st_mode))
    {
        attributes |= static_cast<size_t>(FileAttributes::Directory);
    }
    if ((s->st_flags & UF_HIDDEN) || (path.length() > 0 && (path[0] == '.')))
    {
        attributes |= static_cast<size_t>(FileAttributes::Hidden);
    }

    return static_cast<FileAttributes>(attributes);
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(std::u16string(path));
    if (!s)
    {
        return std::nullopt;
    }

#if CS2CPP_PLATFORM_DARWIN
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_birthtimespec.tv_sec);
#else
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_ctime);
#endif
}

std::optional<DateTime> File::GetLastAccessTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(path);
    if (!s)
    {
        return std::nullopt;
    }

#if CS2CPP_PLATFORM_DARWIN
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_atimespec.tv_sec);
#else
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_atime);
#endif
}

std::optional<DateTime> File::GetLastWriteTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(path);
    if (!s)
    {
        return std::nullopt;
    }

#if CS2CPP_PLATFORM_DARWIN
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_mtimespec.tv_sec);
#else
    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_mtime);
#endif
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto srcFullPath = Path::GetFullPath(srcPath);
    auto utf8SrcFullPath = Encoding::UTF8().GetBytes(srcFullPath);
    if (!utf8SrcFullPath)
    {
        return false;
    }

    auto s = detail::CreateStat(*utf8SrcFullPath);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    auto destFullPath = Path::GetFullPath(destPath);
    auto utf8DestFullPath = Encoding::UTF8().GetBytes(destFullPath);
    if (!utf8DestFullPath)
    {
        return false;
    }

    return rename(reinterpret_cast<const char*>(utf8SrcFullPath->data()), reinterpret_cast<const char*>(utf8DestFullPath->data())) == 0;
}

bool File::SetAttributes(std::u16string_view path, FileAttributes fileAttributes)
{
    auto fullPath = Path::GetFullPath(path);
    auto utf8FullPath = Encoding::UTF8().GetBytes(fullPath);
    if (!utf8FullPath)
    {
        return false;
    }

    auto s = detail::CreateStat(*utf8FullPath);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    // Hide the file if the hidden attribute is specified
    if (static_cast<size_t>(fileAttributes) & static_cast<size_t>(FileAttributes::Hidden))
    {
        lchflags(reinterpret_cast<const char*>(utf8FullPath->data()), s->st_flags | UF_HIDDEN);
    }
    else
    {
        lchflags(reinterpret_cast<const char*>(utf8FullPath->data()), s->st_flags & (~UF_HIDDEN));
    }

    // Set the file mode
    auto newMode = s->st_mode;
    if ((static_cast<size_t>(fileAttributes) & static_cast<size_t>(FileAttributes::ReadOnly)) != 0)
    {
        newMode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    }
    else if ((newMode & S_IRUSR) != 0)
    {
        newMode |= S_IWUSR;
    }

    return chmod(reinterpret_cast<const char*>(utf8FullPath->data()), newMode) == 0;
}

CS2CPP_NAMESPACE_END
