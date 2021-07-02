#include <algorithm>

#include "System/IO/Path.h"
#include "System/Windows/Windows.h"

CS2CPP_NAMESPACE_BEGIN

extern std::array<wchar_t, 16384> GlobalWideCharBuffer;

std::u16string Path::GetDirectoryName(std::u16string_view path)
{
    auto rootLen = static_cast<size_t>(Path::GetRootLength(path));
    auto index = path.length();
    while (rootLen < index && !IsDirectorySeparator(path[--index]));

    std::u16string ret(path.substr(0, index));
    std::replace(ret.begin(), ret.end(), AltDirectorySeparatorChar, DirectorySeparatorChar);

    return ret;
}

std::u16string Path::GetFullPath(std::u16string_view path)
{
    if (path.length() <= 0)
        return {};

    DWORD fullPathLen = GetFullPathNameW(reinterpret_cast<LPCWSTR>(path.data()),
        static_cast<DWORD>(GlobalWideCharBuffer.size()), GlobalWideCharBuffer.data(), nullptr);

    // MSDN says GetFullPathNameW returns the length of the path, not including the terminating null character,
    // but when I checked it, the terminating null character included in the length. Why?
    size_t fixedFullPathLen = GlobalWideCharBuffer[fullPathLen - 1] == u'\0' ? fullPathLen - 1 : fullPathLen;

    return std::u16string(reinterpret_cast<const char16_t*>(GlobalWideCharBuffer.data()), fixedFullPathLen);
}

std::u16string Path::GetPathRoot(std::u16string_view path)
{
    std::u16string root(path.data(), GetRootLength(path));
    std::replace(root.begin(), root.end(), AltDirectorySeparatorChar, DirectorySeparatorChar);

    return root;
}

std::u16string Path::GetTempPath()
{
    auto path = GlobalWideCharBuffer.data();
    auto pathLen = static_cast<size_t>(GetTempPathW(MAX_PATH, path));

    return std::u16string(reinterpret_cast<const char16_t*>(path), pathLen);
}

CS2CPP_NAMESPACE_END
