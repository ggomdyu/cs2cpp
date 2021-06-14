#include <array>

#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

extern thread_local std::vector<wchar_t> GlobalWideCharBuffer;

namespace detail::directory
{

#if !defined(S_ISDIR)
constexpr bool S_ISDIR(unsigned short m) noexcept
{
    return (m & S_IFMT) == S_IFDIR;
}
#endif

std::optional<struct _stat> CreateStat(std::u16string_view path)
{
    struct _stat s {};
    if (_wstat(reinterpret_cast<const wchar_t*>(path.data()), &s) != 0)
    {
        return {};
    }

    return s;
}

bool InternalRecursiveDelete(std::wstring& strBuffer)
{
    if (!Path::IsDirectorySeparator(strBuffer[strBuffer.length() - 1]))
    {
        strBuffer.push_back(Path::DirectorySeparatorChar);
    }

    strBuffer.push_back(u'*');

    WIN32_FIND_DATAW findData;
    const SafeFindHandle handle(FindFirstFileW(strBuffer.data(), &findData));
    if (handle == nullptr)
    {
        return false;
    }

    do
    {
        // Ignore the . and ..
        if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0' ||
            (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
        {
            continue;
        }

        auto filenameLen = std::char_traits<wchar_t>::length(findData.cFileName);
        strBuffer += std::wstring_view(findData.cFileName, filenameLen);

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            InternalRecursiveDelete(strBuffer);
        }
        else
        {
            DeleteFileW(strBuffer.data());
        }

        strBuffer.erase(filenameLen);
    } while (FindNextFileW(handle, &findData) == TRUE);

    return RemoveDirectoryW(strBuffer.data()) == TRUE;
}

}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    auto s = detail::directory::CreateStat(path);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
    {
        return false;
    }

    if (recursive)
    {
        std::wstring wideCharPath(reinterpret_cast<const wchar_t*>(path.data()), path.length());
        return detail::directory::InternalRecursiveDelete(wideCharPath);
    }

    return RemoveDirectoryW(reinterpret_cast<const wchar_t*>(path.data())) == TRUE;
}

bool Directory::Exists(std::u16string_view path)
{
    auto s = detail::directory::CreateStat(path);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
    {
        return false;
    }

    return true;
}

std::vector<String> Directory::GetLogicalDrives()
{
    const auto driveFlags = ::GetLogicalDrives();

    std::vector<String> ret;
    char16_t root[] = u"A:\\";
    for (auto d = driveFlags; d != 0; d >>= 1)
    {
        if ((d & 1) != 0)
        {
            ret.emplace_back(root, static_cast<int32_t>(std::extent_v<decltype(root)>) - 1);
        }

        ++root[0];
    }

    return ret;
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto s = detail::directory::CreateStat(srcPath);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
    {
        return false;
    }

    return _wrename(reinterpret_cast<const wchar_t*>(srcPath.data()), reinterpret_cast<const wchar_t*>(destPath.data())) == 0;
}

bool Directory::SetCurrentDirectory(std::u16string_view path)
{
    return SetCurrentDirectoryW(reinterpret_cast<const wchar_t*>(path.data())) == TRUE;
}

String Directory::GetCurrentDirectory()
{
    const auto strLen = GetCurrentDirectoryW(static_cast<DWORD>(GlobalWideCharBuffer.size()), GlobalWideCharBuffer.data());
    return String(reinterpret_cast<const char16_t*>(GlobalWideCharBuffer.data()), static_cast<int32_t>(strLen));
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    return _wmkdir(reinterpret_cast<const wchar_t*>(path.data())) == 0;
}

CS2CPP_NAMESPACE_END