#pragma once

#include <string_view>

CS2CPP_NAMESPACE_BEGIN

class WindowsPath
{
public:
    WindowsPath() = delete;

protected:
    static constexpr bool IsExtendedPath(std::u16string_view path) noexcept;

protected:
    static constexpr int32_t DevicePrefixLength = 4;
    static constexpr char16_t DirectorySeparatorChar = u'\\';
    static constexpr char16_t AltDirectorySeparatorChar = u'/';
    static constexpr char16_t VolumeSeparatorChar = u':';
    static constexpr char16_t PathSeparator = u';';
};

constexpr bool WindowsPath::IsExtendedPath(std::u16string_view path) noexcept
{
    return path.length() >= DevicePrefixLength
        && path[0] == '\\'
        && (path[1] == '\\' || path[1] == '?')
        && path[2] == '?'
        && path[3] == '\\';
}

using PlatformPath = WindowsPath;

CS2CPP_NAMESPACE_END