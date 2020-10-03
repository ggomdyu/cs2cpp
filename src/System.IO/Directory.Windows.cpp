#include "System.IO/Directory.h"

#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

CS2CPP_NAMESPACE_BEGIN

extern std::optional<struct _stat> CreateStat(std::u16string_view path);

namespace
{

bool InternalRecursiveDelete(std::wstring& strBuffer)
{
    if (!Path::IsDirectorySeparator(strBuffer.back()))
    {
        strBuffer.push_back(Path::DirectorySeparatorChar);
    }

    auto wildcard = u'*';
    strBuffer.push_back(wildcard);

    WIN32_FIND_DATAW findData;
    SafeFindHandle handle(FindFirstFileW(strBuffer.data(), &findData));
    if (!handle)
    {
        return false;
    }

    strBuffer.pop_back();

    do
    {
        // Ignore the . and ..
        if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0'
            || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
        {
            continue;
        }

        auto filename = std::wstring_view(findData.cFileName);
        auto rollbackLength = strBuffer.size();
        strBuffer += filename;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            InternalRecursiveDelete(strBuffer);
        }
        else
        {
            DeleteFileW(strBuffer.data());
        }

        strBuffer.erase(rollbackLength, strBuffer.size() - rollbackLength);
    } while (FindNextFileW(handle, &findData) == TRUE);

    return RemoveDirectoryW(strBuffer.data()) == TRUE;
}

}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    auto fullPath = Path::GetFullPath(path);
    auto s = CreateStat(fullPath);
    if (!s.has_value() || !S_ISDIR(s->st_mode))
    {
        return false;
    }

    if (recursive)
    {
        std::wstring wideCharPath(reinterpret_cast<LPCWSTR>(fullPath.data()), fullPath.length());
        return InternalRecursiveDelete(wideCharPath);
    }

    return RemoveDirectoryW(reinterpret_cast<LPCWSTR>(fullPath.data())) == TRUE;
}

bool Directory::Exists(std::u16string_view path)
{
    auto fullPath = Path::GetFullPath(path);
    auto s = CreateStat(fullPath);
    return s.has_value() && S_ISDIR(s->st_mode);
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto srcFullPath = Path::GetFullPath(srcPath);
    auto s = CreateStat(srcFullPath);
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