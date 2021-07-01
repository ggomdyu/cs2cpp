#include <array>

#include "System/IO/File.h"
#include "System/IO/Path.h"
#include "System/Windows/SafeHandle.h"

CS2CPP_NAMESPACE_BEGIN

namespace detail::file
{

#if !defined(S_ISREG)
constexpr bool S_ISREG(unsigned short m) noexcept
{
    return (m & S_IFMT) == S_IFREG;
}
#endif

std::optional<struct _stat> CreateStat(std::u16string_view path)
{
    struct _stat s{};
    if (_wstat(reinterpret_cast<const wchar_t*>(path.data()), &s) != 0)
        return {};

    return s;
}

}

bool File::Copy(std::u16string_view srcPath, std::u16string_view destPath, bool overwrite)
{
    return CopyFileW(reinterpret_cast<LPCWSTR>(srcPath.data()), reinterpret_cast<LPCWSTR>(destPath.data()),
        overwrite ? FALSE : TRUE) != 0;
}

bool File::Decrypt(std::u16string_view path)
{
    return !!DecryptFileW(reinterpret_cast<LPCWSTR>(path.data()), 0);
}

bool File::Delete(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s || !detail::file::S_ISREG(s->st_mode))
        return false;

    return _wremove(reinterpret_cast<LPCWSTR>(path.data())) == 0;
}

bool File::Encrypt(std::u16string_view path)
{
    return !!EncryptFileW(reinterpret_cast<LPCWSTR>(path.data()));
}

bool File::Exists(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s || !detail::file::S_ISREG(s->st_mode))
        return false;

    return true;
}

std::optional<FileAttributes> File::GetAttributes(std::u16string_view path)
{
    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
    if (!GetFileAttributesExW(reinterpret_cast<LPCWSTR>(path.data()), GetFileExInfoStandard, &fileAttributeData))
        return {};

    return FileAttributes(fileAttributeData.dwFileAttributes);
}

std::optional<DateTime> File::GetCreationTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
        return {};

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_ctime);
}

std::optional<DateTime> File::GetLastAccessTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
        return {};

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_atime);
}

std::optional<DateTime> File::GetLastWriteTimeUtc(std::u16string_view path)
{
    auto s = detail::file::CreateStat(path);
    if (!s)
        return {};

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtime);
}

bool File::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto s = detail::file::CreateStat(srcPath);
    if (!s || !detail::file::S_ISREG(s->st_mode))
        return false;

    return _wrename(reinterpret_cast<LPCWSTR>(srcPath.data()), reinterpret_cast<LPCWSTR>(destPath.data())) == 0;
}

bool File::Replace(std::u16string_view srcPath, std::u16string_view destPath, const std::u16string_view* destBackupPath, bool ignoreMetadataErrors)
{
    return ReplaceFileW(reinterpret_cast<LPCWSTR>(&destPath[0]), reinterpret_cast<LPCWSTR>(&srcPath[0]),
        destBackupPath ? reinterpret_cast<LPCWSTR>(&destBackupPath[0]) : nullptr,
        ignoreMetadataErrors ? REPLACEFILE_IGNORE_MERGE_ERRORS : 0, nullptr, nullptr);
}

bool File::SetAttributes(std::u16string_view path, FileAttributes fileAttributes)
{
    return !!SetFileAttributesW(reinterpret_cast<LPCWSTR>(path.data()), static_cast<DWORD>(fileAttributes));
}

bool File::SetCreationTimeUtc(std::u16string_view path, DateTime creationTimeUtc)
{
    SafeHandle handle(CreateFile2(reinterpret_cast<LPCWSTR>(path.data()), GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
    if (!handle)
        return false;

    auto ticks = creationTimeUtc.ToFileTimeUtc();

    auto fileTime = FILETIME{};
    fileTime.dwLowDateTime = static_cast<DWORD>(ticks);
    fileTime.dwHighDateTime = static_cast<DWORD>(ticks >> 32);

    return SetFileTime(handle, &fileTime, nullptr, nullptr);
}

bool File::SetLastAccessTimeUtc(std::u16string_view path, DateTime lastAccessTimeUtc)
{
    SafeHandle handle(CreateFile2(reinterpret_cast<LPCWSTR>(path.data()), GENERIC_WRITE, FILE_SHARE_READ,
        OPEN_EXISTING, nullptr));
    if (!handle)
        return false;

    auto ticks = lastAccessTimeUtc.ToFileTimeUtc();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};

    return SetFileTime(handle, nullptr, &fileTime, nullptr);
}

bool File::SetLastWriteTimeUtc(std::u16string_view path, DateTime lastWriteTimeUtc)
{
    SafeHandle handle(CreateFile2(reinterpret_cast<LPCWSTR>(path.data()), GENERIC_WRITE, FILE_SHARE_READ,
        OPEN_EXISTING, nullptr));
    if (!handle)
        return false;

    auto ticks = lastWriteTimeUtc.ToFileTimeUtc();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};

    return SetFileTime(handle, nullptr, nullptr, &fileTime);
}

SafeFilePointer File::InternalFileOpen(std::u16string_view path, std::u16string_view mode)
{
    FILE* fp;
    _wfopen_s(&fp, reinterpret_cast<LPCWSTR>(path.data()), reinterpret_cast<LPCWSTR>(mode.data()));

    return SafeFilePointer(fp);
}

CS2CPP_NAMESPACE_END