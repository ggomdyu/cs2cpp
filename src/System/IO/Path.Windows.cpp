#include <algorithm>

#include "System/IO/Path.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

std::u16string Path::GetDirectoryName(std::u16string_view path)
{
    auto rootLen = Path::GetRootLength(path);
    auto index = path.length();
    while (rootLen < index && !IsDirectorySeparator(path[--index]));

    std::u16string ret(path.substr(0, index));
    std::replace(ret.begin(), ret.end(), AltDirectorySeparatorChar, DirectorySeparatorChar);

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
    auto path = std::array<wchar_t, MAX_PATH + 1>{};
    auto pathLen = GetTempPathW(MAX_PATH, path.data());

    return {reinterpret_cast<const char16_t*>(path.data()), static_cast<size_t>(pathLen)};
}

CS2CPP_NAMESPACE_END
