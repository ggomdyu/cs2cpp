#include <sys/stat.h>

#include "System.IO/Directory.h"
#include "System.IO/Path.h"
#include "System/Windows/Windows.h"

#ifndef S_ISDIR
#    define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#endif

CS2CPP_NAMESPACE_BEGIN

extern std::optional<struct _stat> CreateStat(const std::u16string& path);

static bool InternalRecursiveDelete(std::wstring& buffer)
{
    if (!Path::IsDirectorySeparator(buffer.back()))
    {
        buffer.push_back(Path::DirectorySeparatorChar);
    }
    buffer.push_back(u'*');

    // Create a file search handle.
    auto deleter = [](void* handle)
    {
        if (handle != INVALID_HANDLE_VALUE)
        {
            FindClose(handle);
        }
    };
    WIN32_FIND_DATAW findData;
    std::unique_ptr<void, void (*)(void*)> handle(FindFirstFileW(buffer.data(), &findData), deleter);
    if (handle.get() == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // Remove the wildcard.
    buffer.pop_back();

    do
    {
        // Ignore the . and ..
        if (findData.cFileName[0] == L'.' &&
            (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
        {
            continue;
        }

        size_t rollbackNum = buffer.size();
        buffer += findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            InternalRecursiveDelete(buffer);
        }
        else
        {
            DeleteFileW(buffer.data());
        }

        buffer.erase(rollbackNum, buffer.size() - rollbackNum);
    } while (FindNextFileW(handle.get(), &findData) == TRUE);

    return RemoveDirectoryW(buffer.data()) == TRUE;
}

static bool InternalRecursiveDelete(const std::u16string& path)
{
    std::wstring buffer(reinterpret_cast<const wchar_t*>(path.c_str()), path.length());
    return InternalRecursiveDelete(buffer);
}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    std::u16string fullPath = Path::GetFullPath(path);
    std::optional<struct _stat> s = CreateStat(fullPath);
    if (!s.has_value() || !S_ISDIR(s->st_mode))
    {
        return false;
    }

    if (recursive)
    {
        return InternalRecursiveDelete(fullPath);
    }

    return RemoveDirectoryW(reinterpret_cast<LPCWSTR>(fullPath.data())) == TRUE;
}

bool Directory::Exists(std::u16string_view path)
{
    std::u16string fullPath = Path::GetFullPath(path);
    std::optional<struct _stat> s = CreateStat(fullPath);
    return s.has_value() && S_ISDIR(s->st_mode);
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    std::u16string srcFullPath = Path::GetFullPath(srcPath);
    std::optional<struct _stat> s = CreateStat(srcFullPath);
    if (!s.has_value() || !S_ISDIR(s->st_mode))
    {
        return false;
    }

    auto destFullPath = Path::GetFullPath(destPath);
    return _wrename(reinterpret_cast<LPCWSTR>(srcFullPath.data()), reinterpret_cast<LPCWSTR>(destFullPath.data())) == 0;
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    return _wmkdir(reinterpret_cast<LPCWSTR>(fullPath.data())) == 0;
}

CS2CPP_NAMESPACE_END