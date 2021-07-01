#include <array>

#include "System/IO/Directory.h"

CS2CPP_NAMESPACE_BEGIN

extern std::array<wchar_t, 16384> GlobalWideCharBuffer;

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
    if (_wstat(reinterpret_cast<LPCWSTR>(path.data()), &s) != 0)
        return {};

    return s;
}

bool InternalRecursiveDelete(std::wstring& strBuffer)
{
    if (!Path::IsDirectorySeparator(strBuffer.back()))
        strBuffer.push_back(Path::DirectorySeparatorChar);

    // Add a wildcard
    strBuffer.push_back(u'*');

    WIN32_FIND_DATAW findData;
    SafeFindHandle handle(FindFirstFileW(strBuffer.data(), &findData));
    if (!handle)
        return false;

    // Remove the wildcard
    strBuffer.pop_back();

    do
    {
        // Ignore the . and ..
        if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0'
            || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
            continue;

        auto filename = std::wstring_view(findData.cFileName);
        auto rollbackLength = strBuffer.size();
        strBuffer += filename;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            InternalRecursiveDelete(strBuffer);
        else
            DeleteFileW(strBuffer.data());

        strBuffer.erase(rollbackLength, strBuffer.size() - rollbackLength);
    } while (FindNextFileW(handle, &findData) == TRUE);

    return RemoveDirectoryW(strBuffer.data()) == TRUE;
}

}

bool Directory::Delete(std::u16string_view path, bool recursive)
{
    using namespace detail::directory;

    auto s = CreateStat(path);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
        return false;

    if (recursive)
    {
        std::wstring wideCharPath(reinterpret_cast<LPCWSTR>(path.data()), path.length());
        return InternalRecursiveDelete(wideCharPath);
    }

    return RemoveDirectoryW(reinterpret_cast<LPCWSTR>(path.data())) == TRUE;
}

bool Directory::Exists(std::u16string_view path)
{
    auto s = detail::directory::CreateStat(path);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
        return false;

    return true;
}

std::vector<std::u16string> Directory::GetLogicalDrives()
{
    auto driveFlags = ::GetLogicalDrives();

    std::vector<std::u16string> ret;
    char16_t root[] = u"A:\\";
    for (auto d = driveFlags; d != 0; d >>= 1)
    {
        if ((d & 1) != 0)
            ret.emplace_back(root, static_cast<int32_t>(std::extent_v<decltype(root)>) - 1);

        ++root[0];
    }

    return ret;
}

bool Directory::Move(std::u16string_view srcPath, std::u16string_view destPath)
{
    auto s = detail::directory::CreateStat(srcPath);
    if (!s.has_value() || !detail::directory::S_ISDIR(s->st_mode))
        return false;

    return _wrename(reinterpret_cast<LPCWSTR>(srcPath.data()), reinterpret_cast<LPCWSTR>(destPath.data())) == 0;
}

bool Directory::SetCurrentDirectory(std::u16string_view path)
{
    return SetCurrentDirectoryW(reinterpret_cast<LPCWSTR>(path.data())) == TRUE;
}

std::u16string Directory::GetCurrentDirectory()
{
    auto str = reinterpret_cast<const char16_t*>(GlobalWideCharBuffer.data());
    auto strLen = static_cast<size_t>(GetCurrentDirectoryW(static_cast<DWORD>(GlobalWideCharBuffer.size()),
        GlobalWideCharBuffer.data()));

    return std::u16string(str, strLen);
}

bool Directory::InternalCreateDirectory(std::u16string_view path)
{
    return _wmkdir(reinterpret_cast<LPCWSTR>(path.data())) == 0;
}

CS2CPP_NAMESPACE_END