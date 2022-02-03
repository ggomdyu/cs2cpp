#include <sys/stat.h>

#include "System.IO/File.h"
#include "System.IO/Path.h"
#include "System/Windows/Windows.h"

#ifndef S_ISREG
#    define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

CS2CPP_NAMESPACE_BEGIN

std::optional<struct _stat> CreateStat(const std::u16string& path)
{
    struct _stat s{};
    if (_wstat(reinterpret_cast<const wchar_t*>(path.data()), &s) != 0)
    {
        return std::nullopt;
    }

    return s;
}

static std::unique_ptr<void, void (*)(void*)> OpenFileHandle(std::u16string_view path)
{
    auto deleter = [](void* handle)
    {
        if (handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(handle);
        }
    };

    std::u16string fullPath = Path::GetFullPath(path);
    return {CreateFileW(reinterpret_cast<LPCWSTR>(fullPath.data()), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr), deleter};
}

static bool InternalSetFileTime(std::u16string_view path, std::tuple<const FILETIME*, const FILETIME*, const FILETIME*> times)
{
    std::unique_ptr<void, void (*)(void*)> handle = OpenFileHandle(path);
    return SetFileTime(handle.get(), std::get<0>(times), std::get<1>(times), std::get<2>(times)) == TRUE;
}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    std::u16string fullSrcPath = Path::GetFullPath(srcPath);
    std::u16string fullDestPath = Path::GetFullPath(destPath);
    return CopyFileW(reinterpret_cast<LPCWSTR>(fullSrcPath.data()), reinterpret_cast<LPCWSTR>(fullDestPath.data()), overwrite ? FALSE : TRUE) == TRUE;
}

bool File::Decrypt(std::u16string_view path)
{
    std::u16string fullPath = Path::GetFullPath(path);
    return DecryptFileW(reinterpret_cast<LPCWSTR>(fullPath.data()), 0) == TRUE;
}

bool File::Delete(std::u16string_view path)
{
    std::u16string fullPath = Path::GetFullPath(path);
    auto s = CreateStat(fullPath);
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
    auto s = CreateStat(Path::GetFullPath(path));
    return s && S_ISREG(s->st_mode);
}

std::optional<FileAttributes> File::GetAttributes(std::u16string_view path)
{
    std::u16string fullPath = Path::GetFullPath(path);
    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
    if (GetFileAttributesExW(reinterpret_cast<LPCWSTR>(fullPath.data()), GetFileExInfoStandard, &fileAttributeData) == FALSE)
    {
        return std::nullopt;
    }

    return FileAttributes(fileAttributeData.dwFileAttributes);
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = CreateStat(std::u16string(path));
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_ctime);
}

std::optional<DateTime> File::GetLastAccessTimeUtc(std::u16string_view path)
{
    auto s = CreateStat(std::u16string(path));
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_atime);
}

std::optional<DateTime> File::GetLastWriteTimeUtc(std::u16string_view path)
{
    auto s = CreateStat(std::u16string(path));
    if (!s)
    {
        return std::nullopt;
    }

    return DateTime(DateTime::UnixEpoch().Ticks() + TimeSpan::TicksPerSecond * s->st_mtime);
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    std::u16string srcFullPath = Path::GetFullPath(srcPath);
    auto s = CreateStat(srcFullPath);
    if (!s || !S_ISREG(s->st_mode))
    {
        return false;
    }

    std::u16string destFullPath = Path::GetFullPath(destPath);
    return _wrename(reinterpret_cast<LPCWSTR>(srcFullPath.data()), reinterpret_cast<LPCWSTR>(destFullPath.data())) == 0;
}

bool File::Replace(std::u16string_view srcPath, std::u16string_view destPath, std::u16string_view destBackupPath, bool ignoreMetadataErrors)
{
    std::u16string srcFullPath = Path::GetFullPath(srcPath);
    std::u16string destFullPath = Path::GetFullPath(destPath);
    std::u16string destBackupFullPath = Path::GetFullPath(destBackupPath);
    return ReplaceFileW(reinterpret_cast<LPCWSTR>(destFullPath.data()), reinterpret_cast<LPCWSTR>(srcFullPath.data()), destBackupFullPath.empty() ? nullptr : reinterpret_cast<LPCWSTR>(destBackupFullPath.data()), ignoreMetadataErrors ? REPLACEFILE_IGNORE_MERGE_ERRORS : 0, nullptr, nullptr) == TRUE;
}

bool File::SetAttributes(std::u16string_view path, FileAttributes fileAttributes)
{
    std::u16string fullPath = Path::GetFullPath(path);
    return SetFileAttributesW(reinterpret_cast<LPCWSTR>(fullPath.data()), static_cast<DWORD>(fileAttributes)) == TRUE;
}

bool File::SetCreationTime(std::u16string_view path, DateTime creationTime)
{
    int64_t ticks = creationTime.ToFileTime();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};
    return InternalSetFileTime(path, {&fileTime, nullptr, nullptr});
}

bool File::SetLastAccessTime(std::u16string_view path, DateTime lastAccessTime)
{
    int64_t ticks = lastAccessTime.ToFileTime();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};
    return InternalSetFileTime(path, {nullptr, &fileTime, nullptr});
}

bool File::SetLastWriteTime(std::u16string_view path, DateTime lastWriteTime)
{
    int64_t ticks = lastWriteTime.ToFileTime();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};
    return InternalSetFileTime(path, {nullptr, nullptr, &fileTime});
}

CS2CPP_NAMESPACE_END