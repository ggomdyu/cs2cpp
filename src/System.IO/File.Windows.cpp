#include "System.IO/File.h"
#include "System.IO/Path.h"
#include "System/Windows/SafeHandle.h"

#ifndef S_ISREG
#   define S_ISREG(m) ((m & S_IFMT) == S_IFREG)
#endif

CS2CPP_NAMESPACE_BEGIN

namespace detail
{

std::optional<struct _stat> CreateStat(const std::u16string& path)
{
    struct _stat s{};
    if (_wstat(reinterpret_cast<const wchar_t*>(path.data()), &s) != 0)
    {
        return std::nullopt;
    }

    return s;
}

struct TimeTag {};
struct CreationTimeTag : TimeTag {};
struct LastAccessTimeTag : TimeTag {};
struct LastWriteTimeTag : TimeTag {};

template <typename T, typename std::enable_if_t<std::is_base_of_v<TimeTag, T>>* = nullptr>
bool InternalSetFileTime(std::u16string_view path, int64_t ticks)
{
    auto fullPath = Path::GetFullPath(path);
    auto handle = SafeHandle(CreateFile2(reinterpret_cast<LPCWSTR>(fullPath.data()), GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
    if (!handle)
    {
        return false;
    }

    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};

    if constexpr (std::is_same_v<CreationTimeTag, T>)
    {
        return !!SetFileTime(handle, &fileTime, nullptr, nullptr);
    }
    if constexpr (std::is_same_v<LastAccessTimeTag, T>)
    {
        return !!SetFileTime(handle, nullptr, &fileTime, nullptr);
    }
    if constexpr (std::is_same_v<LastWriteTimeTag, T>)
    {
        return !!SetFileTime(handle, nullptr, nullptr, &fileTime);
    }
}

}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    auto fullSrcPath = Path::GetFullPath(srcPath);
    auto fullDestPath = Path::GetFullPath(destPath);
    return CopyFileW(reinterpret_cast<LPCWSTR>(fullSrcPath.data()), reinterpret_cast<LPCWSTR>(fullDestPath.data()), overwrite ? FALSE : TRUE) == TRUE;
}

bool File::Decrypt(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    return DecryptFileW(reinterpret_cast<LPCWSTR>(fullPath.data()), 0) == TRUE;
}

bool File::Delete(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    auto s = detail::CreateStat(fullPath);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    return _wremove(reinterpret_cast<LPCWSTR>(fullPath.data())) == 0;
}

bool File::Encrypt(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    return EncryptFileW(reinterpret_cast<LPCWSTR>(fullPath.data())) == TRUE;
}

bool File::Exists(std::u16string_view path)
{
    auto s = detail::CreateStat(Path::GetFullPath(path));
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    return true;
}

std::optional<FileAttributes> File::GetAttributes(std::u16string_view path)
{
    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;

    auto fullPath = Path::GetFullPath(path);
    if (!GetFileAttributesExW(reinterpret_cast<LPCWSTR>(fullPath.data()), GetFileExInfoStandard, &fileAttributeData))
    {
        return std::nullopt;
    }

    return FileAttributes(fileAttributeData.dwFileAttributes);
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(path);
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_ctime);
}

std::optional<DateTime> File::GetLastAccessTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(path);
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_atime);
}

std::optional<DateTime> File::GetLastWriteTimeUtc(std::u16string_view path)
{
    auto s = detail::CreateStat(path);
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_mtime);
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto srcFullPath = Path::GetFullPath(srcPath);
    auto s = detail::CreateStat(srcFullPath);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    auto destFullPath = Path::GetFullPath(destPath);
    return _wrename(reinterpret_cast<LPCWSTR>(srcFullPath.data()), reinterpret_cast<LPCWSTR>(destFullPath.data())) == 0;
}

bool File::Replace(std::u16string_view srcPath, std::u16string_view destPath, std::u16string_view destBackupPath, bool ignoreMetadataErrors)
{
    auto srcFullPath = Path::GetFullPath(srcPath);
    auto destFullPath = Path::GetFullPath(destPath);
    auto destBackupFullPath = Path::GetFullPath(destBackupPath);

    return ReplaceFileW(
        reinterpret_cast<LPCWSTR>(destFullPath.data()),
        reinterpret_cast<LPCWSTR>(srcFullPath.data()),
        destBackupFullPath.empty() ? nullptr : reinterpret_cast<LPCWSTR>(destBackupFullPath.data()),
        ignoreMetadataErrors ? REPLACEFILE_IGNORE_MERGE_ERRORS : 0,
        nullptr,
        nullptr) == TRUE;
}

bool File::SetAttributes(std::u16string_view path, FileAttributes fileAttributes)
{
    auto fullPath = Path::GetFullPath(path);
    return SetFileAttributesW(reinterpret_cast<LPCWSTR>(fullPath.data()), static_cast<DWORD>(fileAttributes)) == TRUE;
}

bool File::SetCreationTime(std::u16string_view path, DateTime creationTime)
{
    return detail::InternalSetFileTime<detail::CreationTimeTag>(path, creationTime.ToFileTime());
}

bool File::SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime)
{
    return detail::InternalSetFileTime<detail::LastAccessTimeTag>(path, lastAccessTime.ToFileTime());
}

bool File::SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime)
{
    return detail::InternalSetFileTime<detail::LastWriteTimeTag>(path, lastWriteTime.ToFileTime());
}

SafeFilePointer File::InternalFileOpen(std::u16string_view path, std::u16string_view mode)
{
    FILE* fp;

    auto fullPath = Path::GetFullPath(path);
    _wfopen_s(&fp, reinterpret_cast<LPCWSTR>(fullPath.data()), reinterpret_cast<LPCWSTR>(mode.data()));

    return SafeFilePointer(fp);
}

CS2CPP_NAMESPACE_END