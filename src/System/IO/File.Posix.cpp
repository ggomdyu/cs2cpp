#include <optional>
#include <unistd.h>
#include <sys/stat.h>

#include "System/IO/File.h"
#include "System/IO/Path.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::file
{

[[nodiscard]] std::optional<struct stat> CreateStat(const std::vector<std::byte>& path)
{
    struct stat s{};
    if (stat(reinterpret_cast<const char*>(path.data()), &s) != 0)
        return {};

    return s;
}

[[nodiscard]] std::optional<struct stat> CreateStat(std::u16string_view path)
{
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(path.data()), sizeof(path[0]) * (path.length() + 1)});
    if (!utf8Path)
        return {};

    return CreateStat(*utf8Path);
}

[[nodiscard]] bool IsReadOnly(const struct stat& s) noexcept
{
    if (s.st_uid == geteuid())
        return (s.st_mode & S_IRUSR) != 0 && (s.st_mode & S_IWUSR) == 0;
    else if (s.st_gid == getegid())
        return (s.st_mode & S_IRGRP) != 0 && (s.st_mode & S_IWGRP) == 0;
    else
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
    auto utf8Path = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(fullPath.data()), sizeof(fullPath[0]) * (fullPath.length() + 1)});
    if (!utf8Path)
        return false;

    auto s = detail::file::CreateStat(*utf8Path);
    if (!s || !S_ISREG(s->st_mode))
        return false;

    return remove(reinterpret_cast<const char*>(utf8Path->data())) == 0;
}

bool File::Encrypt([[maybe_unused]] std::u16string_view path)
{
    return false;
}

bool File::Exists(std::u16string_view path)
{
    auto s = detail::file::CreateStat(Path::GetFullPath(path));
    return s && S_ISREG(s->st_mode);
}

std::optional<FileAttributes> File::GetAttributes(std::u16string_view path)
{
    auto s = detail::file::CreateStat(Path::GetFullPath(path));
    if (!s)
        return {};

    using Flags = std::underlying_type_t<FileAttributes>;
    Flags attributes{};

    if (detail::file::IsReadOnly(*s))
        attributes |= static_cast<Flags>(FileAttributes::ReadOnly);
    if ((s->st_mode & S_IFMT) == S_IFLNK)
        attributes |= static_cast<Flags>(FileAttributes::ReparsePoint);
    if ((s->st_mode & S_IFMT) == S_IFDIR)
        attributes |= static_cast<Flags>(FileAttributes::Directory);
    if (path.length() > 0 && (path[0] == '.' || (s->st_flags & UF_HIDDEN) == UF_HIDDEN))
        attributes |= static_cast<Flags>(FileAttributes::Hidden);

    return static_cast<FileAttributes>(attributes);
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
        return {};

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
        return {};

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
        return {};

#if CS2CPP_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtime);
#endif
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto srcFullPath = Path::GetFullPath(srcPath);
    auto utf8SrcFullPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(srcFullPath.data()), sizeof(srcFullPath[0]) * (srcFullPath.length() + 1)});
    if (!utf8SrcFullPath)
        return false;

    auto s = detail::file::CreateStat(*utf8SrcFullPath);
    if (!s || !S_ISREG(s->st_mode))
        return false;

    auto destFullPath = Path::GetFullPath(destPath);
    auto utf8DestFullPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(destFullPath.data()), sizeof(destFullPath[0]) * (destFullPath.length() + 1)});
    if (!utf8DestFullPath)
        return false;

    return rename(reinterpret_cast<const char*>(utf8SrcFullPath->data()),
        reinterpret_cast<const char*>(utf8DestFullPath->data())) == 0;
}

bool File::SetAttributes(std::u16string_view path, FileAttributes fileAttributes)
{
    auto fullPath = Path::GetFullPath(path);
    auto utf8FullPath = Encoding::Convert(Encoding::Unicode(), Encoding::UTF8(),
        {reinterpret_cast<const std::byte*>(fullPath.data()), sizeof(fullPath[0]) * (fullPath.length() + 1)});
    if (!utf8FullPath)
        return false;

    auto s = detail::file::CreateStat(*utf8FullPath);
    if (!s || !S_ISREG(s->st_mode))
        return false;

    // Hide the file if the hidden attribute is specified
    if (static_cast<size_t>(fileAttributes) & static_cast<size_t>(FileAttributes::Hidden))
        lchflags(reinterpret_cast<const char*>(utf8FullPath->data()), s->st_flags | UF_HIDDEN);
    else
        lchflags(reinterpret_cast<const char*>(utf8FullPath->data()), s->st_flags & (~UF_HIDDEN));

    // Set the file mode
    auto newMode = s->st_mode;
    if ((static_cast<size_t>(fileAttributes) & static_cast<size_t>(FileAttributes::ReadOnly)) != 0)
        newMode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    else if ((newMode & S_IRUSR) != 0)
        newMode |= S_IWUSR;

    return chmod(reinterpret_cast<const char*>(utf8FullPath->data()), newMode) == 0;
}

CS2CPP_NAMESPACE_END
