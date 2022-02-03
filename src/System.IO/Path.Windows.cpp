#include <algorithm>
#include <array>

#include "System.IO/Path.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Path::GetFullPath(std::u16string_view path)
{
    std::wstring widePath(reinterpret_cast<const wchar_t*>(path.data()), path.length());
    DWORD length = GetFullPathNameW(widePath.data(), 0, nullptr, nullptr);
    if (length <= 0)
    {
        return {};
    }

    std::u16string ret;
    ret.resize(length - 1);
    GetFullPathNameW(widePath.data(), length, reinterpret_cast<LPWSTR>(ret.data()), nullptr);

    return ret;
}

std::u16string Path::GetPathRoot(std::u16string_view path)
{
    std::u16string root(path.data(), GetRootLength(path));
    std::replace(root.begin(), root.end(), AltDirectorySeparatorChar, DirectorySeparatorChar);

    return root;
}

std::u16string Path::GetTempPath()
{
    std::array<WCHAR, 4096> path = {};
    DWORD pathLen = GetTempPathW(static_cast<int>(path.size()), path.data());

    return std::u16string(reinterpret_cast<const char16_t*>(path.data()), static_cast<size_t>(pathLen));
}

CS2CPP_NAMESPACE_END
